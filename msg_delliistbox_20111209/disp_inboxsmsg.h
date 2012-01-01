//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    处理收件箱中的某一条短信   hejizhe 2010-10-01
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if 0
#ifndef __DISP_INBOXSMSG_H__
#define __DISP_INBOXSMSG_H__
#include "smsg_common.h"
#include "inbox_smsg.h"

#define  DISPSMSG_PHNUM_STATIC 		1411
#define  DISPSMSG_PHNUM_EDIT 		1412
#define  DISPSMSG_TIME_STATC 		1413
#define  DISPSMSG_TIME_EDIT 		1414
#define  DISPSMSG_CONT_STATIC 		1415
#define  DISPSMSG_CONT_EDIT 		1416
#define  DISPSMSG_CANCEL_BUTTON 	1417
#define  DISPSMSG_BACK_BUTTON 		1418

#define DISPSMSG_DLG_CTRL_NUM 8
static CTRLDATA CtrlDispSmsgProgress[DISPSMSG_DLG_CTRL_NUM];
static DLGTEMPLATE DispSmsgDlgInitProgress;


//extern SMSG_DATA_STRUCT smsg_data_list[SIM_MSG_MAX_NUM];
//extern unsigned short int cur_inbox_sel; 
//extern unsigned short int Sim_Msg_Num;


// 显示当前焦点下的短信
void Disp_Current_Smsg(HWND hDlg)
{
	SetDlgItemText(hDlg,DISPSMSG_PHNUM_EDIT,smsg_data_list[cur_inbox_sel].smsg_phnum);
	SetDlgItemText(hDlg,DISPSMSG_TIME_EDIT,smsg_data_list[cur_inbox_sel].smsg_time);
	SetDlgItemText(hDlg,DISPSMSG_CONT_EDIT,smsg_data_list[cur_inbox_sel].smsg_content);
}
//删除当前焦点下的短信
void Cancel_Current_Smsg(HWND hDlg, WPARAM wParam)
{
	int i=0, sim_smsg_num=0;
	char smsg_num_str[3];
	char *buf = malloc(10),*smsg;
	
	memset(smsg_num_str,0,3);
	smsg=smsg_num_str;
	sim_smsg_num=cur_inbox_sel;
	sprintf(buf,"%s%s","CMGD=",itoa_msg_num(sim_smsg_num+1,smsg_num_str));		
	if(SearchString(Cmd_16c450(buf,0),"ERR")) 
	{
		printf("ERROR! Cancel smsg Unsuccess\n");
	}
	else
	{
		Sim_Msg_Num-=1;
		printf("Cancel smsg Success\n");
		//退出当前界面，回到短信列表，更新短信列表
		EndDialog( hDlg, wParam);
		DestroyAllControls(hDlg);

		INBOX_SMSG(hDlg);
	}
	free(buf);
}


static int InitDispSmsgDialogProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case MSG_INITDIALOG:
		// 显示该条短信
		Disp_Current_Smsg(hDlg);
		return 1;
	case MSG_COMMAND:
		switch(wParam)
		{
			case DISPSMSG_CANCEL_BUTTON:				
				//删除这条短信
				Cancel_Current_Smsg(hDlg, wParam);
				return 1;
			case DISPSMSG_BACK_BUTTON:
				EndDialog( hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}		  
	}
	return DefaultDialogProc (hDlg, message, wParam, lParam);
}




static void InitDispSmsgDialog (HWND hDlg)
{
	DispSmsgDlgInitProgress.dwStyle = WS_BORDER | WS_CAPTION;
	DispSmsgDlgInitProgress.dwExStyle = WS_EX_NONE;
	DispSmsgDlgInitProgress.x = 240; // 160
	DispSmsgDlgInitProgress.y = 0;
	DispSmsgDlgInitProgress.w = 320;
	DispSmsgDlgInitProgress.h = 480;
	DispSmsgDlgInitProgress.caption = smsg_data_list[cur_inbox_sel].smsg_phnum; //"查看短信";
	DispSmsgDlgInitProgress.hIcon = 0;
	DispSmsgDlgInitProgress.hMenu = 0;
	DispSmsgDlgInitProgress.controlnr = DISPSMSG_DLG_CTRL_NUM;
	DispSmsgDlgInitProgress.controls = CtrlDispSmsgProgress;
	DispSmsgDlgInitProgress.dwAddData = 0;

	CtrlDispSmsgProgress[0].class_name="static";
	CtrlDispSmsgProgress[0].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[0].x =10;
	CtrlDispSmsgProgress[0].y =10;
	CtrlDispSmsgProgress[0].w =50;
	CtrlDispSmsgProgress[0].h =30;  // 390
	CtrlDispSmsgProgress[0].id = DISPSMSG_PHNUM_STATIC;   
	CtrlDispSmsgProgress[0].caption ="号码:";
	
	CtrlDispSmsgProgress[1].class_name="static";
	CtrlDispSmsgProgress[1].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[1].x =60;
	CtrlDispSmsgProgress[1].y =10;
	CtrlDispSmsgProgress[1].w =200;
	CtrlDispSmsgProgress[1].h =30;  // 390
	CtrlDispSmsgProgress[1].id = DISPSMSG_PHNUM_EDIT;   
	CtrlDispSmsgProgress[1].caption ="";
	
	CtrlDispSmsgProgress[2].class_name="static";
	CtrlDispSmsgProgress[2].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[2].x =10;
	CtrlDispSmsgProgress[2].y =50;
	CtrlDispSmsgProgress[2].w =300;
	CtrlDispSmsgProgress[2].h =30;  // 390
	CtrlDispSmsgProgress[2].id = DISPSMSG_TIME_STATC;   
	CtrlDispSmsgProgress[2].caption ="时间:";
	
	CtrlDispSmsgProgress[3].class_name="static";
	CtrlDispSmsgProgress[3].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[3].x =60;
	CtrlDispSmsgProgress[3].y =50;
	CtrlDispSmsgProgress[3].w =200;
	CtrlDispSmsgProgress[3].h =30;  // 390
	CtrlDispSmsgProgress[3].id = DISPSMSG_TIME_EDIT;   
	CtrlDispSmsgProgress[3].caption ="";
	

	CtrlDispSmsgProgress[4].class_name="static";
	CtrlDispSmsgProgress[4].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[4].x =10;
	CtrlDispSmsgProgress[4].y =90;
	CtrlDispSmsgProgress[4].w =300;
	CtrlDispSmsgProgress[4].h =30;  // 390
	CtrlDispSmsgProgress[4].id = DISPSMSG_CONT_STATIC;   
	CtrlDispSmsgProgress[4].caption ="内容:";
	

	CtrlDispSmsgProgress[5].class_name="static";
	CtrlDispSmsgProgress[5].dwStyle= WS_VISIBLE | SS_LEFT;
	CtrlDispSmsgProgress[5].x =60;
	CtrlDispSmsgProgress[5].y =90;
	CtrlDispSmsgProgress[5].w =200;
	CtrlDispSmsgProgress[5].h =90;  // 390
	CtrlDispSmsgProgress[5].id = DISPSMSG_CONT_EDIT;   
	CtrlDispSmsgProgress[5].caption ="";

	CtrlDispSmsgProgress[6].class_name="button";
	CtrlDispSmsgProgress[6].dwStyle= WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON;
	CtrlDispSmsgProgress[6].x =10;
	CtrlDispSmsgProgress[6].y =420;
	CtrlDispSmsgProgress[6].w =60;
	CtrlDispSmsgProgress[6].h =25;
	CtrlDispSmsgProgress[6].id = DISPSMSG_CANCEL_BUTTON;   
	CtrlDispSmsgProgress[6].caption ="删除";

	CtrlDispSmsgProgress[7].class_name="button";
	CtrlDispSmsgProgress[7].dwStyle= WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON;
	CtrlDispSmsgProgress[7].x =250;
	CtrlDispSmsgProgress[7].y =420;
	CtrlDispSmsgProgress[7].w =60;
	CtrlDispSmsgProgress[7].h =25;
	CtrlDispSmsgProgress[7].id = DISPSMSG_BACK_BUTTON;   
	CtrlDispSmsgProgress[7].caption ="返回";

	DialogBoxIndirectParam (&DispSmsgDlgInitProgress, hDlg, InitDispSmsgDialogProc, 0);

}


void Disp_this_inbox_smsg(HWND hDlg/*, SMSG_DATA_STRUCT *smsg_data, unsigned short int cursel_index*/)
{
	InitDispSmsgDialog(hDlg);
}
#endif
#endif
