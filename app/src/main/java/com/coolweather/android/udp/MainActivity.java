package com.coolweather.android.udp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "Main";

    public static final short UDP_PORT = 1500;
    public static final short UDP_SERVER_PORT = 10000;
    public static final short UDP_SERVER_REMOTE_PORT = 10888;
    private Button startButton;
    private Button endButton;

    private long oldtime = 0;
    private long outtimesum = 0;
    private long sum = 1;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        startButton = (Button) findViewById(R.id.start);
        endButton = (Button) findViewById(R.id.end);
//        InitUdpCallback(new NativeUDPCallBack() {
//            @Override
//            public void onRecvUdpMsg(byte[] datas) {
//
//                long deltTime = System.currentTimeMillis() - oldtime;
//                String date = (new String(datas)).trim();
//
//                    Log.d(TAG, "Data: " + date + " Deltime : " + deltTime);
//                    oldtime = System.currentTimeMillis();
//
//
////                if (date.indexOf("89")!=-1) {
////
////                    if (deltTime > 4000) {
////                        outtimesum++;
////                        Log.d(TAG, "Out of 4s." + "The per of out time:" + outtimesum / sum);
////                    }
////                    sum++;
////                    Log.d(TAG, "onRecvUdpMsg: " +date );
//
////                }
//
//            }
//        });

//       Thread a =  new Thread(new Runnable() {
//            @Override
//            public void run() {
//                UdpClientStart(UDP_PORT);
//                Log.d(TAG, "onRecvUdpMsg: Start");
//
//            }
//        });
       while (true)
       {

           UdpServerStart(UDP_PORT,UDP_PORT,"liuhao","255.255.255.255");

           Log.d(TAG, "onCreate: ");


       }
//        new  Thread(new Runnable() {
//            @Override
//            public void run() {
//                UdpServerStart(UDP_SERVER_PORT,UDP_PORT,"From liuhao","255.255.255.255");
//            }
//        }).start();


//        startButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//            }
//        });
//
//        endButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                UdpClientShutdown();
//            }
//        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void UdpClientStart(short localPort);

    public native void UdpClientShutdown();

    public native void InitUdpCallback(NativeUDPCallBack udpCallBackObj);

    public native void UdpServerStart(short localPort, short remotePort, String broadcastContent, String broadcastAddr);

    public native void UdpServerShutdown();
}
