#include "sys.h"
#include "delay.h"
#include "sst25VF016.h"
#include "usart3.h"
#include "ziku.h"


const unsigned char asc2_1608[95][16] = {	  
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*" ",0*/
	{0x00, 0x00, 0x70, 0xF8, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x0D, 0x00, 0x00, 0x00 },/*"!",1*/
	{0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*""",2*/
	{0x00, 0x20, 0xF8, 0xF8, 0x20, 0xF8, 0xF8, 0x20, 0x00, 0x02, 0x0F, 0x0F, 0x02, 0x0F, 0x0F, 0x02},/*"#",3*/
	{0x00, 0x30, 0x78, 0xCE, 0x8E, 0x18, 0x10, 0x00, 0x00, 0x04, 0x0C, 0x38, 0x39, 0x0F, 0x06, 0x00},/*"$",4*/
	{0x18, 0x3C, 0x24, 0xBC, 0xD8, 0x60, 0x30, 0x00, 0x00, 0x06, 0x03, 0x0D, 0x1E, 0x12, 0x1E, 0x0C},/*"%",5*/
	{0x00, 0xB0, 0xF8, 0x48, 0x78, 0x30, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x09, 0x07, 0x0F, 0x09},/*"&",6*/
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*"'",7*/
	{0x00, 0x00, 0xC0, 0xF0, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1F, 0x38, 0x20, 0x00, 0x00},/*"(",8*/
	{0x00, 0x00, 0x08, 0x38, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x20, 0x38, 0x1F, 0x07, 0x00, 0x00},/*")",9*/
	{0x00, 0x80, 0xA0, 0xE0, 0xC0, 0xE0, 0xA0, 0x80, 0x00, 0x00, 0x02, 0x03, 0x01, 0x03, 0x02, 0x00},/*"*",10*/
	{0x00, 0x80, 0x80, 0xE0, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00},/*"+",11*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x3C, 0x1C, 0x00, 0x00},/*",",12*/
	{0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*"-",13*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x00, 0x00},/*".",14*/
	{0x00, 0x00, 0x00, 0x80, 0xE0, 0x78, 0x18, 0x00, 0x00, 0x18, 0x1E, 0x07, 0x01, 0x00, 0x00, 0x00},/*"/",15*/
	{0x00, 0x00, 0xF0, 0xF8, 0x08, 0x68, 0xF8, 0xF0, 0x00, 0x00, 0x07, 0x0F, 0x0B, 0x08, 0x0F, 0x07},/*"0",16*/
	{0x00, 0x20, 0x20, 0x30, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00},/*"1",17*/
	{0x00, 0x30, 0x38, 0x08, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x0C, 0x0E, 0x0B, 0x09, 0x08, 0x08, 0x00},/*"2",18*/
	{0x00, 0x30, 0x38, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x06, 0x0E, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"3",19*/
	{0x00, 0x00, 0xF8, 0xF8, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x03, 0x03, 0x02, 0x02, 0x0F, 0x0F, 0x02},/*"4",20*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0x88, 0x08, 0x00, 0x00, 0x08, 0x08, 0x08, 0x0C, 0x07, 0x03, 0x00},/*"5",21*/
	{0x00, 0xC0, 0xE0, 0x78, 0x58, 0xC8, 0x80, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"6",22*/
	{0x00, 0x08, 0x08, 0x88, 0xE8, 0x78, 0x18, 0x00, 0x00, 0x00, 0x0E, 0x0F, 0x01, 0x00, 0x00, 0x00},/*"7",23*/
	{0x00, 0x70, 0xF8, 0xC8, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x09, 0x0F, 0x07, 0x00},/*"8",24*/
	{0x00, 0xF0, 0xF8, 0x08, 0x08, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x09, 0x0D, 0x0F, 0x03, 0x01, 0x00},/*"9",25*/
	{0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x00, 0x00},/*":",26*/
	{0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x3C, 0x1C, 0x00, 0x00},/*";",27*/
	{0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x08, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x08, 0x00},/*"<",28*/
	{0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00},/*"=",29*/
	{0x00, 0x08, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x08, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00},/*">",30*/
	{0x00, 0x30, 0x38, 0x88, 0xC8, 0x78, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x0D, 0x00, 0x00, 0x00},/*"?",31*/
	{0xF0, 0xF8, 0x08, 0x88, 0xC8, 0x48, 0xF8, 0xF0, 0x07, 0x0F, 0x08, 0x09, 0x0B, 0x0A, 0x0B, 0x0B},/*"@",32*/
	{0x00, 0xE0, 0xF0, 0x18, 0x18, 0xF0, 0xE0, 0x00, 0x00, 0x0F, 0x0F, 0x01, 0x01, 0x0F, 0x0F, 0x00},/*"A",33*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x0F, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"B",34*/
	{0x00, 0xF0, 0xF8, 0x08, 0x08, 0x38, 0x30, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0E, 0x06, 0x00},/*"C",35*/
	{0x00, 0xF8, 0xF8, 0x08, 0x18, 0xF0, 0xE0, 0x00, 0x00, 0x0F, 0x0F, 0x08, 0x0C, 0x07, 0x03, 0x00},/*"D",36*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0x88, 0x08, 0x00, 0x00, 0x0F, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x00},/*"E",37*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0x88, 0x08, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00},/*"F",38*/
	{0x00, 0xF0, 0xF8, 0x08, 0x08, 0x38, 0x30, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x09, 0x0F, 0x0F, 0x00},/*"G",39*/
	{0x00, 0xF8, 0xF8, 0x80, 0x80, 0xF8, 0xF8, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00},/*"H",40*/
	{0x00, 0x00, 0x08, 0xF8, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0F, 0x0F, 0x08, 0x00, 0x00},/*"I",41*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x06, 0x0E, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"J",42*/
	{0x00, 0xF8, 0xF8, 0x80, 0xE0, 0x78, 0x18, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x03, 0x0F, 0x0C, 0x00},/*"K",43*/
	{0x00, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x00},/*"L",44*/
	{0x00, 0xF8, 0xF8, 0x20, 0xC0, 0x20, 0xF8, 0xF8, 0x00, 0x0F, 0x0F, 0x00, 0x01, 0x00, 0x0F, 0x0F},/*"M",45*/
	{0x00, 0xF8, 0xF8, 0x60, 0xC0, 0x80, 0xF8, 0xF8, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0F},/*"N",46*/
	{0x00, 0xF0, 0xF8, 0x08, 0x08, 0xF8, 0xF0, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"O",47*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00},/*"P",48*/
	{0x00, 0xF0, 0xF8, 0x08, 0x08, 0xF8, 0xF0, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x18, 0x3F, 0x27, 0x00},/*"Q",49*/
	{0x00, 0xF8, 0xF8, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x01, 0x0F, 0x0E, 0x00},/*"R",50*/
	{0x00, 0x30, 0x78, 0xC8, 0x88, 0x18, 0x10, 0x00, 0x00, 0x04, 0x0C, 0x08, 0x09, 0x0F, 0x06, 0x00},/*"S",51*/
	{0x00, 0x08, 0x08, 0xF8, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00},/*"T",52*/
	{0x00, 0xF8, 0xF8, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"U",53*/
	{0x00, 0xF8, 0xF8, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x0C, 0x07, 0x03, 0x00},/*"V",54*/
	{0x00, 0xF8, 0xF8, 0x00, 0xC0, 0x00, 0xF8, 0xF8, 0x00, 0x01, 0x0F, 0x0E, 0x01, 0x0E, 0x0F, 0x01},/*"W",55*/
	{0x00, 0x18, 0x38, 0xE0, 0xC0, 0x38, 0x18, 0x00, 0x00, 0x0E, 0x0F, 0x00, 0x01, 0x0F, 0x0E, 0x00},/*"X",56*/
	{0x00, 0x78, 0xF8, 0x80, 0x80, 0xF8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00},/*"Y",57*/
	{0x00, 0x08, 0x08, 0x88, 0xC8, 0x78, 0x38, 0x00, 0x00, 0x0E, 0x0F, 0x09, 0x08, 0x08, 0x08, 0x00},/*"Z",58*/
	{0x00, 0x00, 0xF8, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x40, 0x40, 0x00, 0x00},/*"[",59*/
	{0x00, 0x18, 0x78, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1E, 0x18, 0x00},/*"\",60*/
	{0x00, 0x00, 0x08, 0x08, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7F, 0x7F, 0x00, 0x00},/*"]",61*/
	{0x00, 0x08, 0x0C, 0x06, 0x06, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*"^",62*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40},/*"_",63*/
	{0x00, 0x00, 0x02, 0x06, 0x0E, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*"`",64*/
	{0x00, 0x00, 0x20, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x06, 0x0F, 0x09, 0x09, 0x0F, 0x0F, 0x00},/*"a",65*/
	{0x00, 0xF8, 0xF8, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x0F, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"b",66*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0x60, 0x40, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0C, 0x04, 0x00},/*"c",67*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0xF8, 0xF8, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x0F, 0x00},/*"d",68*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x07, 0x0F, 0x09, 0x09, 0x09, 0x01, 0x00},/*"e",69*/
	{0x00, 0x80, 0xF0, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00},/*"f",70*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0xE0, 0xE0, 0x00, 0x00, 0x47, 0x4F, 0x48, 0x48, 0x7F, 0x3F, 0x00},/*"g",71*/
	{0x00, 0xF8, 0xF8, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00},/*"h",72*/
	{0x00, 0x20, 0x20, 0xEC, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x0F, 0x0F, 0x08, 0x08, 0x00},/*"i",73*/
	{0x00, 0x00, 0x20, 0x20, 0xEC, 0xEC, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x3F, 0x00, 0x00},/*"j",74*/
	{0x00, 0xF8, 0xF8, 0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x0F, 0x0F, 0x01, 0x03, 0x0E, 0x0C, 0x00},/*"k",75*/
	{0x00, 0x08, 0x08, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x0F, 0x0F, 0x08, 0x08, 0x00},/*"l",76*/
	{0x00, 0xE0, 0xE0, 0x20, 0xE0, 0x20, 0xE0, 0xC0, 0x00, 0x0F, 0x0F, 0x00, 0x07, 0x00, 0x0F, 0x0F},/*"m",77*/
	{0x00, 0xE0, 0xE0, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00},/*"n",78*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"o",79*/
	{0x00, 0xE0, 0xE0, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, 0x7F, 0x7F, 0x08, 0x08, 0x0F, 0x07, 0x00},/*"p",80*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0xE0, 0xE0, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x7F, 0x7F, 0x00},/*"q",81*/
	{0x00, 0xE0, 0xE0, 0x80, 0x40, 0x60, 0x60, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00},/*"r",82*/
	{0x00, 0xC0, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x08, 0x09, 0x09, 0x09, 0x0F, 0x06, 0x00},/*"s",83*/
	{0x00, 0x20, 0xF8, 0xF8, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x08, 0x00},/*"t",84*/
	{0x00, 0xE0, 0xE0, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x0F, 0x00},/*"u",85*/
	{0x00, 0xE0, 0xE0, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x0C, 0x07, 0x03, 0x00},/*"v",86*/
	{0x00, 0xE0, 0xE0, 0x00, 0xC0, 0x00, 0xE0, 0xE0, 0x00, 0x03, 0x0F, 0x0C, 0x03, 0x0C, 0x0F, 0x03},/*"w",87*/
	{0x00, 0x60, 0xE0, 0x80, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x0C, 0x0E, 0x03, 0x03, 0x0E, 0x0C, 0x00},/*"x",88*/
	{0x00, 0xE0, 0xE0, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x40, 0x47, 0x4F, 0x68, 0x38, 0x1F, 0x07, 0x00},/*"y",89*/
	{0x00, 0x20, 0x20, 0x20, 0xA0, 0xE0, 0x60, 0x00, 0x00, 0x0C, 0x0E, 0x0B, 0x09, 0x08, 0x08, 0x00},/*"z",90*/
	{0x00, 0x00, 0x80, 0xF0, 0x78, 0x08, 0x00, 0x00, 0x00, 0x01, 0x03, 0x1E, 0x3C, 0x20, 0x00, 0x00},/*"{",91*/
	{0x00, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00},/*"|",92*/
	{0x00, 0x00, 0x08, 0x78, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x1E, 0x03, 0x01, 0x00},/*"}",93*/
	{0x30, 0x18, 0x08, 0x18, 0x30, 0x20, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},/*"~",94*/
};

u8 testziku[1024];



/*
 ***************************************************************
 * ����: eraseZikuFlash 
 * ����: �����ֿⲿ�ֵ�spi flash ����
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void eraseZikuFlash(void)
{
	EraseSectors(256, 70);
}


/*
 ***************************************************************
 * ����: WriteCharToFlash 
 * ����: д�ַ����Ŀ⵽flash
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void WriteCharToFlash(void)
{
	static u8 flag = 0;
	
	if (flag == 0)
	{
		SST25_WriteFlash(CHARADDR, (u8*)asc2_1608[0], sizeof(asc2_1608));
		flag = 1;
	}
}


/*
 ***************************************************************
 * ����: ziku_Write 
 * ����: ���ֿ����
 * ����: 1. index: �������ݵ��������ڼ�k��
 *       2. datalen���������ݵĳ���
 *       3. *data��Ҫ���µ�����
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void ziku_Write(u16 index, u16 datalen, u8 *data)
{
	u32 addr = 0;
	
	addr = index*1024 + ZIKUADDR;
	
	if (datalen < 1024)
	{
		SST25_WriteFlash2(addr, data, datalen);
		uart3_putString("OK");
	}
	else
	{
		SST25_WriteFlash2(addr, data, datalen);
		uart3_putString("Next");			
	}
}