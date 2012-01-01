//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    Confirm_Msg_smsg.h 确认删除
//    added by YoungLee 2011-10-20
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __CONFIRM_MSG_SMSG_H__

#define __CONFIRM_MSG_SMSG_H__
#include "smsg_common.h"

#define CONFIRMMSG_QUEST     2010
#define CONFIRMMSG_POP       2011
#define CONFIRMMSG_SURE      2012
#define CONFIRMMSG_QUIT      2013
#define CONFIRMMSG_TIMER_ID  2014



unsigned char confirm_msg_button_flag = 0;
int           Confirm_Del             = 0;

static CTRLDATA CtrlInitConfirmMsgProgress [] =
{

	{
		"static",                      
		WS_VISIBLE | SS_SIMPLE,
		20, 50, 240, 40,
		CONFIRMMSG_QUEST,
		"",
		0
	},
	{
		"static",
		WS_VISIBLE | SS_SIMPLE,
		20, 1000, 240, 40,
		CONFIRMMSG_POP,
		"",
		0
	},
	{
		"button",                    
	 	WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		200, 140, 60, 25,
		CONFIRMMSG_QUIT,
		"返回",
		0
	},
	{
		"button",                     
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		20, 140, 60, 25,
		CONFIRMMSG_SURE,
		"确认",
		0
	}
};

static DLGTEMPLATE DlgInitConfirmMsgProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    180, 190, 280, 200, 
    "确认删除",
    0, 0,
    4, NULL,
    0
};

static int InitDialogConfirmMsgProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case MSG_INITDIALOG:
		SetDlgItemText(hDlg, CONFIRMMSG_QUEST, "确认删除?");
		#ifdef _TIMER_UNIT_10MS
			SetTimer(hDlg, CONFIRMMSG_TIMER_ID, 40);
		#else
			SetTimer(hDlg, CONFIRMMSG_TIMER_ID, 1);
		#endif
		return 1;
	case MSG_TIMER:
		printf("pop Nothing done&&&&&&&&&&&&&&&&&&&&&&&&\n");
		if(confirm_msg_button_flag == 1)
		{
			Confirm_Del = Kill_SIM_Msg(list_cursel);
			if(Confirm_Del == 1)
			{
				printf("InitDialogConfirmMsgProc() Confirm_Del=%d\n",Confirm_Del);
				SetDlgItemText(hDlg, CONFIRMMSG_POP, "删除成功!");
			}
			if(Confirm_Del == 0)
				SetDlgItemText(hDlg, CONFIRMMSG_POP, "删除失败请重试!");	
			confirm_msg_button_flag = 2;
		}
		else if(confirm_msg_button_flag == 2)
		{
			confirm_msg_button_flag = 0;
			KillTimer(hDlg, CONFIRMMSG_TIMER_ID);
			EndDialog(hDlg, wParam);
			DestroyAllControls(hDlg);
		}
		return 1;
	case MSG_COMMAND:
		switch(wParam)
		{
			case CONFIRMMSG_SURE:
			 	confirm_msg_button_flag = 1;
				return 1;
			case CONFIRMMSG_QUIT:
				EndDialog(hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}
	}
	return DefaultDialogProc(hDlg, message, wParam, lParam);
}


static void InitDialogConfirmMsgBox (HWND hDlg)
{
	DlgInitConfirmMsgProgress.controls = CtrlInitConfirmMsgProgress;
	DialogBoxIndirectParam(&DlgInitConfirmMsgProgress, hDlg, InitDialogConfirmMsgProc, 0L);
}

static void CONFIRM_MSG_SMSG(HWND hDlg)
{
	InitDialogConfirmMsgBox(hDlg);
}

#endif
