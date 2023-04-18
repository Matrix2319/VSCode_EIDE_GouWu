
#include "move.h"

#define SEEP1  120
#define SEEP2  120
#define SEEP9  100

#define ys  79
extern u8 shu_h[16];
u8 FX_flag=0;
u8 move_BUFF_SET_hang = 0;	
/*
1			格数
2			方向
*/

u8 move_BUFF_SET[15][2] = {0};
u8 move_BUFF_SET2_hang = 0;	
u8 move_BUFF_SET2[15][2] = {0};
u8 move_BUFF_SET3_hang = 0;	
u8 move_BUFF_SET3[15][2] = {0};

#define ce_move1_BUFF_hang 2	
u8 ce_move1_BUFF[10][11] = {
							5,1,1,0,0, 0,0,0,0,0,0,
							5,0,1,0,0, 0,0,0,0,0,0,
							};
#define ce_move2_BUFF_hang 4	
u8 ce_move2_BUFF[10][11] = {
							5,1,1,3,0, 0,0,0,0,0,0,
							2,1,1,4,0, 0,0,0,0,0,0,
							5,0,1,3,0, 0,0,0,0,0,0,
							2,0,1,4,0, 0,0,0,0,0,0,
							};


#define chu_hang3  6	//一黄一绿	前一后一		16.07s
u8 chu_3[6][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define chu_hang4  7	//两黄两绿			
u8 chu_4[7][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define chu_hang5  3	//不抓		
u8 chu_5[3][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define sb_hang    4	//放双倍
u8 sb[4][11] = {0,0,0,0,0,0,0,0,0,0,0};	
#define zou_hang1  22	//一绿二黄（可能三黄）	上坡抓 分两次		35s
u8 zou_1[22][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define zou_hang2  18	//一绿二黄（可能三黄）	后空	25s
u8 zou_2[18][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define zou_hang3  10	//启动用  不抓木块
u8 zou_3[10][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fa_hang1  27	//	只偷
u8 fa_1[27][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fa_hang2  30	//	偷+启动黄
u8 fa_2[30][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fa_hang3  12	//	
u8 fa_3[12][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fangfa_hang  14	
u8 fangfa[14][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fangfa_hang1  13	
u8 fangfa_1[13][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fangfa_hang2  3	
u8 fangfa_2[3][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define fangfa_hang3  17	
u8 fangfa_3[17][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define wujie_hang  12	
u8 wujie[12][11] = {0,0,0,0,0,0,0,0,0,0,0};
#define tou_hang  21	//启动时偷	
u8 tou[21][11] = {0,0,0,0,0,0,0,0,0,0,0};	
#define LUXIAN1_hang  26		//完胜 25.96或38s
u8 LUXIAN1[26][11] = {0,0,0,0,0,0,0,0,0,0,0};

#define chu_hang1  6	
u8 chu_1[6][11]  = {
					7,1,2,0,0,12,114,26,0,0, 0,
					1,0,1,4,0, 0,  0, 0,0,0,50,
					5,1,3,4,0,25,  0, 0,0,0,50,
					4,1,3,0,0,24,  0, 0,0,0, 0,
					2,0,2,3,0, 0,  0, 0,0,0,52,
					2,1,2,0,0, 0,  0, 0,0,0, 0
					};
#define chu_hang2 2	//	先双倍		18s
u8 chu_2[4][11] = {
					5,1,4,0,0,	0,0,0,0,0,0,
//					1,1,1,4,0,	0,0,0,0,0,0,
					5,0,4,0,0,	0,0,0,0,0,0,
					1,0,1,4,0,	0,0,0,0,0,0,
	
					};

//					1,1,3,0,0,	31,24,0,0,0,0,
//					1,0,3,0,0,	113,35,0,0,0,0,

//case 1 :	Hand_SHENZHI();		break;	// 
//case 2 :	Hand_SANJIAO();		break;	// 
//case 3 :	Hand_BAZI();		break;	// 
//case 4 :	Hand_SHENZHI_0();	break;	// 
//case 5 :	Hand_SHENZHI_1();	break;	//
//case 6 :	Hand_SHOUHUI_0();	break;	// 
//case 7 :	Hand_SHOUHUI_1();	break;	// 
//case 8 :	Hand_SANJIAO_0();	break;	// 
//case 9 :	Hand_SANJIAO_1();	break;	//
//case 10:	Hand_BAZI_0();		break;	//
//case 11:	Hand_BAZI_1();		break;	//	
				  
/*******************

输入参数：

	t  	格数，					   不走就=0
	f  	方向：1前，0后，
	sp 	速度，
	r  	转向方向：3左转，4右转，   不转就=0
	tim	定时，转弯60，前进一格67   不用就=0
	dj_1舵机动作      十位动作    个位格数		不动就十位0
	dj_2舵机动作      十位动作    个位格数
	dj_3舵机动作      十位动作    个位格数
	dj_4舵机动作      十位动作    个位格数
	YS  多久舵机动作
		  
********************/
void DJ_MOVE_ZONG(u8 t,u8 f,u8 sp,u8 r,u16 tim,u8 dj_1,u8 dj_2,u8 dj_3,u8 dj_4,u8 YS)
{
	if(tim < 5&&t !=0)
	{
		DJ_MOVE(t,f,sp,dj_1,dj_2,dj_3,dj_4,YS);
//		DJ_MOVE(t,f,sp);
		delay_ms(100);
	}
	
	else if(tim>=5)
	{
		DJ_MOVE_YS(t,f,sp,tim);
		delay_ms(100);
	}
	
	if(r == 3||r == 4)
	{
		DJ_MOVE_ZHUAN(r,f);
		delay_ms(100);
	}
}

void DJ_MOVE_BUFF_SET(u8 t,u8 f, u8 yanshi)       //只需要看这个函数，车的移动全靠这个函数
{
	u8 sp;
	u8 shijian = 0;
	if(t == 9)       //无用
	{
//		Hand_SHENZHI();
//		delay_ms(100);
//		DJ_MOVE(1,1,1,0,0,0,0,0);
//		delay_ms(100);
//		DJ_MOVE(1,0,1,0,0,0,0,0);
//		Hand_SANJIAO_xiao();
//		delay_ms(100);
//		DJ_MOVE(1,1,1,0,0,0,0,0);
//		delay_ms(100);
//		DJ_MOVE(1,0,1,0,0,0,0,0);
	}
	else             
	{
		sp = 4;        //速度4档
		if(sp >= 4)
		{
			sp = 4;
		}
		if(f==3||f==4)//先进行左右转
		{
			
//			Hand_SANJIAO();
			delay_ms(10);
			DJ_MOVE_ZHUAN(f,0);
			f = 1;
		}
		if(f==5||f==6)
		{
			shijian = yanshi;
			DJ_MOVE_PingYi(f, shijian);
		}
		DJ_MOVE(t,f,sp,0,0,0,0,0);    //前进函数

//		Hand_SANJIAO();
		delay_ms(50);
		
	}
	
}

void DJ_MOVE_BUFF_shuzu(u8 a[12][2],u8 hang)
{
	u8 i = 0;
		
	for(i = 0;i<hang;i++)
	{
		DJ_MOVE_BUFF_SET(a[i][0],a[i][1],a[i][4]);
	}	
	stop();
	
}



void HAND_FUWEI(void)
{	
	
}
void HAND_ZHUA(void)
{	
	
}

void HAND_FANG(void)
{
	
}

void HAND_ZHUAMUKUAI(void)
{

}


void shuzu(u8 a[50][11],u8 hang)
{
	u8 i = 0;
	u8 n,m;
		
	for(i = 0;i<hang;i++)
	{
		DJ_MOVE_ZONG(a[i][0],a[i][1],a[i][2],a[i][3],a[i][4],a[i][5],a[i][6],a[i][7],a[i][8],a[i][9]);
		if(a[i][10] != 0)
		{
			n = a[i][10]/10;		//十位
			m = a[i][10]%10;		//个位
				
//			if(n == 5)			//前舵机
//			{
//				if(m == 0)			//伸直
//					Hand_SHENZHI();
//				else if(m == 1)		//三角
//					Hand_SANJIAO();
//				else if(m == 2)		//八字
//					Hand_BAZI();
//			}
		}
	}	
}


void ce(void)
{
//	shuzu(chu_1,chu_hang1);
	shuzu(chu_2,chu_hang2);
}
void ce_1(void)
{
	shuzu(ce_move1_BUFF,ce_move1_BUFF_hang);
}
void ce_2(void)
{
	shuzu(ce_move2_BUFF,ce_move2_BUFF_hang);
}

void WJ(void)
{
	shuzu(wujie,wujie_hang);
}

void chu_1_1(void)		//	不用	1.35抓完
{
	shuzu(chu_1,chu_hang1);
	shuzu(sb,sb_hang);
	shuzu(zou_1,zou_hang1);
	shuzu(fa_1,fa_hang1);
}
void chu_2_1(void)		//41s完胜		先双倍
{
	shuzu(chu_2,chu_hang2);
	shuzu(fa_3,fa_hang3);
}
void chu_3_1(void)		//46s完胜		1.33抓完
{
	shuzu(chu_3,chu_hang3);
	shuzu(sb,sb_hang);
	shuzu(zou_1,zou_hang1);
	shuzu(fa_1,fa_hang1);
}
void chu_4_1(void)		//己方抓完 
{
	shuzu(chu_4,chu_hang4);
	shuzu(sb,sb_hang);
	shuzu(zou_1,zou_hang1);
//	shuzu(zou_3,zou_hang3);
	shuzu(fa_1,fa_hang1);
}

void chong_1(void)		
{
	shuzu(chu_4,chu_hang4);
	shuzu(zou_1,zou_hang1);
	shuzu(fa_1,fa_hang1);
}
void chong_2(void)		
{
	shuzu(chu_5,chu_hang5);	
	shuzu(zou_1,zou_hang1);
	shuzu(fa_1,fa_hang1);
	
}
void chong_3(void)		
{
	shuzu(tou,tou_hang);	
	shuzu(zou_1,zou_hang1);
	shuzu(fa_1,fa_hang1);
	
}
void chong_4(void)		
{
	shuzu(tou,tou_hang);	
	shuzu(zou_3,zou_hang3);
	shuzu(fa_1,fa_hang1);
}

void xian(void)		
{
	shuzu(fangfa,fangfa_hang);	
}
void xian2(void)		
{
	shuzu(fangfa_1,fangfa_hang1);	
}
void xian3(void)		
{
	shuzu(fangfa_2,fangfa_hang2);	
}
void xian4(void)		
{
	shuzu(fangfa_3,fangfa_hang3);	
}

void WanSheng(void)		//30s  稳定
{
	
}	

void LUXIAN_1(void)
{
	

}
		
void LUXIAN_2(void)
{
	
}	


