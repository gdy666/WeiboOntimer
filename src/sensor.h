/*
 * sensor.h
 *	传感器读写操作
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef SENSOR_H_
#include "serialib.h"

//DHT11温湿度传感器的数据
struct DHT11{
	int temp;	//温度
	int rehum;	//相对湿度
};

//读取DHT11温湿度传感器的数据
int DHT11_readData(struct DHT11* data);

#define SENSOR_H_



#endif /* SENSOR_H_ */
