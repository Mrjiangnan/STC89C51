#include <reg52.h>
#define uchar unsigned char 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;		//四位数码管控制位

sbit led1 = P1^0;		//路灯
sbit led2 = P1^1;		//故障报警
sbit key1 = P3^5;		//外界环境检测（1表示亮，0表示暗）
sbit key2 = P3^4;		//灯光检测

static unsigned char second,minute,hour;
unsigned int tcount;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};		//共阴极码字

void delay(uchar i)	//延时函数
{
  uchar j,k; 
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

//显示函数
void display()
{
  
   P0=table[minute/10];		//显示秒的个位
   P2_0 = 0;
   delay(5);
   P2_0 = 1;

   P0=(table[minute%10]& 0x7f);		//显示秒的十位
   P2_1 = 0;
   delay(5);
   P2_1 = 1;

   P0=table[second/10];			//显示分钟的个位
   P2_2 = 0;
   delay(5);
   P2_2 = 1;

   P0=table[second%10];		//显示分钟的十位
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}

void test()
{
	while(0 == key2){
		led2 = 0;
		P0 = 0xbf;
		P2_0 = 0;
		delay(5);
		P2_0 = 1;

		P0 = 0xbf;
		P2_1 = 0;
		delay(5);
		P2_1 = 1;

		P0 = 0xbf;
		P2_2 = 0;
		delay(5);
		P2_2 = 1;

    P0 = 0xbf;
		P2_3 = 0;
		delay(5);
		P2_3 = 1;		
	}
}

void main(void)		//主函数
{
	//中断	
		TMOD=0x02;
    TH0=0x06;
    TL0=0x06;
    TR0=1;
    ET0=1;
    EA=1;
   
 while(1)
	{
   display();
		if(45 >= second){	//晚上
			if(1 == key1)		//外界环境还明亮
				led1 = 1;			//灯不打开
			else {					//外界环境昏暗
				led1 = 0;			//灯打开
				test();				//测试灯是否正常发光
				led2 = 1;			//关闭报警灯光
			}
		}
		else if(15 <= second){
			if(1 == key1)
				led1 = 1;
			else {
				led1 = 0;
				test();
				led2 = 1;
			}
		}
		else if(0 == key1){		//白天天气昏暗
			led1 = 0;
			test();
		}
	}
}

//中断函数
 void t0(void) interrupt 1 using 0 
  {	tcount++;

    if(tcount==4000)
	  {tcount=0;
	    second++;

		 if(second==60)
		  {second=0;
		   minute++;

		    if(minute==60)
			 {minute=0;
			  hour++;

			  if(hour==24)
			   {hour=0;
			   }
			 }
		  }
	  }
	}
