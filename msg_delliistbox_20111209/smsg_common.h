//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   短信平台公共函数     hejizhe 2011-10-01
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef __SMSG_COMMON_H__
#define __SMSG_COMMON_H__

//YoungLee's variables and functions start

#define ListLength 100   
#define SIM_VOLUME 20

int del_show_screen = 0;
int list_cursel     = 0;
int MsgListLength   = 0;
int Judge_MsgNum    = 0;
int MsgNum          = 0;

typedef struct __SMSG_LIST                             
{
	char *phumber;                                       
	char *messagecontent;                                
	char *time;                                          
	int  message_ID;
	int  check_flag;
	int gui_list_num;  /*在列表界面中的编号*/
}SMSG_LIST;

SMSG_LIST smsg_list[ListLength]=                      
{
	{}
};
                                                            
int Kill_SIM_Msg(int kill)
{
	char *Inbox_Kt;
	char *Inbox_del_at = malloc(50); 
	int   StartPosition;
	sprintf(Inbox_del_at, "%s%d", "CMGD=", kill);
	if(1)
	//if(!(SearchString(Cmd_16c450(Inbox_del_at, 0), "ERR")))
	{
		free(Inbox_del_at);
		return 1;
	}
	else
		return 0;
}

void ERASE_SIM_MSGS()
{
	int  i, j;
	char *Del_Kt;
	char *Del_erase_at = malloc(50);
	int BeginErase;
	for(i = 0; i < SIM_VOLUME; i++)
	{
		if(smsg_list[i].check_flag == 1)
		{			
			sprintf(Del_erase_at, "%s%d", "CMGD=", smsg_list[i].message_ID);  
			if(!(SearchString(Cmd_16c450(Del_erase_at, 0), "ERR")))
			{
				smsg_list[i].phumber 	   = NULL;
				smsg_list[i].time		   = NULL;
				smsg_list[i].messagecontent = NULL;
				smsg_list[i].message_ID	   = -1;
				smsg_list[i].check_flag     = -1;
				smsg_list[i].gui_list_num   = -1;

				del_show_screen = 1;
				
				printf("The Message has been deleted.\n");
			}

		}
	}
	free(Del_erase_at);
	return 1;
	
}

char* Inbox_Message_Content(char *p)
{
   char *buf = malloc(50); 
   unsigned char lenth;
   while(SearchString(p++, "\"")) p = SearchString(p, "145");
   printf("p1=%s\n", p);
   if (!p) 
   	   return "不可识别的内容";
   else 
   {
	  p += 3; 
	  printf("p2 = %s\n", p);
	   //一般地,应该在p的基础上增加4("145"为3个字节,再加其后的","共4个字节),
	   //但是在寻找"\""时已经偏移了一个字节,故此处增加3个字节.
	  lenth = 0;
	  while((*p > 0x2f) && (*p < 0x3a))
	  {
		  lenth = lenth * 10 + (*p - 48);
		  p++;
	  }   
	  printf("p3 = %s\n", p);
	  //一般地,在长度字节与数据之间有1至2个字节的控制数据
	  //while((*p<0x30)||((*p>0x39)&&(*p<0x41))||((*p>0x46)&&(*p<0x61))||(*p>0x66)) p++;
	  // changed by hejizhe 2011-08-31
	  while(*p < 0x20) p++;
	  printf("p4 = %s\n", p);
	  //sprintf(buf,"%s%s","短信内容: ",StrToDat(p,lenth));
	  sprintf(buf, "%s%s", "", StrToDat(p, lenth));

	  printf("p5=%s\n", p);
	  printf("p5=%s\n", buf);
	  return buf;
	  
   }
}

char* Inbox_Message_Src(char *p)
{
   char *buf = malloc(22), *buf1 = malloc(14), *tmp;
   tmp = buf1; 
   p = SearchString(p, ",\"");
   if (!p)  
   	   return "不可识别的电话号码!";
   p++; p++; 
   while(*p != '\"') *buf1++ = *p++;
   *buf1 = 0x0;
   sprintf(buf, "%s%s", "", tmp);
   return buf;
}

char* Inbox_Message_Time(char *p)
{
   char *buf = malloc(30), *buf1 = malloc(20), *tmp;
   tmp = buf1; 
   p = SearchString(p, ",,\"");
   if (!p)  
       return "不可识别的时间标识!";
   p++; p++; p++; 
   while(*p != '\"') *buf1++ = *p++;
   *(buf1 - 3) = 0x0;
   sprintf(buf, "%s%s", "", tmp);
   return buf;
}

//YoungLee's variables and functions start

//caoj
char  *msg_num_itoa(char *num_string, int msg_num)
{
	int i,v;
	if (msg_num<=9)
	  i=1;
	else if (msg_num>9&&msg_num<100)
	  i=2;
	else i=3;
	  *(num_string+i)='\0';
	while(i--)
	{
		v=msg_num%10;
		msg_num=msg_num/10;
		*(num_string+i)=v+'0';
	}
	return num_string;
}


 int Read_SIM_SMsg_num()
{
	int  sim_smsg_num = 0;
	char smsg_num_str[3];
	memset(smsg_num_str, 0, 3);
	while(!SearchString(Cmd_16c450(msg_num_itoa(smsg_num_str,sim_smsg_num + 1), 9), "ERROR"))
	{
		sim_smsg_num += 1;
		Judge_MsgNum = 1;
	}
	return sim_smsg_num;
}

//caoj

char *itoa_msg_num(int num, char *str)
{
	int n=num;
	char ch;
	int i=0,j;
	char index[11]="0123456789";
	char str_temp[3];
	
	if(n==0)
	{
	// 若是字符'0'
		str[0]='0';
		str[1]='\0';
	}
	else
	{
		while(n)
		{
			ch=index[n%10];
			n/=10;
			str_temp[i++]=ch;
		}

		for(j=0;j<i;j++)
		{
			str[j]=str_temp[i-j-1];
		}
		str[j]='\0';
	}
	return str;
}

char* SearchString(char* dst,char* target)  //在字符串dst中搜寻字符串target
{
   char *p = NULL,*pdst,*ptarget;
   pdst = dst;ptarget = target;
   while(1)
    {
     while((*pdst != *ptarget)&&(*pdst++ != '\0'));  //开始搜索target的第一个字符 
     if(*pdst == '\0') break;    //如果到 dst的结尾,则退出
     p = pdst;
     while(*pdst++ == *ptarget++);  //继续配对剩余的字符
     if(*(ptarget-1) == '\0') break;  //如果target到结尾,则退出
     else                       //否则继续搜寻整个target,直至到dst结束
       {
        p = NULL;
        ptarget = target;
       }
    } 
   return p;
}

char *StrToDat(char *src,unsigned char lenth) 
{
// 从字串src 上截取长度length 个字符
// 用于截取短信正文
// added by hejizhe 2011-08-31  18:02

	int i=0;
	char *p,*p1;
	p=p1=src;
	printf("msg:%s\n", p1);
	printf("msg_lenth:%d\n", lenth);

	for(;i<lenth;i++)
	{
		p1++;
	}
	*p1='\0';

	printf("msg_content:%s\n", p);
	return p;

}

char* Message_Content(char *p)
{
   char *buf = malloc(50); 
   unsigned char lenth;
   while(SearchString(p++,"\"")) p = SearchString(p,"145");
   printf("p1=%s\n", p);
   if (!p) return "不可识别的内容";
   else 
   {
	  p += 3; 
	  printf("p2=%s\n", p);
	   //一般地,应该在p的基础上增加4("145"为3个字节,再加其后的","共4个字节),
	   //但是在寻找"\""时已经偏移了一个字节,故此处增加3个字节.
	  lenth = 0;
	  while((*p>0x2f)&&(*p<0x3a)) 
	  {
		  lenth = lenth*10+(*p-48);
		  p++;
	  }
	  printf("p3=%s\n", p);
	  //一般地,在长度字节与数据之间有1至2个字节的控制数据
	  //while((*p<0x30)||((*p>0x39)&&(*p<0x41))||((*p>0x46)&&(*p<0x61))||(*p>0x66)) p++;
	  // changed by hejizhe 2011-08-31
	  while(*p<0x20) p++;
	  printf("p4=%s\n", p);
	  //sprintf(buf,"%s%s","短信内容: ",StrToDat(p,lenth));
	  sprintf(buf,"%s%s","短信内容: ",StrToDat(p,lenth));

	  printf("p5=%s\n", p);
	  printf("p5=%s\n", buf);
	  return buf;
	  
   }
}

char* Message_Src(char *p)
{
   char *buf = malloc(22),*buf1 = malloc(14),*tmp;
   tmp = buf1; 
   p = SearchString(p,",\"");
   if (!p)  return "不可识别的电话号码!";
   p++;p++; 
   while(*p != '\"') *buf1++ = *p++;
   *buf1 = 0x0;
   sprintf(buf,"%s%s","短信来自: ",tmp);
   return buf;
} 
char* Message_Time(char *p)
{
   char *buf = malloc(30),*buf1 = malloc(20),*tmp;
   tmp = buf1; 
   p = SearchString(p,",,\"");
   if (!p)  return "不可识别的时间标识!";
   p++;p++;p++; 
   while(*p != '\"') *buf1++ = *p++;
   *(buf1-3) = 0x0;
   sprintf(buf,"%s%s","收到时间: ",tmp);
   return buf;
}
#endif
