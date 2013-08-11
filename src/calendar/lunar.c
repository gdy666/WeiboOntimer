#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fdatefunc.h"

int getNowLunar(char* strDate)
{
	SunDate sunDate;
	time_t now_time;
	time(&now_time);
	struct tm* now = localtime(&now_time);
	if (now) {
		sunDate.year=(U16)(now->tm_year + 1900);
		sunDate.month=(U8)(now->tm_mon + 1);
		sunDate.day=(U8)now->tm_mday;
	}
	else{
		return -1;
	}
	int tmp;
	SunDate* pLD=(SunDate*)malloc(sizeof(SunDate));
	tmp=lunarLeapMonth(sunDate.year);
	BL* bl;
	bl=getLunarBLArray(sunDate.year);
	sunToLunar(pLD,&sunDate,bl,tmp);

	char* lunar=(char*)calloc(16,sizeof(char));
	if(tmp){
			if(pLD->month<tmp){
				sprintf(lunar,"%s月",monthNameInYear(pLD->month+1));
			}else if(pLD->month>tmp){
				sprintf(lunar,"%s月",monthNameInYear(pLD->month));
			}else
				sprintf(lunar,"[润]%s月",monthNameInYear(tmp));
		}
		else{
			sprintf(lunar,"%s月",monthNameInYear(pLD->month));
		}
	strcat(lunar,dayNameInMonth(pLD->day));

	sprintf(strDate,"今天是%d年%d月%d日，%s%s年[%s]%s，星期%s，今年的第%d天，今天出生滴娃的星座是%s。",
			sunDate.year,sunDate.month,sunDate.day,
			yearToNature(sunDate),yearToEarth(sunDate),yearToAnimal(sunDate),
			lunar,
			dayNameInWeek(sunDate),
			daysOfSunYear(sunDate),
			starNameInSun(&sunDate));

	free(pLD);
	freeBLArray(bl);
	free(lunar);
	return 0;
}
