/*
 * @Author: gongluck
 * @Date: 2025-01-06 16:48:50
 * @Last Modified by: gongluck
 * @Last Modified time: 2025-01-17 17:15:32
 */

package main

import (
	"context"
	"flag"
	"fmt"
	"net/url"
	"strings"

	"github.com/eclipse/paho.golang/autopaho"
	"github.com/eclipse/paho.golang/paho"
	"github.com/google/uuid"
)

var (
	broker   = flag.String("broker", "tcp://localhost:1883", "Broker Address")
	clientId = flag.String("clientId", uuid.NewString(), "Client Id")
	user     = flag.String("user", "admin", "User Name")
	passwd   = flag.String("passwd", "admin", "Password")
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

	startClient(cliCfg, *clientId)

	for {
	}
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
			topicParts := strings.Split(pr.Packet.Topic, "/")
			if len(topicParts) >= 3 {
				if topicParts[2] == "state" {
					topic := "devices/" + topicParts[1] + "/command"
					// fmt.Println("Send ", topic)
					if _, err := pr.Client.Publish(context.Background(), &paho.Publish{
						QoS:     1,
						Topic:   topic,
						Payload: []byte(uuid.NewString()),
					}); err != nil {
						fmt.Printf("publish %s, error: %v\n", topic, err)
					}
				} else if topicParts[2] == "result" {
					// fmt.Printf("%s Received result message: %s from topic: %s\n", id, pr.Packet.Payload(), pr.Packet.Topic())
				} else if topicParts[2] == "will" {
					fmt.Printf("%s Received will message: %s from topic: %s\n", id, pr.Packet.Payload, pr.Packet.Topic)
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
	subscribe(c, "+")
}

func subscribe(client *autopaho.ConnectionManager, id string) {
	topic := "devices/" + id + "/state"
	// fmt.Println(id, " subscribe ", topic)
	if _, err := client.Subscribe(context.Background(), &paho.Subscribe{
		Subscriptions: []paho.SubscribeOptions{
			{Topic: topic, QoS: 1},
		},
	}); err != nil {
		fmt.Printf("subscribe %s, error: %v\n", topic, err)
	}

	topic = "devices/" + id + "/result"
	// fmt.Println(id, " subscribe ", topic)
	if _, err := client.Subscribe(context.Background(), &paho.Subscribe{
		Subscriptions: []paho.SubscribeOptions{
			{Topic: topic, QoS: 1},
		},
	}); err != nil {
		fmt.Printf("subscribe %s, error: %v\n", topic, err)
	}

	topic = "devices/" + id + "/will"
	// fmt.Println(id, " subscribe ", topic)
	if _, err := client.Subscribe(context.Background(), &paho.Subscribe{
		Subscriptions: []paho.SubscribeOptions{
			{Topic: topic, QoS: 1},
		},
	}); err != nil {
		fmt.Printf("subscribe %s, error: %v\n", topic, err)
	}
}
