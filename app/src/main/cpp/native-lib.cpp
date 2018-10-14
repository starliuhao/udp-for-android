#include <jni.h>
#include <string>
#include <sstream>
#include <sys/epoll.h>
#include <assert.h>
#include "UdpSocket.hpp"
#include "help.hpp"
#include "sys_utils.h"

static UdpSocket g_UdpClient;
static UdpSocket g_UdpServer;

jmethodID g_UdpCallback;
jobject g_UdpInterfaceObj;

static bool g_UdpServerRunning = false;
static bool g_ShutdownUdpClient = false;
static bool g_UdpClientRunning = false;

static const int g_UdpClientBufferSize = 1024;

extern "C" JNIEXPORT jstring

JNICALL
Java_com_coolweather_android_udp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_coolweather_android_udp_MainActivity_UdpClientStart(JNIEnv *env, jobject instance,
                                                             jshort localPort) {
    g_ShutdownUdpClient = false;
    g_UdpClient.Open( localPort );
    char buffer[g_UdpClientBufferSize] = {};
    jbyteArray pSend = env->NewByteArray( g_UdpClientBufferSize );

    while ( !g_ShutdownUdpClient )
    {
        g_UdpClientRunning = true;

        int n = g_UdpClient.Receive(reinterpret_cast<uint8_t *>(buffer), g_UdpClientBufferSize);

        if ( n <= 0 )
        {
            continue;
        }
        else
        {

            env->SetByteArrayRegion( pSend, 0, n, (jbyte*)buffer); // Copy
            assert( g_UdpInterfaceObj != nullptr );
            env->CallVoidMethod( g_UdpInterfaceObj, g_UdpCallback, pSend );

        }
    }

    env->DeleteLocalRef( pSend );
    g_UdpClient.Close();

    g_UdpClientRunning = false;
    // TODO

}extern "C"
JNIEXPORT void JNICALL
Java_com_coolweather_android_udp_MainActivity_UdpClientShutdown(JNIEnv *env, jobject instance) {

    (void) env;
    g_ShutdownUdpClient = true;
    g_UdpClientRunning = false;

    g_UdpClient.Close();


}extern "C"
JNIEXPORT void JNICALL
Java_com_coolweather_android_udp_MainActivity_InitUdpCallback(JNIEnv *env, jobject instance,
                                                              jobject udpCallBackObj) {

    g_UdpInterfaceObj = env->NewGlobalRef( udpCallBackObj );
    jclass udpInterfaceClass = env->GetObjectClass( udpCallBackObj );
    g_UdpCallback = GetMethodID( env, udpInterfaceClass, "onRecvUdpMsg", "([B)V" );

    assert( g_UdpInterfaceObj && g_UdpCallback );

}extern "C"
JNIEXPORT void JNICALL
Java_com_coolweather_android_udp_MainActivity_UdpServerStart(JNIEnv *env, jobject instance,
                                                             jshort localPort, jshort remotePort,
                                                             jstring broadcastContent_,
                                                             jstring broadcastAddr_) {
    if(g_UdpServerRunning){
        return;
    }
    g_UdpServerRunning = true;
    const char *broadcastContent = env->GetStringUTFChars(broadcastContent_, 0);
    const char *broadcastAddr = env->GetStringUTFChars(broadcastAddr_, 0);
    auto buffersize = env->GetStringLength(broadcastContent_);

   int open= g_UdpServer.Open( localPort );
    Print("open g_UdpServer %d"+open);
    assert(open >= 0);
    g_UdpServer.InitRemoteAddress(broadcastAddr, remotePort);
    jint index = 0;
    char *out = new char[20];
    memset(out, 0, 20);
    while ( g_UdpServerRunning )
    {

        int n = g_UdpServer.Send( (uint8_t*)broadcastContent, buffersize );
        Print("open g_UdpServer %d"+n);

        usleep( 500 * 1000 );
        g_UdpServerRunning = false;
    }
    g_UdpServerRunning = false;
    g_UdpServer.Close();
    // TODO
    env->ReleaseStringUTFChars(broadcastContent_, broadcastContent);
    env->ReleaseStringUTFChars(broadcastAddr_, broadcastAddr);
}
//extern "C"
//JNIEXPORT void JNICALL
//Java_com_mapleaf_motouserver_MotouServer_UdpServerStart(JNIEnv *env, jclass type, jshort localPort, jshort remotePort, jstring broadcastContent, jstring broadcastAddr)
//{
//(void) type;
//if ( g_UdpServerRunning )
//{
//Print("--udp is already broadcasting\n");
//return;
//}
//
//g_UdpServerRunning = true;
//
//const char *pBroadcastAddr = env->GetStringUTFChars(broadcastAddr, 0);
//Print("--udp-- start broadcast %s", pBroadcastAddr);
//const char *pContent = env->GetStringUTFChars(broadcastContent, 0);
//auto bufferSize = env->GetStringLength(broadcastContent);
//
//// socket创建失败
//int udp_socket_retry_time = 1;
//if (g_UdpServer.Open( localPort ) < 0)
//{
//Print("--udp-- udp socket create error， error ：%s\n", strerror( errno ) );
//while (udp_socket_retry_time < 4)
//{
//Print("--udp-- udp socket create retry time is %d\n", udp_socket_retry_time);
//if (g_UdpServer.Open( localPort ) < 0){
//udp_socket_retry_time++;
//usleep( 500 * 1000 * udp_socket_retry_time);
//}
//else
//{
//Print("--udp-- udp socket create success \n" );
//break;
//}
//}
//}
//g_UdpServer.InitRemoteAddress(pBroadcastAddr, remotePort);
//int send_fail_time = 0;
//while ( g_UdpServerRunning )
//{
//if (int n = g_UdpServer.Send( (uint8_t*)pContent, bufferSize ) >=0 )
//{
//usleep( 200 * 1000 );
//Print("--udp-- broadcast %d bytes: %s\n", n, pContent);
//}
//else
//{
//send_fail_time = (send_fail_time + 1) % INT32_MAX;
//Print( "--udp broadcast send error, time is : %d ， error ：%s\n", send_fail_time, strerror( errno ) );
//}
//}
//g_UdpServerRunning = false;
//
//g_UdpServer.Close();
//
////
//env->ReleaseStringUTFChars(broadcastContent, pContent);
//env->ReleaseStringUTFChars(broadcastAddr, pBroadcastAddr );
//}
extern "C"
JNIEXPORT void JNICALL
Java_com_coolweather_android_udp_MainActivity_UdpServerShutdown(JNIEnv *env, jobject instance) {

    g_UdpServerRunning = false;

}