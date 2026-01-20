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
int main(){
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1; // 定义一个变量，值为 1 (表示开启)
    struct sockaddr_in src;
    socklen_t len = sizeof(src);

    struct sockaddr_in addr;
    struct ip_mreq mreq;
    memset(&addr, 0, sizeof(addr));      // 清零，安全初始化
    addr.sin_family = AF_INET;            // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定本机任意网卡
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    addr.sin_port = htons(1818);         // 端口号

    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
    setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));//join
    char buf[1024];  // 1 KB 的缓冲区

    int n =recvfrom(socketfd, buf, sizeof(buf), 0,(struct sockaddr*)&src, &len);
    if (n > 0) {
    buf[n] = 0; 
    printf("received: %s\n", buf);
    }
}
