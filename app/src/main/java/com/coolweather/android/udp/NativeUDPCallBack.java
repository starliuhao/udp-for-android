package com.coolweather.android.udp;

public interface NativeUDPCallBack {

    /*收到的UDP广播消息监听*/
    public void onRecvUdpMsg(byte[] datas);
}
