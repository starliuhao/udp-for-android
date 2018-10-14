//
// Created by liuhao1 on 2018/6/5.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

class UdpSocket
{
public:
    UdpSocket()  = default;
    ~UdpSocket() = default;

    int  Open( unsigned short port );
    void InitRemoteAddress( char const* ip, unsigned short port );

    void Close();
    int  Send( uint8_t* buffer, size_t bufferLength );

    int Receive( uint8_t* buffer, size_t bufferLength );

private:
    int                handle     = { -1 };
    struct sockaddr_in localAddr;
    struct sockaddr_in remoteAddr;
    struct hostent*    h;
};
