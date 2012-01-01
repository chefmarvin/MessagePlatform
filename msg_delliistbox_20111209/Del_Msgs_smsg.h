//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    Del_Msgs_smsg.h 短信查看
//    added by YoungLee 2011-11-26
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __DEL_MSGS_SMSG_H__

#define __DEL_MSGS_SMSG_H__
#include "smsg_common.h"

#define DEL_MSGS_LIST        2006
#define DEL_MSGS_BUTTON_OK   2007
#define DEL_MSGS_BUTTON_DEL  2008
#define DEL_MSGS_BUTTON_BACK 2009
#define DEL_MSGS_KILL        2010
#define DELMSGS_TIMER_ID     2015



int           Judge_Del_Msgs;
unsigned char msgs_del_button_flag = 0;


static CTRLDATA CtrlInitDelMsgsProgress [] =
{
    {
		CTRL_LISTBOX,
		WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LBS_NOTIFY | LBS_SORT | LBS_AUTOCHECKBOX | LBS_MULTIPLESEL,
		25, 25, 270, 370,
		DEL_MSGS_LIST,
		"",
		0
	},
	{
		"static",
		WS_VISIBLE | SS_SIMPLE,
		110, 400, 100, 25,
		DEL_MSGS_KILL,
		"",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		20, 420, 60, 25,
		DEL_MSGS_BUTTON_OK,
		"确定",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		115, 420, 60, 25,
		DEL_MSGS_BUTTON_DEL,
		"删除",
		0
	},
	{
		"button",
		WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
		220, 420, 60, 25,
		DEL_MSGS_BUTTON_BACK,
		"返回",
		0
	}
};

static DLGTEMPLATE DlgInitDelMsgsProgress =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    160, 0, 320, 480,
    "短信删除",
    0, 0,
    5, NULL,
    0
};


#if 0
static void Init_DelMsgs_list(HWND hDlg)
{
	unsigned short i = 0;
	printf("Init_DelMsgs_list \n");
	SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_RESETCONTENT, 0, (LPARAM)0);
	for(; i < SIM_VOLUME; i++)
	{
		SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_ADDSTRING, 0, (LPARAM)smsg_list[i].phumber);
	}
	SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_SETCURSEL, 0, (LPARAM)0);
}
#else
static void Init_DelMsgs_list(HWND hDlg)
{
	int i = 0;
	//int gui_num=0;
	
	ReadSIMInboxBox();
	
	SendDlgItemMessage (hDlg, DEL_MSGS_LIST, LB_RESETCONTENT, 0, (LPARAM)0);  /* 清空列表*/
	for(i=0; i < SIM_VOLUME; i++)
	{
		if(smsg_list[i].phumber != NULL)
		{
			HICON hIcon1;
			LISTBOXITEMINFO msglistitem;

			//smsg_list[i].gui_list_num=gui_num++;
			smsg_list[i].check_flag = 0;
			//hIcon1=LoadIconFromFile(HDC_SCREEN, "res/audio.ico", 1);
			msglistitem.string = smsg_list[i].phumber;
			msglistitem.cmFlag = CMFLAG_BLANK  /*| IMGFLAG_BITMAP*/; //CMFLAG_CHECKED;
			msglistitem.hIcon  = 0; //(DWORD)GetSystemBitmap(SYSBMP_CHECKMARK);
			SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_ADDSTRING, 0, (LPARAM)&msglistitem);	
			printf("Init_DelMsgs_list_0  LB_ADDSTRING=%s\n", msglistitem.string);
		}
	}
	SendDlgItemMessage (hDlg, DEL_MSGS_LIST, LB_SETCURSEL, 0, (LPARAM)0);  /*设置默认焦点项*/
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
//删除   多选列表回调
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Read_DelMsgs_list(HWND hDlg, int id, int nc, DWORD add_data)
{
	const RECT msgdellist_rect = {0, 0, 320, 480};
	int        check_status    = 0;
	list_cursel = SendMessage(hDlg, LB_GETCURSEL, 0, 0L);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//刷新 多选列表
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Update_DelMsgs_list(HWND hDlg)
{
	int              i, sel_count = 0;
	int             *sel_items;	
	int gui_num=0;
	HICON            hIcon1;
	LISTBOXITEMINFO  msglistitem;
	
	printf("Update_DelMsgs_list()_0\n");
	
	// 根据状态添加列表图标
	SendDlgItemMessage (hDlg, DEL_MSGS_LIST, LB_RESETCONTENT, 0, (LPARAM)0);  /* 清空列表*/
	for(i = 0; i < SIM_VOLUME; i++)
	{
		printf("*********************Ready to read!*******************\n");


		//if(SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_GETCHECKMARK, list_cursel, 0)==CMFLAG_CHECKED)
		if(smsg_list[i].phumber != NULL)
		{
			smsg_list[i].gui_list_num = gui_num++;  /*刷新短信在界面列表中标号*/
			if(smsg_list[i].check_flag == 1)
			{
				hIcon1=LoadIconFromFile(HDC_SCREEN, "res/audio.ico", 1);
				msglistitem.string = smsg_list[i].phumber;
				msglistitem.cmFlag = CMFLAG_CHECKED /*| IMGFLAG_BITMAP*/; //CMFLAG_BLANK;
				msglistitem.hIcon  = hIcon1; //(DWORD)GetSystemBitmap(SYSBMP_CHECKMARK);
				SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_ADDSTRING, 0, (LPARAM)&msglistitem); 
			}
			else
			{
				//hIcon1=LoadIconFromFile(HDC_SCREEN, "res/audio.ico", 1);
				msglistitem.string = smsg_list[i].phumber;
				msglistitem.cmFlag = CMFLAG_BLANK /*| IMGFLAG_BITMAP*/; //CMFLAG_BLANK;
				msglistitem.hIcon  = 0;//msglistitem.hIcon = hIcon1; //(DWORD)GetSystemBitmap(SYSBMP_CHECKMARK);
				SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_ADDSTRING, 0, (LPARAM)&msglistitem); 
			}
		}
	}
}

//处理高亮项
static void disp_hilight(HWND hDlg)
{
	HDC hdc_del;
	int i=0;
	printf("disp_hilight()_____________________________list_cursel=%d\n", list_cursel);
	#if 1
	///*
	for(i=0; i<SIM_VOLUME; i++)
	{
		if(smsg_list[i].phumber)
		{
			printf("smsg_list[%d].gui_list_num=%d\n", i, smsg_list[i].gui_list_num);
			printf("smsg_list[%d].messagecontent=%d\n", i, smsg_list[i].messagecontent);
			printf("smsg_list[%d].check_flag=%d\n", i, smsg_list[i].check_flag);
		}
	}
	//*/
	for(i=0; i<SIM_VOLUME; i++)
	{
		if((smsg_list[i].phumber) && (smsg_list[i].gui_list_num == list_cursel))
		{
			if(smsg_list[i].check_flag == 0)
			{
				smsg_list[i].check_flag = 1;
				printf("SELECT guilist: %d in SIM: %d\n", list_cursel, smsg_list[i].message_ID);
			}
			else
			{
				smsg_list[i].check_flag = 0;
				printf("Cancel guilist: %d in SIM: %d\n", list_cursel, smsg_list[i].message_ID);
			}
			break;
		}
	}
	hdc_del = BeginPaint(hDlg);
	Update_DelMsgs_list(hDlg);
	EndPaint(hDlg, hdc_del);
	#else
	if(smsg_list[list_cursel + 1].phumber != NULL)
	{
		if(smsg_list[list_cursel + 1].check_flag == 0)
		{
			smsg_list[list_cursel + 1].check_flag = 1;
			printf("SELECT SELECT%d\n", list_cursel);
		}
		else
		{
			smsg_list[list_cursel + 1].check_flag = 0;
			printf("CANCEL SELECT %d\n", list_cursel);
		}
		hdc_del = BeginPaint(hDlg);
		Update_DelMsgs_list(hDlg);
		EndPaint(hDlg, hdc_del);
	}
	#endif
}


static int InitDialogDelMsgsProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	int        i;
	int        back;
	int        check = 0;
	
	HDC        hdc_del;
	const RECT delmsgs_rect = {0, 0, 320, 480};
	int        check_status = 0;

	printf("InitDialogDelMsgsProc_1 \n");
	
	switch(message)
	{
	case MSG_INITDIALOG:
		printf("InitDialogDelMsgsProc MSG_INITDIALOG\n");
		SetNotificationCallback(GetDlgItem(hDlg, DEL_MSGS_LIST), Read_DelMsgs_list);
		Init_DelMsgs_list(hDlg);
		Confirm_Del = 0;   // 2011-12-28
		#ifdef _TIMER_UNIT_10MS
			SetTimer(hDlg, DELMSGS_TIMER_ID, 40);
		#else
			SetTimer(hDlg, DELMSGS_TIMER_ID, 1);
		#endif
		return 1;
	case MSG_TIMER:
		printf("DEL_MSGS Nothing done&&&&&&&&&&&&&&&&&&&&&&&&\n");
		if(msgs_del_button_flag == 1)
		{
			ERASE_SIM_MSGS();
			msgs_del_button_flag = 2;
			Update_DelMsgs_list(hDlg);
		}
		else if(msgs_del_button_flag == 2)
		{
			msgs_del_button_flag = 0;
			KillTimer(hDlg, SHOWMSG_DLG_TIMER_ID);
			EndDialog(hDlg, wParam);
			DestroyAllControls(hDlg);
		}
		return 1;
	case MSG_PAINT:
		printf("MSG_PAINT DEL_MSGS=======================================\n");
		hdc_del = BeginPaint(hDlg);
		
		//Update_DelMsgs_list(hDlg);
		EndPaint(hDlg, hdc_del);
		return 1;
	case MSG_COMMAND:
		switch(wParam)
		{
			printf("InitDialogDelMsgsProc_2 \n");
			case DEL_MSGS_LIST:
				printf("InitDialogDelMsgsProc_33333333333333333333333 \n");
					// 查看该高亮当前的选择状态
				// 根据该高亮列表项的当前选中状态更新状态
				check_status = SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_GETCHECKMARK, list_cursel, 0);
				printf("Read_DelMsgs_list list_cursel=%d, check_status=%d\n", list_cursel, check_status);
				if(check_status == CMFLAG_CHECKED)
				{
					// 若当前选中----则取消选中状态
					printf("Read_DelMsgs_list list_cursel=%d, check_status==CMFLAG_CHECKED\n", list_cursel);
					SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_SETCHECKMARK, list_cursel, (LPARAM)CMFLAG_BLANK);
				}
				else
				{
					// 若当前未选中----则置为选中状态
					printf("Read_DelMsgs_list list_cursel=%d, check_status==NO_CHECKED\n", list_cursel);
					SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_SETCHECKMARK, list_cursel, (LPARAM)CMFLAG_CHECKED);
					
				}
				// 查看状态修改了没有
				check_status = SendDlgItemMessage(hDlg, DEL_MSGS_LIST, LB_GETCHECKMARK, list_cursel, 0);
				if(check_status == CMFLAG_CHECKED) printf("Read_DelMsgs_list() After Update status==CMFLAG_CHECKED\n");
				else printf("Read_DelMsgs_list() After Update status==NO_CHECKED\n");

				// 置列表区域为无效-----用于MSG_PRINT的列表刷新重绘
				InvalidateRect(hDlg, &delmsgs_rect, FALSE);
				return 1;
			case DEL_MSGS_BUTTON_OK:
				//( 取消) 选中当前高亮项----然后刷新列表
				disp_hilight(hDlg);				
				return 1;
		    case DEL_MSGS_BUTTON_DEL:
				msgs_del_button_flag = 1;
				return 1;
			case DEL_MSGS_BUTTON_BACK:
				for(i = 0; i < MsgNum; i++)
					smsg_list[i].check_flag = 0;
				EndDialog(hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}
	}
	printf("InitDialogDelMsgsProc_4 \n");
	return DefaultDialogProc(hDlg, message, wParam, lParam);
}

static void InitDelMsgsBox (HWND hDlg)
{
	DlgInitDelMsgsProgress.controls = CtrlInitDelMsgsProgress;
	DialogBoxIndirectParam(&DlgInitDelMsgsProgress, hDlg, InitDialogDelMsgsProc, 0L);
}

static void DEL_MSGS_SMSG(HWND hDlg)
{
	InitDelMsgsBox(hDlg);
}

#endif
