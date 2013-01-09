/********************************************************************/
/* Copyright (C) DongZiHao&ChenYouZhi, 2012                         */
/*                                                                  */
/*  FILE NAME             :  dbapi.h                                */
/*  PRINCIPAL AUTHOR      :  DongZiHao&Chenyouzhi                   */
/*  SUBSYSTEM NAME        :  DB                                     */
/*  MODULE NAME           :  dbapi                                  */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/11/29                             */
/*  DESCRIPTION           :  Export Interface for NO SQL API        */
/********************************************************************/

/*
 *  Revision log:
 *
 *  Created by DongZiHao&Chenyouzhi,2012/11/29
 *
 */

#ifndef DBAPI_H
#define DBAPI_H
#include <tchdb.h>

/*  
 * create the object and open the database
 * input        : dbname
 * output       : NONE
 * in/out       : hdb
 * return       : NONE
 */
extern void createDB(void **hdb,char *dbname);

/*  
 * store records
 * input        : key,value
 * output       : NONE
 * in/out       : hdb
 * return       : NONE
 */
extern void setValue(void* hdb,char* key,char* value);

/*  
 * retrieve records
 * input        : key
 * output       : value
 * in/out       : hdb
 * return       : NONE
 */
extern char *getValue(void* hdb,char* key);

/*  
 * close the database
 * input        : NONE
 * output       : NONE
 * in/out       : hdb
 * return       : ecode
 */
extern int closeDB(void* hdb);

/*
 * delete records
 * input        : keybuf
 * output       : NONE
 * in/out       : hdb
 * return       : 0
 */
extern int deleteValue(void* hdb,char* keybuf);
#endif
