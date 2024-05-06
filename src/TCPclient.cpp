#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in address;
    int sock=0,valread;
    struct sockaddr_in serv_addr;
    char* hello="hello from the client";
    char buffer[BUFFER_SIZE]={0};

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr,'0',sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);

    //将IP地址从点分十进制转换为二进制
    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
        perror("invalid address");
        exit(EXIT_FAILURE);        
    } 
    /*
    inet_pton:用于将IP地址转换为二进制形式
    AF_INET：表示地址族（Address Family），在这里指定为 IPv4。
    "127.0.0.1"：表示要转换的 IPv4 地址，这里是本地回环地址。
    &serv_addr.sin_addr：是一个指向 struct sockaddr_in 结构体中的 
    sin_addr 成员的指针，用于存储转换后的二进制 IP 地址。
    */
   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection error");
        exit(EXIT_FAILURE);  
    }
    std::cout<<"conneted"<<std::endl;
    if (send(sock, hello, strlen(hello), 0) < 0) {
    std::cerr << "Send failed" << std::endl;
    return -1;
    }

    valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Message from server: " << buffer << std::endl;

    return 0;
}