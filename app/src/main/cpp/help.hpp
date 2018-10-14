//
// Created by liuhao1 on 2018/6/5.
//

#ifndef UDP_HELP_HPP
#define UDP_HELP_HPP

#include <jni.h>
#include <assert.h>

#if defined( __ANDROID__ )
static jmethodID GetMethodID( JNIEnv* env, jclass cls, const char * name, const char * signature )
{
    jmethodID mid = env->GetMethodID( cls, name, signature );
    assert( mid );
    return mid;
}
#endif
#endif //UDP_HELP_HPP
