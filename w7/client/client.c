/********************************************************************/
/* Copyright (C) DayDayUp,USTC  2013                                */
/*                                                                  */
/*  FILE NAME             :  client.c                               */
/*  PRINCIPAL AUTHOR      :  DayDayUp                               */
/*  SUBSYSTEM NAME        :  DB                                     */
/*  MODULE NAME           :  client                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  LINUX                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  run as client                          */
/********************************************************************/

#include<stdio.h> 
#include<arpa/inet.h>
#include<string.h> 
#include<assert.h> 

#define PORT                5555 
#define IP_ADDR             "127.0.0.1"  
#define MAX_CONNECT_QUEUE   1024 
#define MAX_BUF_LEN         1024 

/* 初始化客户端 */
int initClient(int port,char *ipAddr)
{
    struct sockaddr_in serverAddr; 
    serverAddr.sin_family=AF_INET; 
    serverAddr.sin_port=htons(port); 
    serverAddr.sin_addr.s_addr=inet_addr(ipAddr);

    memset(&serverAddr.sin_zero,0,8);
    int sockfd=socket(PF_INET,SOCK_STREAM,0);
    assert((sockfd!=-1));
    int ret= connect(sockfd,(const struct sockaddr*)&serverAddr,sizeof(struct sockaddr)); 
    if(ret ==-1) 
    { 
         fprintf(stderr,"Connect Error,%s:%d\n",__FILE__,__LINE__); 
         return-1; 
    } 
    
    return sockfd;
}

int main() 
{   
    /* 定义master和服务器端句柄 */
    int sockfdM,sockfdS[MAX_BUF_LEN];

    /* 定义与命令行有关的变量 */
    char buf[MAX_BUF_LEN],cmd[MAX_BUF_LEN],buf1[MAX_BUF_LEN],buf2[MAX_BUF_LEN],ports[MAX_BUF_LEN];
    const char *prompt = "Daydayup";
    char dbname[1024] = "\0";
    char *operator;

    /* 定义变量存储服务器数目和服务器端口号 */
    int serverNum=0;
    int port[MAX_BUF_LEN];

    /* 客户端连接到master获得服务器数目及其端口号 */
    sockfdM=initClient(PORT,IP_ADDR);
    
    char *temp="client";
    send(sockfdM,temp,sizeof(temp),0);
    recv(sockfdM,ports,MAX_BUF_LEN,0);

    temp=strrchr(ports,';');
    temp=temp+1;
    serverNum=atoi(temp);

    temp=strtok(ports,";");
    port[0]=atoi(temp);
    sockfdS[0]=initClient(port[0],IP_ADDR);

    int i;
    for(i=1;i<serverNum;i++)
    {
        temp=strtok(NULL,";");
        port[i]=atoi(temp);
        sockfdS[i]=initClient(port[i],IP_ADDR);
    }

    /* 客户端与服务器端进行通信 */
    while(1)
    {
      
        printf("%s::%s>>",prompt,dbname);
        gets(cmd);
         
        sscanf(cmd,"%[^ ]",buf1);
    	operator=buf1;
    	if(strcmp(operator,"help")==0)
    	{
            printf("open filename - Ex:open daydayup.hdb\n");
            printf("set key value - Ex:set 100 helloworld\n");
            printf("get key       - Ex:get 100\n");
            printf("delete key    - Ex:delete 100\n");
            printf("close         - leave daydayup.hdb\n");
            printf("help          - list cmds info\n");
    	}
        else if(strcmp(operator,"close")==0)
        {
            for(i=0;i<serverNum;i++)
                send(sockfdS[i],cmd,sizeof(cmd),0);
            dbname[0]='\0';
        }
        else if(strcmp(operator,"exit")==0)
        {
            for(i=0;i<serverNum;i++)
                send(sockfdS[i],cmd,sizeof(cmd),0);
            exit(0);
        }
        else if(strcmp(operator,"open")==0)
    	{
            
            if(strlen(dbname)>0)
                printf("please close %s first.\n",dbname);
            else
            {
                for(i=0;i<serverNum;i++)
                    send(sockfdS[i],cmd,sizeof(cmd),0);
	        sscanf(cmd,"open %s",dbname);
            }
        }
        else if(strcmp(operator,"get")==0)
        {
            sscanf(cmd,"get %s",buf2);
            int j=atoi(buf2)%serverNum;
	    printf("get %s\n",buf2);
            if(j==0)
            {  
                send(sockfdS[0],cmd,sizeof(cmd),0);
                recv(sockfdS[0],buf,MAX_BUF_LEN,0);
            }
            else if(j==1)
            {   
                send(sockfdS[1],cmd,sizeof(cmd),0);
                recv(sockfdS[1],buf,MAX_BUF_LEN,0);
            }
            else
            {
                send(sockfdS[2],cmd,sizeof(cmd),0);
                recv(sockfdS[2],buf,MAX_BUF_LEN,0);
            }
            printf("%s->%s\n",buf2,buf);
        }
        else if(strcmp(operator,"set")==0)
        {
            sscanf(cmd,"set %s",buf);
            int j=atoi(buf)%serverNum;
	    printf("set %s\n",buf);
            if(j==0)
                send(sockfdS[0],cmd,sizeof(cmd),0);
            else if(j==1)
                send(sockfdS[1],cmd,sizeof(cmd),0);
            else
                send(sockfdS[2],cmd,sizeof(cmd),0);
            printf("set success!!!\n");
        }
        else if(strcmp(operator,"delete")==0)
        {
            sscanf (cmd,"delete %s",buf);
            int j=atoi(buf)%serverNum;
            if(j==0)
                send(sockfdS[0],cmd,sizeof(cmd),0);
            else if(j==1)
                send(sockfdS[1],cmd,sizeof(cmd),0);
            else
                send(sockfdS[2],cmd,sizeof(cmd),0);
            printf("delete success!!!\n");
        }
        else
        {
            printf("Unknown Command!!!\n");
        }

    }
 
    /* 关闭句柄 */
    close(sockfdM);
    for(i=0;i<serverNum;i++)
        close(sockfdS[i]);

    return 0; 
} 


