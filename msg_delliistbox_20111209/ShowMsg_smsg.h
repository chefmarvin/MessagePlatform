//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    ShowMsg_smsg.h 短信查看
//    added by YoungLee 2011-10-20
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __SHOWMSG_SMSG_H__

#define __SHOWMSG_SMSG_H__
#include "smsg_common.h"
#include "Confirm_Msg_smsg.h"

#define SHOWMSG_NUM          1996
#define SHOWMSG_TIME         1997
#define SHOWMSG_TITLE        1998
#define SHOWMSG_NUMCOT       1999
#define SHOWMSG_TIMECOT      2000
#define SHOWMSG_CONTENT      2001
#define SHOWMSG_DELETE       2002
#define SHOWMSG_BACK         2003
#define SHOWMSG_KILL         2004
#define SHOWMSG_DLG_TIMER_ID 2005

unsigned char showmsg_del_button_flag = 0;

static CTRLDATA CtrlInitShowMsgProgress [] =
{
    {
		"static",
		WS_VISIBLE | SS_SIMPLE,         
		10, 10, 60, 25,
		SHOWMSG_NUM,
		"短信号码:",
		0
	},
	{
		"static",                       
		WS_VISIBLE | SS_SIMPLE,
		85, 10, 200, 25,
		SHOWMSG_NUMCOT,
		"",
		0
	},
	{
		"static",                      
		WS_VISIBLE | SS_SIMPLE,
		10, 45, 60, 25,
		SHOWMSG_TIME,
		"短信时间:",
		0
	},
	{
		"static",                      
		WS_VISIBLE | SS_SIMPLE,
		85, 45, 200, 25,
		SHOWMSG_TIMECOT,
		"",
		0
	},
	{
		"static",                      
		WS_VISIBLE | SS_SIMPLE,
		10, 80, 60, 25,
		SHOWMSG_TITLE,
		"短信内容:",
		0
	},
	{
		"static",                      
		WS_VISIBLE | SS_SIMPLE,
		85, 80, 200, 300,
		SHOWMSG_CONTENT,
		"",
		0
	},
	{
		"static",
		WS_VISIBLE | SS_SIMPLE,
		40, 300, 240, 25,
		SHOWMSG_KILL,
		"",
		0
	},
	{
		"button",                    
	 	WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		190, 420, 60, 25,
		SHOWMSG_BACK,
		"返回",
		0
	},
	{
		"button",                     
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		40, 420, 60, 25,
		SHOWMSG_DELETE,
		"删除",
		0
	}
};

static DLGTEMPLATE DlgInitShowMsgProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    160, 0, 320, 480, 
    "当前短信",
    0, 0,
    9, NULL,
    0
};

static int InitDialogShowMsgProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	//***********刷新用**********
	HDC        hdc1;
	const RECT msgshow_rect = {0, 0, 320, 480};
	int i=0;
	//******************************
	switch (message) 
	{
	case MSG_INITDIALOG:
		#if 0  // 2011-12-28
		SetDlgItemText(hDlg, SHOWMSG_NUMCOT,  smsg_list[list_cursel + 1].phumber);  
		SetDlgItemText(hDlg, SHOWMSG_TIMECOT, smsg_list[list_cursel + 1].time);
		SetDlgItemText(hDlg, SHOWMSG_CONTENT, smsg_list[list_cursel + 1].messagecontent);
		printf("messge_ID = %d.\n", smsg_list[list_cursel].message_ID);
		#else
		for(i=0; i<SIM_VOLUME; i++)
		{
			if(smsg_list[i].gui_list_num == list_cursel)
			{

				printf("SELECT guilist: %d in SIM: %d\n", list_cursel, smsg_list[i].message_ID);
				SetDlgItemText(hDlg, SHOWMSG_NUMCOT,  smsg_list[i].phumber);  
				SetDlgItemText(hDlg, SHOWMSG_TIMECOT, smsg_list[i].time);
				SetDlgItemText(hDlg, SHOWMSG_CONTENT, smsg_list[i].messagecontent);
				break;
			}			
		}
		printf("SELECT ERROR: %d\n", list_cursel);
		#endif
	case MSG_PAINT:
		printf("MSG_PAINT SHOWSMSG=======================================\n");
		printf("InitDialogShowMsgProc() Confirm_Del=%d\n",Confirm_Del);
		if(Confirm_Del == 1)
		{
			hdc1 = BeginPaint(hDlg);
			EndPaint(hDlg, hdc1);
			EndDialog(hDlg, wParam);
			DestroyAllControls(hDlg);
			del_show_screen = 1;
			Confirm_Del     = 0;
			printf("Destroy ShowMsgP DDDDDDDDDDDDDDDDDDDDDD\n");
			return 1;
		}
		else
		{
			printf("Update Show Screen msg_print UUUUUUUUUUUUUUU\n");
			hdc1= BeginPaint(hDlg);
			EndPaint(hDlg, hdc1);
   			return 1;
		}
		//return 1;
	case MSG_COMMAND:
		switch(wParam)
		{
			case SHOWMSG_DELETE:
			 	CONFIRM_MSG_SMSG(hDlg);
				break;
			case SHOWMSG_BACK:
				EndDialog(hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}
	}
	return DefaultDialogProc(hDlg, message, wParam, lParam);
}
  
static void InitDialogShowMsgBox (HWND hDlg)
{
	DlgInitShowMsgProgress.controls = CtrlInitShowMsgProgress;
	DialogBoxIndirectParam(&DlgInitShowMsgProgress, hDlg, InitDialogShowMsgProc, 0L);
}

static void SHOWMSG_SMSG(HWND hDlg)
{
	InitDialogShowMsgBox(hDlg);
}

#endif
