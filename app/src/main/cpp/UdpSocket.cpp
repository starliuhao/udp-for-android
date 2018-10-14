//
// Created by bugki on 2018/2/4.
//

#include "UdpSocket.hpp"
#include <fcntl.h>
#include "sys_utils.h"
#include <errno.h>
    int UdpSocket::Open( unsigned short port )
    {
        int result = -1;

        handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
        if ( handle < 0 )
        {
            Print("fail = %d",result);
            return result;
        }

        // non-block
        int nonBlocking = 1;
        if ( -1 == fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking))
        {
            Print("fail = %d",result);
            return -1;
        }

        int broadcast = 1;
        result        = setsockopt( handle, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &broadcast, sizeof broadcast );
        if ( result < 0 )
        {
            Print("setsockopt = %d",result);
            Print("fail = %d",result);
            return result;
        }

        /* bind any port */
        localAddr.sin_family      = AF_INET;
        localAddr.sin_addr.s_addr = htonl( INADDR_ANY );
        localAddr.sin_port        = htons( port );

        result = bind( handle, (struct sockaddr*)&localAddr, sizeof( localAddr ) );
        if ( result < 0 )
        {
            Print("fail = %d",result);
        }

        Print("succsss = %d",result);
        return result;
    }

    void UdpSocket::InitRemoteAddress(char const* ip, unsigned short port)
    {
        /* get server IP address (no check if input is IP address or DNS name */
        h = gethostbyname( ip );
        if ( h == NULL )
        {


        }

        remoteAddr.sin_family = h->h_addrtype;
        memcpy( (char*)&remoteAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length );
        remoteAddr.sin_port = htons( port );
    }

    void UdpSocket::Close()
    {
        if ( handle >= 0 )
        {
            close( handle );
            handle = -1;
        }
    }

    int UdpSocket::Send( uint8_t* buffer, size_t bufferLength )
    {
        return sendto( handle, buffer, bufferLength, 0, (struct sockaddr*)&remoteAddr,
                       sizeof( remoteAddr ) );
    }

    int UdpSocket::Receive( uint8_t* buffer, size_t bufferLength )
    {
        socklen_t len = sizeof( remoteAddr );
        int       n = recvfrom( handle, buffer, bufferLength, 0, (struct sockaddr*)&remoteAddr, &len );
        if ( n > 0)
        {
            char *ip = inet_ntoa(remoteAddr.sin_addr);
        }

        return n;
    }



