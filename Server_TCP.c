#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#define PORT 8090

int main(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    //tao socket
    //tao file mo ta soket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //gan dia chi cho socket
    //gan port 8090
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );  //gan cong la 8090
    //bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //listen, chi dinh socket lang nghe ket noi
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //accept, chap nhan ket noi
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    for(;;)
    {
        char buffer[1024];
	    memset(buffer,0,sizeof(buffer));
	    char str_cli_ip[INET_ADDRSTRLEN];
	    struct sockaddr_in* ip_client = (struct sockaddr_in*)&address;
	    inet_ntop(AF_INET, &ip_client->sin_addr, str_cli_ip, INET_ADDRSTRLEN);

	    printf("Client's IP: %s\n", str_cli_ip );
	    char str_cli_port[INET_ADDRSTRLEN];
	    printf("Port: %d\n", ntohs(ip_client->sin_port));
	    printf("Tin nhan tu Client: ");
	    //read, doc du lieu gan vao bien valread tra ve so byte ma no doc duoc
	    valread = read( new_socket, buffer, 1024);
	    printf("%s\n",buffer );

        if (strcmp(buffer, "bye\n") == 0)
            break;

	    //Server tra loi
	    char mess_from_server[225];
        memset(mess_from_server,0,sizeof(mess_from_server));
        printf("Nhap noi dung tin nhan gui den Client: ");
        fgets(mess_from_server, 225, stdin);
        fflush(stdin);
	    send(new_socket, mess_from_server, strlen(mess_from_server), 0 );
        if (strcmp(mess_from_server, "bye\n") == 0)
            break;
    }
    close(new_socket);
    return 0;
}
