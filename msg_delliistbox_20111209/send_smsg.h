//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    send_smsg.h 短信发
//    added by hejizhe 2011-09-13
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "smsg_common.h"
#include <string.h>
#include <stdio.h>
#define SEND_OK 2011
#define SEND_BACK 2012
#define NUM_STATIC 2013
#define NUM_EDIT 2014
#define CONTENT_STATIC 2015 
#define CONTENT_EDIT 2016
#define SEND_STATE_INFO_STATIC 2017

int map=0;


static CTRLDATA CtrlInitSendProgress [] =
{
  {
		"static",
		WS_VISIBLE | SS_CENTER,
		30,10,100,30,
		NUM_STATIC,
		"NUM_STATIC",
		0
	},
	  {
		"edit",
		WS_VISIBLE | SS_CENTER| WS_BORDER|WS_TABSTOP,
		30,50,260,30,
		NUM_EDIT,
		"",
		0
	},
	 {
		"static",
		WS_VISIBLE | SS_CENTER,
		30,90,150,30,
		CONTENT_STATIC,
		"CONTENT_STATIC",
		0
	},
	{
		"edit",
		WS_VISIBLE | SS_CENTER| WS_BORDER|WS_TABSTOP,
		30,130,260,240,
		CONTENT_EDIT,
		"",
		0
	},
	{
		"static",
		WS_VISIBLE | SS_CENTER,
		30,380,260,30,
		SEND_STATE_INFO_STATIC,
		"SEND_SUCCED",
		0
	},
	
	{
		"button",
		WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
		30, 420, 40, 30,
		SEND_OK, 
		"OK",
		0
	},
	  
		{
		"button",
		WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 
		250, 420, 40, 30,
		SEND_BACK, 
		"BACK",
		0
	},
   
};
static DLGTEMPLATE DlgInitSendProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    160, 0, 320, 480, 
    "SEND_MSG",
    0, 0,
    7, NULL,
    0
};
static int InitDialogSendBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	char *phnum=malloc(12);        //驱动
    char *msg_content=malloc(70); 
	int phnum_len; 
	char *buf=malloc(22);
	char *buf_spr=malloc(20);
	//if((phnum=malloc(12)==NULL) || (msg_content=malloc(70)==NULL))
	//	{
	//	  return;   错误
	 //   }
	switch (message) 
	{
	case MSG_INITDIALOG:
	    #ifdef _TIMER_UNIT_10MS
		
			SetTimer ( hDlg,  10,  80 );
		#else 
			SetTimer ( hDlg,  10,  1 );
		#endif

		SetDlgItemText(hDlg, SEND_STATE_INFO_STATIC, "");
		
	/*	char *p;                  //位置? ? ?
        p = Cmd_16c450("CSMS=1",0);
        p = Cmd_16c450("CMGF=1",0); 
        p = Cmd_16c450("CSDH=1",0);
	*/	
		return 1;
	case MSG_TIMER:
		if(map==0)
			{
			  SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"");
			}
			else if(map==1)
			{
			  SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"发送成功");
			  SetDlgItemText(hDlg,NUM_EDIT," ");
			  SetDlgItemText(hDlg,CONTENT_EDIT," ");
			  map=0;
			}
		else
			{
			  SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"发送失败");
			  map=0;
			}
	case MSG_COMMAND:
		switch(wParam)
		{
		    case SEND_OK:
				GetWindowText(GetDlgItem(hDlg,NUM_EDIT),phnum,11);     
				GetWindowText(GetDlgItem(hDlg,CONTENT_EDIT),msg_content,70); 
				printf("GetWindowText  phnum=%s\n", phnum);             
				printf("GetWindowText  msg_content=%s\n", msg_content);  
				// 判断phnum 和msg_content 都不为空
				// 判断phnum 
				if(!*(phnum))
				{
			    	SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"号码为空");
					return;
			    }
				if(!*(msg_content))
				{
					SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"内容为空");
					return;
				}
				
				phnum_len=strlen(phnum);
				
				if((*(phnum)!='1')||((phnum_len)!=11))
				{
					SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"号码错误");
					return;
				}	
				// 连接AT 和CMGS
                sprintf(buf,"%s%s", "\"", phnum);
			    sprintf(buf_spr,"%s%s", buf,"\"");
			    sprintf(buf,"%s%s","CMGS=",buf_spr);
				printf("GSM will send ATCMD:AT+%s\n", buf);
				map=2;
				if(SearchString(Cmd_16c450(buf,0),">"))
				//if(1)
				{
					printf("GSM will send msg:%s\n", msg_content);
					SetDlgItemText(hDlg,SEND_STATE_INFO_STATIC,"Sending......");
					Cmd_16c450(msg_content,1);
					map=1;	
				}	
			    break;

				
			case SEND_BACK:
				KillTimer ( hDlg,  10);
				EndDialog( hDlg, wParam);
				DestroyAllControls(hDlg);
				return 1;
		}
		  
	}
	free(phnum);
	free(msg_content);
	return DefaultDialogProc (hDlg, message, wParam, lParam);

}


static void SEND_SMSG(HWND hWnd)
{
		DlgInitSendProgress.controls = CtrlInitSendProgress;
		
		DialogBoxIndirectParam (&DlgInitSendProgress, hWnd, InitDialogSendBoxProc, 0);
}

