/********************************************************************/
/* Copyright (C) DayDayUp,USTC  2013                                */
/*                                                                  */
/*  FILE NAME             :  server.c                               */
/*  PRINCIPAL AUTHOR      :  DayDayUp                               */
/*  SUBSYSTEM NAME        :  DB                                     */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  LINUX                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  run as server                          */
/********************************************************************/

#include "dbapi.h"
#include <stdio.h> 
#include <arpa/inet.h> 
#include <string.h>  
#include <stdlib.h>
#include <tchdb.h>
#include <pthread.h>

#define PORT               5555 
#define IP_ADDR            "127.0.0.1" 
#define MAX_CONNECT_QUEUE  1024 
#define MAX_BUF_LEN        1024 

int initServer(int port,char *ipAddr);

int main(int argc,char **argv)
{
    /* 服务器启动时通知master */
    struct sockaddr_in masterAddr; 
    masterAddr.sin_family=AF_INET; 
    masterAddr.sin_port=htons(PORT); 
    masterAddr.sin_addr.s_addr=inet_addr(IP_ADDR);

    memset(&masterAddr.sin_zero,0,8);
    int sockfd0=socket(PF_INET,SOCK_STREAM,0);

    connect(sockfd0,(const struct sockaddr*)&masterAddr,sizeof(struct sockaddr));

    send(sockfd0,argv[1],sizeof(argv[1]),0);

    /* 服务器与客户端通信 */
    int port;
    char *ipAddr;
    
    if(argc==3)
    {
        port=atoi(argv[1]);
        ipAddr=argv[2];
    }
    else 
    {
        printf("参数初始化错误\n");
    }
    int sockfd1=initServer(port,ipAddr);
    
    struct sockaddr_in clientAddr; 
    int clientAddr_len=sizeof(struct sockaddr_in);  
    
    char buf[MAX_BUF_LEN],cmd[MAX_BUF_LEN],buf0[MAX_BUF_LEN],buf1[MAX_BUF_LEN];
    char dbname[1024] = "\0";
    char *operator,*key,*value;
    int lena, lenb;

    void *hdb;

    int clientfd=accept(sockfd1,(struct sockaddr*)&clientAddr,&clientAddr_len);

    while(1) 
    { 
        recv(clientfd,cmd,MAX_BUF_LEN,0);

        sscanf(cmd,"%[^ ]",buf);
    	operator=buf;
        
        if(strcmp(operator,"close")==0)
    	{
            if(hdb)
	    {
                closeDB(hdb);
	    }
	    dbname[0]='\0';
            printf("receive\"close\"\n");
    	}
        else if(strcmp(operator,"exit")==0)
    	{
            if(hdb)
	    {
	        closeDB(hdb);
	    }
            printf("receive\"exit\"\n");
            exit(0);
        }
        else if(strcmp(operator,"open")==0)
    	{
            if(strlen(dbname)==0)
	    {
                sscanf(cmd,"open %s",dbname);	
		createDB(&hdb,dbname);
	    }
            printf("receive\"open\"\n");
     	}
        else if(strcmp(operator,"set")==0)
     	{
	    sscanf ( cmd, "set %s", buf );
	    lena = strlen ( cmd );
  	    lenb = strlen ( buf ) + 5;
	    memmove( buf1, cmd + lenb, lena - lenb );
            setValue(hdb,key=buf,value=buf1);
            printf("receive\"set\"\n");
     	}
        else if(strcmp(operator,"get")==0)
     	{
            sscanf(cmd,"get %s",buf);
            strcpy(buf0,getValue(hdb,key=buf));
            send(clientfd,buf0,sizeof(buf0),0);
            printf("receive\"get\"\n");
     	}
        else if(strcmp(operator,"delete")==0)
     	{
            sscanf(cmd,"delete %s",buf);
            deleteValue(hdb,buf);
            printf("receive\"delete\"\n");
        }
        else 
        {
            printf("receive\"Unknown Command\"\n");
        }
       
    } 
    close(clientfd);  
    close(sockfd0);
    close(sockfd1);
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
