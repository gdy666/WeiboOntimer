/*
 * main.c
 *
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#include "yeini.h"
#include "weibo.h"
#include "sensor.h"
#include "includes.h"

void debug(const char *fmt, ...)
{
	#ifdef DEBUG
		 va_list ap;
		 va_start(ap, fmt);
		 vprintf(fmt, ap);
		 va_end(ap);
	#endif
}

extern char weibo_content_add[];
extern char access_token[];

int main()
{
	init_config();	//初始化配置
	curl_global_init(CURL_GLOBAL_ALL);
	char* weibo_content=(char*)calloc(280,sizeof(char));


	if(0 == getNowLunar(weibo_content)){	//通过getNowLunar把当前时间日期以预先设置的格式将输出到weibo_content
		struct DHT11 dht11_data;

		while(DHT11_readData(&dht11_data)){
			sleep(2);
		}
		char dht11_content[128];
		//%%25=%,防止post丢失问题
		sprintf(dht11_content,"当前室内温度:%d ℃,相对湿度:%d%%25。",dht11_data.temp,dht11_data.rehum);

		strcat(weibo_content,dht11_content);
		strcat(weibo_content, weibo_content_add);	//将在配置文件中读取到的内容追加
		while(new_weibo_post(access_token,weibo_content)){ //如果微博发表不成功则睡眠8秒后重发
			sleep(8);
		}
	}
	curl_global_cleanup();
	return 0;
}


