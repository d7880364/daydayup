/********************************************************************/
/* Copyright (C) DayDayUp,USTC  2013                                */
/*                                                                  */
/*  FILE NAME             :  master.c                               */
/*  PRINCIPAL AUTHOR      :  DayDayUp                               */
/*  SUBSYSTEM NAME        :  Server,Client                          */
/*  MODULE NAME           :  master                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  LINUX                                  */
/*  DATE OF FIRST RELEASE :  2013/1/9                               */
/*  DESCRIPTION           :  Impliment dynamic server               */
/********************************************************************/

/*
 *  Revision log:
 *
 *  Created by DayDayUp,2013/1/9
 *
 */

#include <stdio.h> 
#include <arpa/inet.h> 
#include <string.h>  
#include <stdlib.h>
#include <pthread.h>

#define PORT               5555 
#define IP_ADDR            "127.0.0.1" 
#define MAX_CONNECT_QUEUE  1024 
#define MAX_BUF_LEN        1024 

int sockfd;
int clientfd;
int serverCount=0;
char port[MAX_BUF_LEN]="\0"; 

struct sockaddr_in clientAddr; 
int clientAddr_len=sizeof(struct sockaddr_in);

int initServer(int port,char *ipAddr);
void handle();

int main(int argc,char **argv)
{
    sockfd=initServer(PORT,IP_ADDR); 
 
    while(1)
    {
        sleep(1);
        pthread_t pid;
        pthread_create(&pid,NULL,(void *)handle,NULL);
    }
    
    close(clientfd);  
    close(sockfd);
    return 0; 
}

/* 初始化服务器 */
int initServer(int port,char *ipAddr)
{
    int sockfd=-1;

    struct sockaddr_in serverAddr; 
    serverAddr.sin_family=AF_INET; 
    serverAddr.sin_port=htons(port); 
    serverAddr.sin_addr.s_addr=inet_addr(ipAddr);

    memset(&serverAddr.sin_zero,0,8);
 
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)
    {
        fprintf(stderr,"Create Socket Handler Error,%s:%d\n",__FILE__,__LINE__);
        return -1;
    } 
 
    if(bind(sockfd,(const struct sockaddr*)&serverAddr,sizeof(struct sockaddr))==-1)
    { 
        fprintf(stderr,"Bind Error,%s:%d,",__FILE__,__LINE__); 
        fprintf(stderr,"%s:%d\n",(char*)inet_ntoa(serverAddr.sin_addr),ntohs(serverAddr.sin_port)); 
        close(sockfd); 
        return-1; 
    }
 
    if(listen(sockfd,MAX_CONNECT_QUEUE)!=0)
    {
        fprintf(stderr,"Listen Error,%s:%d\n",__FILE__,__LINE__);
        return -1;
    }

    return sockfd; 
}

/* 修改serverCount和port */
void handle()
{
    clientfd=accept(sockfd,(struct sockaddr*)&clientAddr,&clientAddr_len);

    char temp[MAX_BUF_LEN];
    recv(clientfd,temp,MAX_BUF_LEN,0);

    if(strcmp(temp,"client")==0)
    {
        sprintf(temp,"%d",serverCount);
        strcat(port,temp);
        send(clientfd,port,sizeof(port),0);
        printf("client start!!\n");
    }
    else
    {
        serverCount++;
        strcat(port,temp);
        strcat(port,";");
        printf("%d server start!!\n",serverCount);
    }
}
