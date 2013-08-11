/*
 * includes.h
 *
 *  Created on: 2013年8月11日
 *      Author: yeso
 *  		QQ:	272288813
 *		e-mail: cheng@yeso.me
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

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

#endif /* INCLUDES_H_ */
