/*
 * yeeini.c
 *
 *  Created on: 2013年7月13日
 *      Author: Yeso
 */
#include "yeini.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int find_key(const char *buf, const char *section, const char *key,
	int *sectionStart, int *valueStart, unsigned long *valueSize);
static void get_line(const char *buf, int inStart, int *lineStart, int *lineEnd);
static void getPath();
#ifndef strnicmp
#define strnicmp strncasecmp
#endif
#define NOT_COMMENT(c)	(c!=';' && c!='#')	/* 不是注释行 */

static char* CFG_NAME="ye.conf";
char CFG_FILE[64]="";

char access_token[33]="";	//新浪微博Token
char weibo_content_add[160]="";	//微博内容
char serial_port[16]="";				//串口地址
/*

*/
static void getPath()
{
	int cnt = readlink("/proc/self/exe", CFG_FILE, sizeof(CFG_FILE));
	if (cnt < 0 || cnt >= sizeof(CFG_FILE))
	{
		printf("***Error***\n");
		exit(-1);
	}
	//获取当前目录绝对路径，即去掉程序名
	int i;
	for (i = cnt; i >=0; --i)
	{
		if (CFG_FILE[i] == '/')
		{
			CFG_FILE[i+1] = '\0';
			break;
		}
	}
	strcat(CFG_FILE,CFG_NAME);
	return ;
}

void init_config(){

	getPath();
	char* buf=load_file(CFG_FILE);
	//device_id=get_int(buf,"Yeelight","DEVICE_ID",0);
	//switch_id=get_int(buf,"Yeelight","SWITCH_ID",0);
	//get_string(buf,"Yeelight","YEEBOX_IP","",yeebox_ip,sizeof(yeebox_ip));
	//yeebox_port=get_int(buf,"Yeelight","YEEBOX_PORT",0);
	//yeelight=get_int(buf,"Yeelight","YEELIGHT",0);
	//get_string(buf,"Yeelight","YEELINK_APIKEY","",yeelink_apikey,sizeof(yeelink_apikey));
	get_string(buf,"Ye_Config","AccessToken","",access_token,sizeof(access_token));
	get_string(buf,"Ye_Config","WeiboContentAdd","",weibo_content_add,sizeof(weibo_content_add));
	get_string(buf,"Ye_Config","Serial","/dev/tty0",serial_port,sizeof(serial_port));
	free(buf);
}

char *load_file(const char *fileName)
{
	FILE *fp = NULL;
	long size = 0;
	char *buf = NULL;
	if ((fp=fopen(fileName, "rb")) == NULL){//如果文件不存在或打开错误
		perror("请确认配置文件路径");
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	buf = (char *)malloc(size+1);
	buf[size] = '\0';
	if (fread(buf, size, 1, fp) < 1)
	{
		free(buf);
		buf = NULL;
	}
	fclose(fp);
	return buf;
}

int get_string(const char *buf, const char *section, const char *key,
	const char *defaultValue, char *value, unsigned long size)
{
	int sectionStart, valueStart;
	unsigned long valueSize;

	if (find_key(buf, section, key, &sectionStart, &valueStart, &valueSize)!=0 || valueSize==0)	/* 未找到？ */
	{
		strncpy(value, defaultValue, size);
		return -1;
	}
	if (size-1 < valueSize)		/* 找到但太长？ */
		valueSize = size - 1;
	memset(value, 0, size);
	strncpy(value, buf+valueStart, valueSize);
	return 0;
}

int get_int(const char *buf, const char *section, const char *key, int defaultValue)
{
	char value[21] = {0};
	get_string(buf, section, key, "", value, sizeof(value));
	if (value[0] == '\0')	/* 找不到或找到但为空？ */
		return defaultValue;
	return atoi(value);
}

static void get_line(const char *buf, int inStart, int *lineStart, int *lineEnd)
{
	int start, end;
	for (start=inStart; buf[start]==' ' || buf[start]=='\t' || buf[start]=='\r' || buf[start]=='\n'; start++);
	for (end=start; buf[end]!='\r' && buf[end]!='\n' && buf[end]!='\0'; end++);
	*lineStart = start;
	*lineEnd = end;
}

static int find_key(const char *buf, const char *section, const char *key,
	int *sectionStart, int *valueStart, unsigned long *valueSize)
{
	int lineStart, lineEnd, i;
	for (*sectionStart=-1, lineEnd=0; buf[lineEnd]!='\0'; )
	{
		get_line(buf, lineEnd, &lineStart, &lineEnd);
		if (buf[lineStart] == '[')
		{
			for (i=++lineStart; i<lineEnd && buf[i]!=']'; i++);
			if (i<lineEnd && strnicmp(buf+lineStart, section, i-lineStart)==0)	/* 找到Section？ */
			{
				*sectionStart = lineStart-1;
				if (key == NULL)
					return -1;
			}
			else if (*sectionStart != -1)	/* 找到Section但未找到Key */
				return -1;
		}
		else if (*sectionStart!=-1 && NOT_COMMENT(buf[lineStart]))	/* 找到Section且该行不是注释 */
		{
			for (i=lineStart+1; i<lineEnd && buf[i]!='='; i++);
			if (i<lineEnd && strnicmp(buf+lineStart, key, i-lineStart)==0)	/* 找到Key？ */
			{
				*valueStart = i + 1;
				*valueSize = lineEnd - *valueStart;
				return 0;
			}
		}
	}
	return -1;
}
