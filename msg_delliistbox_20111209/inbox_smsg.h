//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    Inbox_smsg.h 短信收
//    added by hejizhe 2011-09-13
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef __INBOX_SMSG_H__

#define __INBOX_SMSG_H__
#include "smsg_common.h"
#include "ShowMsg_smsg.h"
#include "Del_Msgs_smsg.h"

#define INBOX_LIST        1991
#define INBOX_BUTTON_OK   1992
#define INBOX_BUTTON_BACK 1993
#define INBOX_DEL_MSGS    1994
#define SMSG_SHOWMSG      1995

static CTRLDATA CtrlInitInboxProgress [] =
{
    {
		CTRL_LISTBOX,
		WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_SORT | LBS_NOTIFY | WS_TABSTOP,
		25, 25, 270, 370,
		INBOX_LIST,
		"",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		40, 420, 60, 25,
		INBOX_BUTTON_OK,
		"确定",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		115, 420, 60, 25,
		INBOX_DEL_MSGS,
		"删除",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		190, 420, 60, 25,
		INBOX_BUTTON_BACK,
		"返回",
		0
	}
};


static DLGTEMPLATE DlgInitInboxProgress =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    160, 0, 320, 480,
    "收件箱",
    0, 0,
    4, NULL,
    0
};

static void ReadSIMInboxBox ()
{
	int   init;
	int   loop;
	char *Inbox_Pt;
	char *Inbox_recv_at = malloc(20);
	int in_gui_list_num=0;

	for(init = 0; init < SIM_VOLUME; init++)
	{
			smsg_list[init].phumber 	   = NULL;
			smsg_list[init].time		   = NULL;
			smsg_list[init].messagecontent = NULL;
			smsg_list[init].message_ID	   = -1;
			smsg_list[init].check_flag     = -1;
			smsg_list[init].gui_list_num   = -1;
	}
	
	for(loop = 0; loop < SIM_VOLUME; loop++)
	{
		sprintf(Inbox_recv_at,"%s%d","CMGR=", loop + 1);
		if(!(SearchString(Cmd_16c450(Inbox_recv_at, 0), "ERR")))
		{
			Inbox_Pt = Cmd_16c450(Inbox_recv_at, 0);
			smsg_list[loop].phumber 	   = Inbox_Message_Src(Inbox_Pt);
			smsg_list[loop].time		   = Inbox_Message_Time(Inbox_Pt);
			smsg_list[loop].messagecontent = Inbox_Message_Content(Inbox_Pt);
			smsg_list[loop].message_ID	   = loop + 1;
			smsg_list[loop].gui_list_num   = in_gui_list_num++;
			/*
			printf("mag_num!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=%d\n", in_gui_list_num);
			printf("smsg_list[loop].phumber =%s\n", smsg_list[loop].phumber );
			printf("smsg_list[loop].time =%s\n", smsg_list[loop].time );
			printf("smsg_list[loop].messagecontent =%s\n", smsg_list[loop].messagecontent );
			printf("smsg_list[loop].message_ID =%d\n", smsg_list[loop].message_ID );
			printf("smsg_list[loop].gui_list_num =%d\n", smsg_list[loop].gui_list_num );	
			*/
		}
	}
	free(Inbox_recv_at);
}

static void Init_inbox_list(HWND hDlg)
{
	unsigned short i = 0;
	ReadSIMInboxBox();
	SendDlgItemMessage(hDlg, INBOX_LIST, LB_RESETCONTENT, 0, (LPARAM)0);
	for(; i < SIM_VOLUME; i++)
	{
		if(smsg_list[i].phumber) SendDlgItemMessage(hDlg, INBOX_LIST, LB_ADDSTRING, 0, (LPARAM)smsg_list[i].phumber);
	}
	SendDlgItemMessage(hDlg, INBOX_LIST, LB_SETCURSEL, 0, (LPARAM)0);
}

static void Read_inbox_list(HWND hDlg, int id, int nc, DWORD add_data)
{
	list_cursel = SendMessage(hDlg, LB_GETCURSEL, 0, 0L);
}

static int InitDialogInboxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	//*********刷新用********
	HDC        hdc;
	const RECT msglist_rect = {0, 0, 320, 480};
	//**************************
	switch(message)
	{
	case MSG_INITDIALOG:
		SetNotificationCallback(GetDlgItem(hDlg, INBOX_LIST), Read_inbox_list);
		Init_inbox_list(hDlg);
		Confirm_Del = 0;   // 2011-12-28
		return 1;
	case MSG_PAINT:   //刷新
		printf("MSG_PAINT INBOX LIST=======================================\n");
		printf("inbox list() Confirm_Del=%d\n",Confirm_Del);
		if(del_show_screen == 1)
		{
			printf("Update inbox list Screen UUUUUUUUUUUUUUUUU\n");
			InvalidateRect(hDlg, &msglist_rect, FALSE);
			hdc = BeginPaint(hDlg);
			EndPaint(hDlg, hdc);
			Init_inbox_list(hDlg);
			Confirm_Del = 0;   // 2011-12-28
			del_show_screen = 0;
		}
		else
		{
			printf("Update inbox list screen SSSSSSSSSSSSSSSSSS\n");
			hdc = BeginPaint(hDlg);
			EndPaint(hDlg, hdc);
		}
		return 1;
	case MSG_COMMAND:
		switch(wParam)
		{
		    case INBOX_BUTTON_OK:
				SHOWMSG_SMSG(hDlg);
				break;
			case INBOX_DEL_MSGS:			
				DEL_MSGS_SMSG(hDlg);
				break;
			case INBOX_BUTTON_BACK:
				EndDialog(hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}
	}
	return DefaultDialogProc(hDlg, message, wParam, lParam);
}
  
static void InitDialogInboxBox (HWND hDlg)
{
	DlgInitInboxProgress.controls = CtrlInitInboxProgress;   
	DialogBoxIndirectParam(&DlgInitInboxProgress, hDlg, InitDialogInboxProc, 0L);
}

static void INBOX_SMSG(HWND hDlg)  
{
	InitDialogInboxBox(hDlg);   
}

#endif
