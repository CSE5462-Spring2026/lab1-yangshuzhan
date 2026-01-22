/*创建 UDP socket

加入一个 multicast 地址（多播组）

接收字符串消息

把消息里的 key-value 拆出来并格式化打印*/
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
void format_message(char *buf) {
    int in_quote = 0;

    for (char *p = buf; *p; p++) {
        if (*p == '"') {
            in_quote = !in_quote;
        } else if (*p == ' ' && !in_quote) {
            *p = '\n';
        }
    }
}

int main(){
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1; // 1 means on
    struct sockaddr_in src;
    socklen_t len = sizeof(src);

    struct sockaddr_in addr;
    struct ip_mreq mreq;
    memset(&addr, 0, sizeof(addr));      // initialize
    addr.sin_family = AF_INET;            // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // address

    char mcast_ip[32];
    int port;

    printf("Enter multicast IP port: ");
    scanf("%31s%d", mcast_ip,&port);
    mreq.imr_multiaddr.s_addr = inet_addr(mcast_ip); //multicast address 
    addr.sin_port = htons(port);         //port number

    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
    setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));//join
    char buf[1024];  // 1 KB cache

    while (1) {
    int n = recvfrom(socketfd, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr*)&src, &len);
    if (n < 0) {
        perror("recvfrom");
        break;
    }

    buf[n] = 0; 
    format_message(buf);
    printf("*****************\n %s\n", buf);
}
}

