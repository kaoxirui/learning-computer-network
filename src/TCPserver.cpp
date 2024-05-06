#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd,new_socket;
    struct sockaddr_in address;
    int opt=1;
    int addrlen=sizeof(address);
    char buffer[BUFFER_SIZE]={0};
    const char *hello="hello from server";

    //创建socket文件描述符
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0){//第三个参数0，表示根据地址族和套接字类型自动选择适当的协议
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //绑定socket到指定的端口
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt))){
        perror("socketopt");
        exit(EXIT_FAILURE);
    }
        if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))){
        perror("socketopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    /*
        设置套接字地址结构sockaddr_in
        用于设置套接字的地址信息，包括地址族，IP地址和端口号
        address.sin_family=AF_INET;表示使用IP4地址族，AF_INET是一个常量，表示IPV4
        address.sin_addr.s_addr=INADDR_ANY;表示接受来自任意IP地址的连接
        address.sin_port=htons(PORT);htons函数用于将主机字节序转换为网络字节序（大端字节序）
    */
    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,3)<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if(new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen)<0){
        perror("accept");
        exit(EXIT_FAILURE);
        /*(struct sockaddr*)&address将指向struct sockaddr_in类型的指针&address转向指为
        struct sockaddr类型的指针
        在网络编程中，struct sockaddr 是一个通用的套接字地址结构，用于表示网络地址信息。
        而 struct sockaddr_in 是基于 IPv4 的具体套接字地址结构。
        */
    }
    std::cout<<"connection accepted"<<std::endl;
    //接受客户端消息
    if(read(new_socket,buffer,BUFFER_SIZE)<0){
        perror("read");
        exit(EXIT_FAILURE);
    }
    std::cout<<"Message from client:"<<buffer<<std::endl;
    //向客户端发送消息
    if(send(new_socket,hello,strlen(hello),0)<0){
        perror("send");
        exit(EXIT_FAILURE);
    }
    return 0;
}