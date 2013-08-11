/*
 * curlutil.c
 *
 *  Created on: 2013年8月8日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "curlutil.h"

#define MAXBUFFER 40960
static size_t get_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *)stream) + strlen((char *)ptr) > MAXBUFFER) return 0;
	strcat(stream, (char *)ptr);
	return size*nmemb;
}


int post_with_argv(const char* URL,const char* argv,char* result)
{
	CURL *curl = NULL;
	CURLcode ret;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(argv));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	ret = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	return ret;
}

int get_with_argv(const char* URL,const char* argv,char* result)
{
	CURL *curl = NULL;
	CURLcode ret;
	char* data = (char*) calloc(512,sizeof(char));

	strcat(data,URL);
	strcat(data,"?");
	strcat(data,argv);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_URL, data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
	ret = curl_easy_perform(curl);

	free(data);
	curl_easy_cleanup(curl);
	return ret;
}
