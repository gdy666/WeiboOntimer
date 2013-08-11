#ifndef YEEINI_H_
#define YEEINI_H_
char *load_file(const char *fileName);	//读取文件
int get_string(const char *buf, const char *section, const char *key,
	const char *defaultValue, char *value, unsigned long size);	/* 读取字符串 */
int get_int(const char *buf, const char *section, const char *key, int defaultValue);	/* 读取整数 */

void init_config();	//初始化配置参数

#endif
