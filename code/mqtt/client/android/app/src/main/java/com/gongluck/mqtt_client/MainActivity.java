package com.gongluck.mqtt_client;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.gson.Gson;

import org.eclipse.paho.mqttv5.client.IMqttToken;
import org.eclipse.paho.mqttv5.client.MqttCallback;
import org.eclipse.paho.mqttv5.client.MqttClient;
import org.eclipse.paho.mqttv5.client.MqttConnectionOptions;
import org.eclipse.paho.mqttv5.client.MqttDisconnectResponse;
import org.eclipse.paho.mqttv5.client.persist.MemoryPersistence;
import org.eclipse.paho.mqttv5.common.MqttException;
import org.eclipse.paho.mqttv5.common.MqttMessage;
import org.eclipse.paho.mqttv5.common.packet.MqttProperties;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    static String TAG = "MQTT_Client";
    private MqttClient mClient;
    private String mBroker = "tcp://192.168.33.94:1883";
    private String mClientId = UUID.randomUUID().toString();
    //订阅设备相关主题
    //private String mSubTopic = "devices/" + mClientId + "/#";
    //订阅设备任务主题
    private String mTaskTopic = "devices/" + mClientId + "/command";
    //发布设备状态主题
    private String mPubTopic = "devices/" + mClientId + "/state";
    //发布设备任务结果主题
    private String mResultTopic = "devices/" + mClientId + "/result";
    //发布设备遗言主题
    private String mWillTopic = "devices/" + mClientId + "/will";
    private int mQos = 1;
    private int mMsgNo = 0;
    private Runnable mPublishRunnable = new Runnable() {
        @Override
        public void run() {
            PublishMsg("", "");
            // 再次调度下一次执行，间隔1秒
            new Handler(Looper.getMainLooper()).postDelayed(this, 10);
        }
    };

    void ConnectMqtt() {
        DisconnectMqtt();

        MemoryPersistence persistence = new MemoryPersistence();
        try {
            //client实例
            mClient = new MqttClient(mBroker, mClientId, persistence);

            //连接参数
            MqttConnectionOptions connOpts = new MqttConnectionOptions();
            connOpts.setUserName("admin");
            connOpts.setPassword("admin".getBytes());
            // 是否清除会话
            connOpts.setCleanStart(true);
            // 心跳间隔，单位为秒
            connOpts.setKeepAliveInterval(30);
            // 连接超时时间，单位为秒
            connOpts.setConnectionTimeout(60);
            // 是否自动重连
            connOpts.setAutomaticReconnect(true);
            //https://docs.emqx.com/zh/emqx/latest/design/inflight-window-and-message-queue.html
            connOpts.setReceiveMaximum(1);

            //遗言消息
            connOpts.setWill(mWillTopic, new MqttMessage((mClientId + " offline").getBytes(), 1, true, null));

            //订阅回调
            mClient.setCallback(new PushCallback());

            Log.i(TAG, "Connecting to broker: " + mBroker);
            //连接失败会报异常
            mClient.connect(connOpts);
            Log.i(TAG, "Connected");

            //订阅主题
            mClient.subscribe(mTaskTopic, 1);

            new Handler(Looper.getMainLooper()).post(mPublishRunnable);
        } catch (MqttException me) {
            printMqttException(me);
        }
    }

    void DisconnectMqtt() {
        try {
            if (mClient != null) {
                mClient.disconnect();
                Log.i(TAG, "Disconnected");
                mClient.close();
            }
            mClient = null;
        } catch (MqttException me) {
            printMqttException(me);
        }
    }

    void PublishMsg(String topic, String payload) {
        try {
            //发布消息
            String msg = payload;
            if (msg.isEmpty()) {
                State state = new State();
                state.No = mMsgNo++;
                state.state = mClientId + " online";
                msg = state.toJson();
            }
            Log.i(TAG, "Publishing message: " + msg);
            MqttMessage message = new MqttMessage(msg.getBytes(), mQos, true, null);
            mClient.publish(topic.isEmpty() ? mPubTopic : topic, message);
            Log.i(TAG, "Message published");
        } catch (MqttException me) {
            printMqttException(me);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        new Handler(Looper.getMainLooper()).postDelayed(this::ConnectMqtt, 100);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        DisconnectMqtt();
    }

    private String getCurrentTimeWithMillis() {
        // 创建SimpleDateFormat对象，指定日期时间格式，包括毫秒
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());
        // 获取当前时间
        Date date = new Date();
        // 格式化日期时间
        return dateFormat.format(date);
    }

    void printMqttException(MqttException me) {
        Log.e(TAG, "reason " + me.getReasonCode());
        Log.e(TAG, "msg " + me.getMessage());
        Log.e(TAG, "loc " + me.getLocalizedMessage());
        Log.e(TAG, "cause " + me.getCause());
        Log.e(TAG, "excep " + me);
        me.printStackTrace();
    }

    // 模拟状态消息结构
    private class State {
        public int No = 0;
        public String Id = mClientId;
        public String state = "";
        public String other = UUID.randomUUID().toString();
        public String time = getCurrentTimeWithMillis();

        public String toJson() {
            Gson gson = new Gson();
            return gson.toJson(this);
        }
    }

    public class PushCallback implements MqttCallback {

        @Override
        public void disconnected(MqttDisconnectResponse disconnectResponse) {
            Log.w(TAG, "连接断开，可以做重连");
            new Handler(Looper.getMainLooper()).post(() -> ConnectMqtt());
        }

        @Override
        public void mqttErrorOccurred(MqttException me) {
            printMqttException(me);
        }

        public void messageArrived(String topic, MqttMessage message) {
            Log.i(TAG, "接收消息主题:" + topic);
            Log.i(TAG, "接收消息Qos:" + message.getQos());
            Log.i(TAG, "接收消息内容:" + new String(message.getPayload()));
            if (topic.equals(mTaskTopic)) { //模拟执行5s后返回结果
                new Handler(Looper.getMainLooper()).postDelayed(() -> PublishMsg(mResultTopic, getCurrentTimeWithMillis()), 5000);
            }
        }

        @Override
        public void deliveryComplete(IMqttToken token) {
            Log.i(TAG, "deliveryComplete---------" + token.isComplete());
        }

        @Override
        public void connectComplete(boolean reconnect, String serverURI) {
            Log.i(TAG, "connected: " + reconnect + ", " + serverURI);
        }

        @Override
        public void authPacketArrived(int reasonCode, MqttProperties properties) {

        }
    }
}