#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#if defined(Q_OS_WIN)
#include <windows.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CONCURRENT_MAX 10

ServerThread::ServerThread(int server_fd)
{
    serverSocket = server_fd;
    (void) memset(clientSocket, 0, sizeof(clientSocket));
}

ServerThread::~ServerThread()
{

}

void ServerThread::run()
{
    char recv_msg[BUFFER_SIZE];
    //fd_set
    fd_set server_fd_set;
    int max_fd = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    while (1) {
        FD_ZERO(&server_fd_set);
        //服务器端socket
        FD_SET(serverSocket, &server_fd_set);
#if defined(Q_OS_WIN)
        max_fd = 1;
#else
        if (max_fd < serverSocket) {
            max_fd = serverSocket;
        }
#endif
        //客户端连接
        for (int i = 0; i < CONCURRENT_MAX; i++) {
            if (clientSocket[i]!=0) {
                FD_SET(clientSocket[i], &server_fd_set);
#if defined(Q_OS_WIN)
                max_fd ++;
#else
                if (max_fd < clientSocket[i]) {
                    max_fd = clientSocket[i];
                }
#endif
            }
        }
        int ret = select(max_fd+1, &server_fd_set, NULL, NULL, &tv);
        tv.tv_sec = 20;
        if (ret < 0) {
            emit addLog("ERROR: select error!\n");
            continue;
        }else if(ret == 0){
            emit addLog("NOTE: select timeout!\n");
            continue;
        }else{
            if (FD_ISSET(serverSocket, &server_fd_set)) {
                struct sockaddr_in client_address;
#if defined(Q_OS_WIN)
                int address_len;
#else
                socklen_t address_len;
#endif
                int client_socket_fd = accept(serverSocket, (struct sockaddr *)&client_address, &address_len);
                if (client_socket_fd > 0) {
                    int index = -1;
                    for (int i = 0; i < CONCURRENT_MAX; i++) {
                        if (clientSocket[i] == 0) {
                            index = i;
                            clientSocket[i] = client_socket_fd;
                            break;
                        }
                    }
                    if (index >= 0) {
                        char logMsg[128] = {0};
                        sprintf(logMsg, "Note: New client(%d) connect successfully, %s:%d \n",index,inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));
                        emit addLog(logMsg);
                    }else{
                        char logMsg[128] = {0};
                        sprintf(logMsg, "ERROR: The server has reached the max client numbers, connect failed, %s:%d \n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));
                        emit addLog(logMsg);
                    }
                }
            }

            for (int i = 0; i <CONCURRENT_MAX; i++) {
                if (clientSocket[i]!=0) {
                    if (FD_ISSET(clientSocket[i], &server_fd_set)) {
                        (void) memset(recv_msg, 0, BUFFER_SIZE);
                        long byte_num = recv(clientSocket[i],recv_msg,BUFFER_SIZE,0);
                        if (byte_num > 0) {
                            if (byte_num > BUFFER_SIZE) {
                                byte_num = BUFFER_SIZE;
                            }
                            recv_msg[byte_num] = '\0';
                            printf("客户端(%d):%s\n",i,recv_msg);
                            QString retMsg(recv_msg);
                            emit sendStatus(retMsg);
                        }else if(byte_num < 0){
                            emit addLog("ERROR: Error returned from client!\n");
                        }else{
                            FD_CLR(clientSocket[i], &server_fd_set);
                            clientSocket[i] = 0;
                            emit addLog("ERROR: Client quit!\n");
                        }
                    }
                }
            }
        }
    }
}

void ServerThread::sendAction(char *action)
{
    char input_msg[1024];
    (void) memset(input_msg, '\0', sizeof (input_msg));
    sprintf(input_msg, "%s\n", action);
    for (int i=0; i<CONCURRENT_MAX; i++) {
        if (clientSocket[i]!=0) {
            printf("Send status cmd to %d, %s\n", i, input_msg);
            send(clientSocket[i], input_msg, BUFFER_SIZE, 0);
        }
    }
}

