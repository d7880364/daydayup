/********************************************************************/
/* Copyright (C) DongZiHao,ChenYouZhi 2012                          */
/*                                                                  */
/*  FILE NAME             :  dbapi.c                                */
/*  PRINCIPAL AUTHOR      :  DongZiHao,Chenyouzhi                   */
/*  SUBSYSTEM NAME        :  DB                                     */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  LINUX                                  */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  Export Interface for NO SQL API        */
/********************************************************************/

/*
 *  Revision log:
 *
 *  Created by DongZiHao&Chenyouzhi,2012/11/29
 *
 */

#include <tcutil.h>
#include <tchdb.h>
#include "dbapi.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void createDB(void **hdb,char *dbname)
{
    int ecode = -1;

    *hdb = tchdbnew();
 
    //tchdbsetmutex(*hdb); 
      
    if(!tchdbopen(*hdb,dbname,HDBOWRITER|HDBOCREAT|HDBONOLCK))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr,"open error: %s\n",tchdberrmsg(ecode));
    }
}

/* store records */
void setValue(void *hdb,char* key,char* value)
{  
    int ecode = -1;  
    if(!tchdbput2(hdb,key,value))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr,"put error: %s\n",tchdberrmsg(ecode));
    }
}

/* retrieve records */
char *getValue(void *hdb,char* key)
{
    int ecode = -1;

    char *value = tchdbget2(hdb,key);

    if(value)
    {
	return value;
    }
    else
    
    {
        return "";
    }   
}

/* delete records */
int deleteValue(void *hdb,char* keybuf)
{
    int ecode;
    if(!tchdbout(hdb, keybuf, strlen(keybuf)))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr, "delete error: %s\n", tchdberrmsg(ecode));
        return -1;
    }
    return 0;
}

/* close the database */
int closeDB(void *hdb)
{
    int ecode = -1;
    if(!tchdbclose(hdb))
    {
        ecode = tchdbecode(hdb);
        fprintf(stderr,"close error: %s\n",tchdberrmsg(ecode));
    }
    tchdbdel(hdb);
    return ecode;
}
