/*
 * weibo.c
 *
 *  Created on: 2013年8月6日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "weibo.h"
#include "curlutil.h"

#ifdef DEBUG
	#include <stdarg.h>
	void debug(const char *fmt, ...)
	 {
			 va_list ap;
			 va_start(ap, fmt);
			 vprintf(fmt, ap);
			 va_end(ap);
	 }
 #else
	void debug(const char *fmt, ...)
	 {
	 }
#endif

#define WEIBO_POST_BUFFER_LENGTH 280
#define MAXBUFFER 40960
#define ARGVBUFFER 256
extern char access_token[];

long long since_id=0;	//

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *)stream) + strlen((char *)ptr) > 20480) return 0;
	strcat(stream, (char *)ptr);
	return size*nmemb;
}


int new_weibo_post(const char* content)
{
	int ret;
	int state = SUCCESS;	//标记是否发微博成功
	char* argv = (char*)calloc(ARGVBUFFER,sizeof(char));
	char* result = (char*)calloc(MAXBUFFER,sizeof(char));

	sprintf(argv, "access_token=%s&visible=0&status=%s", access_token, content);
	ret = post_with_argv(WEIBO_POST_URL, argv, result);
	debug("ret:%d \n result:\n%s\n",ret,result);
	if(NULL==strstr(result,"created_at")){	//微博发表不成功
		state=FAULT;
	}

	free(argv);
	free(result);
	return state;
}

int new_weibo_post_upload(const char* content,const char* picture_file_name)
{
	CURL *curl = NULL;
	CURLcode ret;
	int state=SUCCESS;	//标记是否发微博成功
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	char* result=(char*)calloc(2048,sizeof(char));

	curl_formadd(&post, &last, CURLFORM_PTRNAME, "access_token", CURLFORM_PTRCONTENTS, access_token, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_PTRNAME, "status", CURLFORM_PTRCONTENTS, content, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_PTRNAME, "pic", CURLFORM_FILE, picture_file_name, CURLFORM_END);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, WEIBO_UPLOAD_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	ret = curl_easy_perform(curl);

	printf("ret:%d\nresult:\n%s\n",ret,result);
		if(NULL==strstr(result,"mention_status")){	//微博发表不成功
			state=FAULT;
		}
	curl_formfree(post);
	curl_easy_cleanup(curl);

	free(result);
	return state;
}

int get_newest_at_user(char* result,int count)
{
	int ret;
	char* argv =(char*) calloc(128,sizeof(char));
	int state=SUCCESS;
	sprintf(argv,"access_token=%s&&filter_by_type=1&filter_by_author=1&count=%d",access_token,count);
	ret=get_with_argv(WEIBO_GET_NEWEST_URL,argv,result);

	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"statuses")){	//获取出错?
		state=FAULT;
	}

	free(argv);
	return state;
}

int get_unread_count(char* result)
{
	int ret;
	char* argv =(char*) calloc(128,sizeof(char));
	int state=SUCCESS;

	sprintf(argv,"access_token=%s",access_token);
	ret=get_with_argv(WEIBO_GET_UNREAD_URL,argv,result);
	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"statuses")){	//获取出错?
		state=FAULT;
	}
	free(argv);
	return state;
}

int new_comments_post(const char* comments,int64 id)
{
	int ret;
	char* argv =(char*) calloc(256,sizeof(char));
	char* result = (char*)calloc(MAXBUFFER,sizeof(char));
	int state=SUCCESS;

	sprintf(argv, "access_token=%s&id=%lld&comment=%s", access_token, id,comments);
	ret=post_with_argv(WEIBO_POST_COMMENT_URL,argv,result);
	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"created_at")){	//获取出错?
		state=FAULT;
	}
	free(argv);
	free(result);
	return state;
}

int repost_weibo(const char* comments,int64 id,int iscomment)
{
	int ret;
	char* argv =(char*) calloc(256,sizeof(char));
	char* result = (char*)calloc(MAXBUFFER,sizeof(char));
	int state=SUCCESS;

	sprintf(argv, "access_token=%s&id=%lld&status=%s&is_comment=%d", access_token, id, comments, iscomment);
	ret=post_with_argv(WEIBO_REPOST_URL,argv,result);
	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"created_at")){	//获取出错?
		state=FAULT;
	}
	free(argv);
	free(result);
	return state;
}

int remind_reset(const char* type)
{
	int ret;
	char* argv =(char*) calloc(256,sizeof(char));
	char* result = (char*)calloc(MAXBUFFER,sizeof(char));
	int state=SUCCESS;

	sprintf(argv, "access_token=%s&type=%s", access_token,type);
	ret=post_with_argv(WEIBO_REMIND_RESET_URL,argv,result);
	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"created_at")){	//获取出错?
		state=FAULT;
	}
	free(argv);
	free(result);
	return state;
}



int get_token(const char* appid,const char* appsecret,const char* username,const char* password)
{
	int ret;
	char* argv =(char*) calloc(256,sizeof(char));
	char* result = (char*)calloc(MAXBUFFER,sizeof(char));
	int state=SUCCESS;

	sprintf(argv, "client_id=%s&client_secret=%s&grant_type=password&username=%s&password=%s",
			appid,appsecret,username,password);
	ret=post_with_argv(WEIBO_ACCESS_TOKEN_URL,argv,result);
	debug("ret:%d\nresult:\n%s\n",ret,result);
	if(NULL==strstr(result,"created_at")){	//获取出错?
		state=FAULT;
	}
	free(argv);
	free(result);
	return state;
}
