/*
 * sensor.c
 *
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */
#include "sensor.h"
#define DHT11_ORDER "DHT11"
//#include "includes.h"

extern char serial_port[];
int DHT11_readData(struct DHT11* data)
{
	serial *s;
	int state=0;
	if (serial_open(&s, serial_port, 9600) == 0){
		debug("Port opened.\n");

	} else {
		debug("Problem with port opening\n");
		return -1;
	}

	system("sleep 3");
	char buffer[128];
	serial_write(s,"DHT11");
	system("sleep 3");
	serial_read(s, buffer, '\n', 128);
	debug("receive from serial's data length is :%d\n",strlen(buffer));
	debug("receive from serial's data content is :%s\n",buffer);

	//判定接收到的字符串长度，小于50失败
	if(strlen(buffer)>50){
		sscanf(buffer,"%*s%*s%*s%*s%d%*s%d",&data->temp,&data->rehum);
		debug("temp:%d rehum:%d\n",data->temp,data->rehum);
	}else{
		state=1;
	}

	close(s->fd);
	return state;
}
