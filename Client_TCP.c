#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8090

int main(){
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char add[225];

    printf("Nhap dia chi Server: ");
    fgets(add, 225, stdin);
    add[strlen(add)-1] = 0;
    //tao socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    //Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, add, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/Address not supported \n");
        return -1;
    }
    //connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    for(;;)
    {
        char mess_from_client[225];
        char buffer[1024];
        memset(mess_from_client,0,sizeof(mess_from_client));
        memset(buffer,0,sizeof(buffer));
        printf("Nhap noi dung tin nhan gui den Server: ");
        fgets(mess_from_client, 225, stdin);
        fflush(stdin);
        send(sock , mess_from_client , strlen(mess_from_client) , 0 );
        if (strcmp(mess_from_client,"bye\n")==0)
            break;

        valread = read( sock , buffer, 1024);
        printf("Tin nhan tu Server: ");
        printf("%s\n",buffer );
        fflush(stdin);
        if (strcmp(buffer,"bye\n")==0)
            break;
    }
    close(sock);
    return 0;
}
