#ifndef __ARM830_H__
#define __ARM830_H__

#define CPLD_BASE0     (0xd3000000)
#define CPLD_BASE1     (0xd3100000)
#define    OUTPUT      *(unsigned char*)CPLD_BASE0
#define    INPUT        *(unsigned char*)(CPLD_BASE0+0x16) 

#define CS0    (unsigned char*)(CPLD_BASE0+0x30)
#define CS1    (unsigned char*)(CPLD_BASE0+0x40)
#define CS2    (unsigned char*)(CPLD_BASE0+0x50)
#define CS3    (unsigned char*)(CPLD_BASE0+0x60)
#define CS1B   (unsigned char*)(CPLD_BASE0+0x80)
#define CS2B   (unsigned char*)(CPLD_BASE0+0xc0)

#define   GM450RL       *CS0
#define   GM450RH       *(CS0+1)
#define   LCR            *(CS0+3)
#define   LSR            *(CS0+5)

char no_null[200];

extern void delay(int t);
void delay(int t)
{
	while(t>0) t--;
}

extern char* Cmd_16c450(char* cmd,char mode);
char* Cmd_16c450(char* cmd,char mode)
{
   int i,fd;
   char *tmp=no_null;
   //char tmp[200];
   fd=open("/dev/myGSM",0);  //打开驱动
   switch(mode)
   {
      case 0: 
	  	printf("begin ioctl(1)\n");
        i = ioctl(fd,1,(unsigned long)cmd); //发送一般的AT指令,格式AT+...0xd
        printf("end ioctl(1)\n");
        break;
      case 1: 
        i = ioctl(fd,2,(unsigned long)cmd); //发送短信内容,格式...26
        break;
      case 2: 
        i = ioctl(fd,0,(unsigned long)cmd); //拨打电话,格式ATD...";"0xd
        break;
      case 3: 
        i = ioctl(fd,31,(unsigned long)cmd); //接听或挂断电话,格式...0xd
        break;
      case 4: 
        i=ioctl(fd,3,0);    //Start Timer3
        break;
      case 5: 
        i=ioctl(fd,4,0);    //Stop Timer3
        break;
      case 6: 
        i = ioctl(fd,32,(unsigned long)cmd); //直接向450写字符串
        break;
      case 7: 
        i = ioctl(fd,33,(unsigned long)cmd); //直接从450读字符串
        break;
	  
	  //检测有无新短信到达
	  case 8: 
	  	printf("ioctl_37 detecting....\n");
        i = ioctl(fd,37,(unsigned long)cmd); //直接从450读字符串
        break;

	  case 9:
	  	printf("ioctl_36 try to dect msg\n");
		i=ioctl(fd,36,(unsigned long)cmd);   //i=ioctl(fd,36,0);
	  	break;
		
      default: 
        break;
   }
   if(mode<4 || mode==9 || mode==8)
   {   
   		//printf("before read\n");
   		read(fd,tmp,200);
		//printf("Cmd_16c450()-->read(tmp=%s)\n", tmp);
   }
   else tmp = 0;
   //else *tmp='\0';
   close (fd);
   return tmp;   
}
//extern char *StrToDat(char *src,unsigned char lenth);
#endif
