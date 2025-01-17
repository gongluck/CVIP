/*
 * @Author: gongluck
 * @Date: 2025-01-06 16:48:50
 * @Last Modified by: gongluck
 * @Last Modified time: 2025-01-17 17:05:25
 */

package main

import (
	"context"
	"encoding/json"
	"flag"
	"fmt"
	"net/url"
	"sync"
	"time"

	"github.com/eclipse/paho.golang/autopaho"
	"github.com/eclipse/paho.golang/paho"

	"github.com/google/uuid"
)

var (
	broker      = flag.String("broker", "tcp://localhost:1883", "Broker Address")
	clientId    = flag.String("clientId", uuid.NewString(), "Client Id")
	concurrency = flag.Int("concurrency", 0, "Concurrency Count")
	user        = flag.String("user", "admin", "User Name")
	passwd      = flag.String("passwd", "admin", "Password")
)

func main() {
	flag.Parse()

	fmt.Println("broker: ", *broker)
	u, err := url.Parse(*broker)
	if err != nil {
		panic(err)
	}

	cliCfg := autopaho.ClientConfig{
		ServerUrls:                    []*url.URL{u},
		ConnectUsername:               *user,
		ConnectPassword:               []byte(*passwd),
		KeepAlive:                     30,
		CleanStartOnInitialConnection: true,
		SessionExpiryInterval:         60,
	}

	wg := sync.WaitGroup{}
	for i := 0; i < *concurrency; i++ {
		wg.Add(1)
		go func() {
			id := uuid.NewString()
			fmt.Printf("Start client: %d: %s\n", i, id)
			startClient(cliCfg, id)
			wg.Done()
		}()
		time.Sleep(time.Millisecond * 5)
	}

	wg.Add(1)
	go func() {
		startClient(cliCfg, *clientId)
		wg.Done()
	}()

	wg.Wait()
}

func startClient(cliCfg autopaho.ClientConfig, id string) {
	cliCfg.ClientID = id
	cliCfg.OnConnectionUp = func(cm *autopaho.ConnectionManager, c *paho.Connack) {
		fmt.Println(id, " OnConnectionUp: ", c)
	}
	cliCfg.OnConnectError = func(err error) { fmt.Println(id, " OnConnectError: ", err) }
	cliCfg.OnPublishReceived = []func(paho.PublishReceived) (bool, error){
		func(pr paho.PublishReceived) (bool, error) {
			// fmt.Printf("received message on topic %s; body: %s (retain: %t)\n", pr.Packet.Topic, pr.Packet.Payload, pr.Packet.Retain)
			if pr.Packet.Topic == "devices/"+id+"/command" {
				topic := "devices/" + id + "/result"
				if _, err := pr.Client.Publish(context.Background(), &paho.Publish{
					QoS:     1,
					Topic:   topic,
					Payload: []byte(uuid.NewString()),
				}); err != nil {
					fmt.Printf("publish %s, error: %v\n", topic, err)
				}
			}

			return true, nil
		}}
	//遗言消息
	cliCfg.WillMessage = &paho.WillMessage{
		Retain: true, QoS: 1, Topic: "devices/" + id + "/will", Payload: []byte(id + " offline"),
	}

	c, err := autopaho.NewConnection(context.Background(), cliCfg)
	if err != nil {
		fmt.Printf("NewConnection %s, error: %v\n", id, err)
	}
	if err = c.AwaitConnection(context.Background()); err != nil {
		fmt.Printf("AwaitConnection %s, error: %v\n", id, err)
	}
	subscribe(c, id)
	publish(c, id)
}

type msg struct {
	Id string
	N  uint64
	A  string
	B  string
	C  string
	D  string
}

func publish(client *autopaho.ConnectionManager, id string) {
	var i uint64

	for {
		i++
		msg := msg{
			Id: id,
			N:  i,
			A:  uuid.NewString(),
			B:  uuid.NewString(),
			C:  uuid.NewString(),
			D:  uuid.NewString(),
		}
		str, err := json.Marshal(msg)
		if err != nil {
			fmt.Println("Marshal error: ", err)
		}

		topic := "devices/" + id + "/state"
		// fmt.Println("publish: " + topic)
		if _, err = client.Publish(context.Background(), &paho.Publish{
			QoS:     1,
			Topic:   topic,
			Payload: []byte(str),
		}); err != nil {
			fmt.Printf("publish %s, error: %v\n", topic, err)
		}

		time.Sleep(time.Second * 1)
	}
}

func subscribe(client *autopaho.ConnectionManager, id string) {
	topic := "devices/" + id + "/command"
	// fmt.Println(id, " subscribe ", topic)
	if _, err := client.Subscribe(context.Background(), &paho.Subscribe{
		Subscriptions: []paho.SubscribeOptions{
			{Topic: topic, QoS: 1},
		},
	}); err != nil {
		fmt.Printf("subscribe %s, error: %v\n", topic, err)
	}
}
