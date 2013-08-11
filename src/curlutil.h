/*
 * curlutil.h
 *
 *  Created on: 2013年8月8日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef CURLUTIL_H_
#define CURLUTIL_H_

#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * URL:地址 ，argv：参数列表 ,result 返回结果
 */
int post_with_argv(const char* URL,const char* argv,char* result);

int get_with_argv(const char* URL,const char* argv,char* result);

#endif /* CURLUTIL_H_ */
