//
// Created by tracy on 2018/2/6.
//

#pragma once

#if defined( WIN32 ) || defined( _WIN32 ) || defined( WIN64 ) || defined( _WIN64 )
#if !defined( OS_WINDOWS )
		#define OS_WINDOWS
	#endif
#elif defined( __ANDROID__ )
#if !defined( OS_ANDROID )
#define OS_ANDROID
#endif
#elif defined( __hexagon__ ) || defined( __qdsp6__ )
#if !defined( OS_HEXAGON )
		#define OS_HEXAGON
	#endif
#elif defined( __APPLE__ )
	#if !defined( OS_APPLE )
		#define OS_APPLE
	#endif
	#include <Availability.h>
	#if __IPHONE_OS_VERSION_MAX_ALLOWED && !defined( OS_APPLE_IOS )
		#define OS_APPLE_IOS
	#elif __MAC_OS_X_VERSION_MAX_ALLOWED && !defined( OS_APPLE_MACOS )
		#define OS_APPLE_MACOS
	#endif
#elif defined( __linux__ )
	#if !defined( OS_LINUX )
		#define OS_LINUX
	#endif
#else
	#error "unknown platform"
#endif

#if defined( OS_ANDROID )
#include <jni.h>
#else
struct JNIEnv
{
};
struct JavaVM
{
};
struct _jobject
{
};
typedef _jobject*       jobject;
#endif
