//
// Created by bugki on 2017/4/18.
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "platform.h"
#if defined (OS_ANDROID)
#include <android/log.h>
#elif defined (OS_APPLE)
#include <stdarg.h>
#elif defined( OS_WINDOWS )
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined( OS_LINUX )
#include <stdarg.h>
#endif

/*
================================================================================================================================

System level functionality

================================================================================================================================
*/

struct LogToFile
{
    LogToFile()
    {
        logFile = fopen( "/storage/udisk0/motou_tcp.txt", "w");
        if ( logFile == nullptr )
        {
            logFile = fopen("/sdcard/motou_tcp.txt", "w");
        }
    }

    ~LogToFile()
    {
        fclose( logFile );
    }

    void Write( char const* msg )
    {
        fprintf( logFile, "%s\n", msg );
        fflush( logFile );
    }

    FILE* logFile = { nullptr };
};

//static LogToFile logToFile;


static void Print( const char * format, ... )
{
#if defined( OS_WINDOWS )
    char buffer[4096];
	va_list args;
	va_start( args, format );
	vsnprintf_s( buffer, 4096, _TRUNCATE, format, args );
	va_end( args );

	OutputDebugStringA( buffer );
#elif defined( OS_LINUX )
    va_list args;
	va_start( args, format );
	vprintf( format, args );
	va_end( args );
	fflush( stdout );
#elif defined( OS_APPLE )
    char buffer[4096];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, 4096, format, args );
	va_end( args );

	printf( "%s\n", buffer );
#elif defined( OS_ANDROID )
    va_list args;
    va_start( args, format );
    __android_log_vprint( ANDROID_LOG_VERBOSE, "mt_native", format, args );
    va_end( args );
#endif
}
