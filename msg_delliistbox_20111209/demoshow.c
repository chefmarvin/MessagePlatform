/* 
**
** License: GPL
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "arm830.h"

#include "gsm_driver_app.h"
#include "send_smsg.h"
#include "inbox_smsg.h"
#include "help_smsg.h"
#include "smsg_common.h"

#define SMSG_SEND 1001
#define SMSG_OK 1002
#define SMSG_INBOX 1003
#define SMSG_HELP 1004
#define SMSG_STATE 1005

int shijian=8;
int new_msmsmsg_flag=0;
int new_flag=0;
int shijian2=5;


///*
char have_sim = 0;
char *sim_info;
//*/
static DLGTEMPLATE MainDlgInitProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    160, 0, 320, 480, 
    "短信平台",
    0, 0,
    5
};

#define IDC_PROMPTINFO	100
#define IDC_PROGRESS  	110

//char *numstring;
char numstring[3];

static CTRLDATA MainCtrlInitProgress [] =
{ 

	{
		"static",
		 SS_LEFT | WS_VISIBLE,
		10,100,280,300,
		SMSG_STATE,
		"0000",
		0
	},
	{
		"button",
		WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
		10, 420, 60, 25,
		SMSG_SEND, 
		"发短信",
		0
	},
	{
		"button",
		WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
		90, 420, 60, 25,
		SMSG_OK, 
		"确定",
		0
	},
	{
		"button",
		WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
		170, 420, 60, 25,
		SMSG_INBOX, 
		"收短信",
		0
	},
    {
        "button",
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        250, 420, 60, 25,
        SMSG_HELP, 
        "帮助",
        0
    }
};
char GSM_Ini()
{
   char *p;
   p = Cmd_16c450("CSMS=1",0);
   p = Cmd_16c450("CMGF=1",0); 
   p = Cmd_16c450("CSDH=1",0);
   p = Cmd_16c450("CLVL=250",0);

   if(p=SearchString(Cmd_16c450("CPIN?",0),"READY"))  have_sim = 1;//检测是否有SIM卡
   else have_sim = 0;   
   return have_sim;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//           读取SIM 卡中已有短信数目  hejizhe 2011-10-01
//////////////////////////////////////////////////////////////////////////////////////////////////////////



#if 0
static void msg_num_itoa(char *num_string, int msg_num)
{
int i,v;
//char ch;
if (msg_num<=9)
  i=1;
  else if (msg_num>9&&msg_num<100)
  i=2;
  else i=3;
 // *(num_string+i)='\0';
while(i--)
{
v=msg_num%10;
msg_num=msg_num/10;
*(num_string+i-1)=v+'0';
}
}
#else

#endif

static int InitMainDialogBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	char *Pt;
	char *recv_at=malloc(20);
	//char *str;

	char *str1=malloc(200);
	char *str2=malloc(200);
	int time=0;
	int shijian3=0;
	
    switch (message) 
	{
    case MSG_INITDIALOG:
		//检测GSM
#ifdef _TIMER_UNIT_10MS
			SetTimer ( hDlg,  0,  10 );
#else 
			SetTimer ( hDlg,  0,  1 );
#endif

		if(!GSM_Ini())  SetDlgItemText(hDlg,SMSG_STATE,"\n你SIM卡呢!"); 
		else //显示SIM的信息
		{
			if(SearchString(Cmd_16c450("COPS?",0),"CHN-CUGSM")) 
				sim_info = "\n\t\t\t\t\t\t\t\t\t\t\t\t中国联通";
			else  sim_info = "\n\t\t\t\t\t\t\t\t\t\t\t\t中国移动";
			SetDlgItemText(hDlg,SMSG_STATE,sim_info);
		}
		//*/
		MsgNum = Read_SIM_SMsg_num();
        return 1;
	case MSG_TIMER:
		printf("MSG_TIMER\n");
		if(new_msmsmsg_flag==1)
		{
			time++;
			if(time>shijian)
			{
				time=0;
				new_msmsmsg_flag=0;
				SetDlgItemText(hDlg,SMSG_STATE,"");
			
			}

		}
		// 
		//msgNum=Read_SIM_SMsg_num();   //返回当前SIM 卡中有多少条短信
		//msg_num_itoa(numstring,msgNum+1);
		//if(!SearchString(Cmd_16c450(numstring,9),"ERROR"))
		if(!SearchString(Cmd_16c450(msg_num_itoa(numstring,MsgNum+1),9),"ERROR"))
		{
			printf("NEW SMSMSG\n");
			new_msmsmsg_flag=1;
			SetDlgItemText(hDlg,SMSG_STATE,"NEW SMSMSG");
			//
			//sprintf(P,"%s%s", "\"", phnum);
            //sprintf(P_spr,"%s%s", P,"\"");
            //sprintf(P,"%s%s","CMGR=",P_spr);
			sprintf(recv_at,"%s%s","CMGR=", msg_num_itoa(numstring,MsgNum+1));
			Pt = Cmd_16c450(recv_at,0);
			//sprintf(str,"%s%s","\t", Message_Src( Pt));
			//sprintf(str,"%s%s",Message_Src( Pt), "\n");
			//sprintf(str,"%s%s","\t", Message_Time(Pt));
			//sprintf(str,"%s%s",Message_Time(Pt), "\n");
			//sprintf(str,"%s%s","\t", Message_Content(Pt));

			sprintf(str1,"\n%s\n%s",Message_Src( Pt),Message_Time(Pt));
			sprintf(str2,"%s\n%s",str1,Message_Content(Pt));
			
			//sprintf(str,"%s%s",Message_Content(Pt), "\n");
			//SetDlgItemText(hDlg,SMSG_STATE,Message_Src( Pt));
			//SetDlgItemText(hDlg,SMSG_STATE,Message_Time(Pt));
			//SetDlgItemText(hDlg,SMSG_STATE,Message_Content(Pt));
			SetDlgItemText(hDlg,SMSG_STATE,str2);
			
			MsgNum+=1;
		}
		//{
		//	// printf("有新短信\n");
		//msg_num_itoa(numstring,msgNum+1);
		
		// 
		//}
		return 1; 
    case MSG_COMMAND:
        switch (wParam) 
		{
        case SMSG_SEND:
			//发短信-----处理
			SEND_SMSG(hDlg);
			break;
		case SMSG_OK:
			//查看新短信-----处理
			
			break;
		case SMSG_INBOX:
			//收件箱-----处理
			INBOX_SMSG(hDlg);
			break;
		case SMSG_HELP:
			//帮助-----处理
			HELP_SMSG(hDlg);
			break;
        }
        break;
        
    }
	free(str1);
	free(str2);
	free(recv_at);
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}


static void InitMainDialogBox (HWND hDlg)
{
    MainDlgInitProgress.controls = MainCtrlInitProgress;
	MainDlgInitProgress.controlnr=5;
    
    DialogBoxIndirectParam (&MainDlgInitProgress, hDlg, InitMainDialogBoxProc, 0L);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _LITE_VERSION
    SetDesktopRect(0, 0, 1024, 768);
#endif
    
    InitMainDialogBox (HWND_DESKTOP);

    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

