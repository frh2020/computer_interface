
#include <graphics.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <graphics.h>
#include <alloc.h>
#include <process.h>
#include <stdmt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <prns.c>


#define WindowMinX 5
#define WindowMinY 50
#define WindowMidX 400
#define WindowMidY 240
#define WindowMaxX 633
#define WindowMaxY 473


void getitemcount(char **, int *, int *, int *);
void InitialGraphics(void);
void MainM(void);
void SetMainbM(void);
void MianMD(void);
void SelectMainM(int select, char In_Active);
int  SubM(void);
void SetSubM(char **name, int ord);
int SubMD(int ord);
void SelectSubM(int ord, int select, char In_Active);
void Help_pageshow(void){return;};
void Exitmenu(int);
int GetKey(void);
void Functions(int ID);
void ClearAll(void);
void Draw(void);
void About(void);
void SaveAs(int ID);
void SaveData(char *fname);
void SavePict(char *fname);
void SaveVram(void);
void PutVram(void);
void MessageBox(char *Message,int sign);
void Go1(void);
void Go2(void);
void MoveM();
void ControlWin(void);
void Keyselect(int horo,int viter);
int GetData();
void DisplayData(int x);
void MotorInitial(void);
void MotorStep(int direction);
void InitialInfo(void);
void IndicateWin(int number);
void Showing(char *showtext,int textcolor);
void PowerShow(void);
char *ToString(int values);
void QuitTo(char *pcInf1, char *pcInf2, char *pcInf3);
void durations(void);
void SetDuration(void);

MENUTYPE MainMenu,SubMenu[INUM];
int maxx, texth, textw, i;
static long setupp[4]={10,50,4000,200};
static int datastream[5000],data0,dtemp,x0temp;
void *MenuImageBuf, *savescrbuf[4]  ;
char *doing={"Computer Now Waiting..."};
char *message[3]={"0","Data File Name:         ","Directory Name:            "};
char *setuptext[][2]={"Resolution orders:(1-10)      ", "     *0.28fs",
			       "measuring Time/per step:     ", " ms" ,
			       "Measuring Length    ", "     fs" ,
			       "Arrange of Wavelength", " nm",};

int gmode;
static int motornum=1;
long steps[3]={0,36000,500};
static float Mindex[3]={0,3.6,5},DataLong;
static int stat;
long dissteps=0;
void main(void)
{
 InitialGraphics();
 MotorInitial();
 maxx=getmaxx();
 texth=textheight("text");
 textw=textwidth("t");


 SetMainbM();
 SetSubM(SubMenuItem1,0);
 SetSubM(SubMenuItem2,1);
 SetSubM(SubMenuItem3,2);
 SetSubM(SubMenuItem4,3);
 SetSubM(SubMenuItem5,4);
 SetSubM(SubMenuItem6,5);
  MainM();
}

void MainM(void)
{
 unsigned key;
 int ID;

 MianMD();

 for(key=0;;)
 {
  key=GetKey();

  switch(key)
  {
   case LEFT:
    SelectMainM(MainMenu.select,0);
    if (MainMenu.select<1)
     MainMenu.select=MainMenu.itemnum-1;
    else MainMenu.select--;
    SelectMainM(MainMenu.select,1);
    break;
   case RIGHT:
    SelectMainM(MainMenu.select,0);
     if (MainMenu.select>MainMenu.itemnum-2)
      MainMenu.select=0;
     else MainMenu.select++;
    SelectMainM(MainMenu.select,1);
    break;
   case DOWN:
   case RETURN:
   switch (MainMenu.COMMAND_ID[MainMenu.select])
   {
    case 5:
     if (key==DOWN)
      break;
     closegraph();
     QuitTo("Thank you for using ULTRA!  "," HAPPY"," 1997!" );
     break;
    default:
     ID=SubM();
     if (ID>=0)
      Functions(ID);
     break;
    }
   }
  }
 }

void Functions(int ID)
{ int i,j;
 switch (ID)
 {
  case 0:
   ClearAll();
   break;
  case 3:
   prns();
   break;
  case 5:
  case 500:
   closegraph();
   QuitTo("Good Job! "," See you"," next time!");
      break;
  case 4:
    SaveVram();
    bar(0,0,getmaxx(),getmaxy());
    gmode=getgraphmode();textmode(C80);
     system("");
    setgraphmode(gmode);
    PutVram();
    break;
  case 1:
  case 2:
  case 100:
  case 101:
  case 102:
  case 103:

   SaveAs(ID);
   if(ID==100)IndicateWin(0);
   else IndicateWin((ID-100)*2-1);
   break;
  case 200:
  case 202:
  Showing("Press Any Key...",GREEN);
   motornum=(ID-200+2)/2;
   Go1();
  Showing(doing,GREEN);
   break;
  case 201:
  case 203:
   motornum=(ID-200+1)/2;
   Showing("Ready to Move Motor...",GREEN);

   MoveM();
   Showing(doing,GREEN);
   break;
  case 205:
  PowerShow();
  break;
  case 300:
  /*durations();    */
  break;
  case 401:
   About();
   break;
  default:
   setfillstyle(SOLID_FILL, AllColors[5]);
   setcolor(0);
   bar3d(WindowMinX, WindowMinY, WindowMidX, WindowMaxY,0,0);
   MessageBox("Under Construction!",0);
   break;
 }
}
void  ClearAll(void)
   {int j;
    /*for(i=0;i<5000;i++)datastream[i]=-1;  */

   setfillstyle(SOLID_FILL,AllColors[0]);
   setcolor(15);
   bar3d(5,3*texth+25,400,getmaxy()-5,0,0);
   bar3d(10,55,395,450,0,0);
   outtextxy(200,460,"Intensity:");
   outtextxy(20,460,"Power:");
   }

int SubM(void)
{
 MENUTYPE *mn=&SubMenu[MainMenu.select];

 unsigned key;

 if (SubMD(MainMenu.select))
  return -2;

  for(key=0;;)
  {
   key=GetKey();

   switch(key)
   {
    case ESC:
     Exitmenu(MainMenu.select);
     return -1;
    case UP:
     SelectSubM(MainMenu.select, mn->select,0);
     if(mn->select>0)
      mn->select--;
     else mn->select=mn->itemnum-1;
      SelectSubM(MainMenu.select, mn->select,1);
     break;
    case DOWN:
     SelectSubM(MainMenu.select, mn->select,0);
     if (mn->select<mn->itemnum-1)
      mn->select++;
     else mn->select=0;
       SelectSubM(MainMenu.select, mn->select,1);
     break;
    case LEFT:
     Exitmenu(MainMenu.select);
     SelectMainM(MainMenu.select,0);
     if (MainMenu.select<1)
      MainMenu.select=MainMenu.itemnum-1;
     else MainMenu.select--;
     SelectMainM(MainMenu.select,1);
     if (SubMD(MainMenu.select))
      return -2;
     mn=&SubMenu[MainMenu.select];
     break;
    case RIGHT:
     Exitmenu(MainMenu.select);
     SelectMainM(MainMenu.select,0);
     if(MainMenu.select>MainMenu.itemnum-2)
      MainMenu.select=0;
     else MainMenu.select++;
     SelectMainM(MainMenu.select,1);
     if (SubMD(MainMenu.select))
      return -2;
     mn=&SubMenu[MainMenu.select];
     break;
    case RETURN:
     Exitmenu(MainMenu.select);
     return mn->COMMAND_ID[mn->select];
   }
  }
 }

void SetMainbM()
{
 int count, totallen, maxlen, i, j=1;

 MainMenu.coor[0]=5;
 MainMenu.coor[1]=25;
 MainMenu.coor[2]=maxx-5;
 MainMenu.coor[3]=3*texth+25;

 getitemcount(MainMenuItem, &count, &totallen, &maxlen);
 MainMenu.itemnum=count;

 maxlen=(maxx/textw-totallen-4)/count;

 for(i=0,j=2;i<count;i++)
 {
  MainMenu.itemcoor[i*4]=j*textw;
  MainMenu.itemcoor[i*4+1]=MainMenu.coor[1];
  MainMenu.itemcoor[i*4+2]=textw*(j+maxlen+strlen(MainMenuItem[i]));
  MainMenu.itemcoor[i*4+3]=MainMenu.coor[3];
  MainMenu.itemdispxy[i*2]=textw*(j+0.5*maxlen);
  MainMenu.itemdispxy[i*2+1]=texth+25;
  MainMenu.COMMAND_ID[i]=i;
  j+=maxlen+strlen(MainMenuItem[i]);
 }
 MainMenu.itemname=MainMenuItem;
}

void SetSubM(char **name, int ord)
{
 int count, i, j, maxlen;

 getitemcount(name, &count, &i, &maxlen);
 SubMenu[ord].itemname=name;
 SubMenu[ord].itemnum=count;
 j=textw*(maxlen+2);
 if(j+MainMenu.itemcoor[4*ord]<maxx)
 {
   SubMenu[ord].coor[0]=MainMenu.itemcoor[4*(ord)];
  SubMenu[ord].coor[2]=SubMenu[ord].coor[0]+j;
 }
 else
 {
  SubMenu[ord].coor[2]=maxx;
  SubMenu[ord].coor[0]=SubMenu[ord].coor[2]-j;
 }
 SubMenu[ord].coor[1]=MainMenu.itemcoor[4*ord+3];
 SubMenu[ord].coor[3]=SubMenu[ord].coor[1]+count*(texth+6);
 for (i=0; i<count; i++)
 {
  SubMenu[ord].itemcoor[i*4]=SubMenu[ord].coor[0];
  SubMenu[ord].itemcoor[i*4+1]=SubMenu[ord].coor[1]+i*(6+texth);
  SubMenu[ord].itemcoor[i*4+2]=SubMenu[ord].coor[2];
  SubMenu[ord].itemcoor[i*4+3]=SubMenu[ord].itemcoor[i*4+1]+texth+6;
  SubMenu[ord].itemdispxy[i*2]=SubMenu[ord].itemcoor[i*4]+textw;
  SubMenu[ord].itemdispxy[i*2+1]=SubMenu[ord].itemcoor[i*4+1]+3;
  SubMenu[ord].COMMAND_ID[i]=ord*100+i;
 }
}

void MianMD(void)
{
 unsigned i;
 setfillstyle(SOLID_FILL,15);
 setcolor(0);
 bar3d(0,0,getmaxx(),getmaxy(),0,0);
 setfillstyle(SOLID_FILL,AllColors[0]);
 bar3d(5,3*texth+25,400,getmaxy()-5,0,0);
 setfillstyle(SOLID_FILL, 7);
 bar3d(MainMenu.coor[0], MainMenu.coor[1], MainMenu.coor[2],MainMenu.coor[3],0,0);
 setfillstyle(SOLID_FILL,AllColors[3]);
 bar3d(WindowMidX+3,WindowMinY+2,WindowMaxX,WindowMidY+45,0,0);
 setfillstyle(SOLID_FILL,BLUE);
 setcolor(0);
 bar3d(5,5,getmaxx()-5,25,0,0);
 setcolor(15);
 outtextxy(150,12,"Ultrashort Laser Pulse measurement System");
 line(5,3*texth+25,maxx-5,texth*3+25);

 InitialInfo();
 ClearAll();
 for(i=0;i<MainMenu.itemnum;i++)
 {
 SelectMainM(i,0);

   if (MainMenu.select==i)
   SelectMainM(i,1);


 }
}

int SubMD(int ord)
{
 unsigned size, i;
 MENUTYPE *mn=&SubMenu[ord];
 size=imagesize(mn->coor[0],mn->coor[1],mn->coor[2],mn->coor[3]);
 if ((MenuImageBuf=(unsigned char *)malloc(size))==NULL)
 {
  clearviewport();
  outtextxy(100,100,"Out of memroy!");
  outtextxy(100,110,"Press any key...");
  getch();
  return 1;
 }
getimage(mn->coor[0],mn->coor[1],mn->coor[2],mn->coor[3],MenuImageBuf);

setcolor(AllColors[MenuEdge]);
setfillstyle(SOLID_FILL, AllColors[MenuInGround]);
bar3d(mn->coor[0],mn->coor[1],mn->coor[2],mn->coor[3],0,0);
setcolor(AllColors[0]);
bar3d(mn->coor[0],mn->coor[1],mn->coor[2],mn->coor[3],0,0);

for(i=0;i<mn->itemnum;i++)
{
 if(i==mn->select)
  SelectSubM(ord,i,1);
 else
 {
  setcolor(AllColors[MenuInItem]);
  outtextxy(mn->itemdispxy[i*2],mn->itemdispxy[i*2+1],mn->itemname[i]);
 }
}
return 0;
}

static void Exitmenu(int ord)
{
 MENUTYPE *mn=&SubMenu[ord];
 if(MenuImageBuf)
 {
  putimage(mn->coor[0],mn->coor[1],MenuImageBuf,COPY_PUT);
  farfree(MenuImageBuf);
 }
}

void SelectMainM(int select, char In_Active)
{
 MENUTYPE *tp=&MainMenu;
 int start,end,bot,top;

 start=tp->itemcoor[select*4]+8;
 top=tp->itemcoor[select*4+1]+2;
 end=tp->itemcoor[select*4+2]-8;
 bot=tp->itemcoor[select*4+3]-2;
 if (In_Active==0)
 {
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  setcolor(15);
  bar3d(start,top,end,bot,0,0);
  setcolor(0);
  line(start,bot,end,bot);
  line(end,top,end,bot);
  setcolor(AllColors[MenuInItem]);
  outtextxy(tp->itemdispxy[select*2],tp->itemdispxy[select*2+1],tp->itemname
[select]);
 }
 else
 {
  setfillstyle(SOLID_FILL,CYAN); /*AllColors6[MenuActiveGround]);*/
  bar(start,top,end,bot);
  setcolor(0);
  line(end,top,start,top);
  line(start,top,start,bot);
  setcolor(AllColors[MenuActiveItem]);
  outtextxy(tp->itemdispxy[select*2]+1,tp->itemdispxy[select*2+1]+1,tp->itemname
[select]);
 }
}

void SelectSubM(int ord,int select,char In_Active)
{
 MENUTYPE *tp=&SubMenu[ord];
 int start,end,bot,top;

 start=tp->itemcoor[select*4]+2;
 top=tp->itemcoor[select*4+1]+1;
 end=tp->itemcoor[select*4+2]-2;
 bot=tp->itemcoor[select*4+3]-1;
 if(In_Active==0)
 {
  setfillstyle(SOLID_FILL,AllColors[MenuInGround]);
  bar(start,top,end,bot);
  setcolor(AllColors[MenuInItem]);
  outtextxy(tp->itemdispxy[select*2],tp->itemdispxy[select*2+1],tp->itemname
[select]);
 }
 else
 {
  setfillstyle(SOLID_FILL,AllColors[MenuActiveGround]);
  bar(start,top,end,bot);
  setcolor(0);
  line(start,top,end,top);
  line(start,top,start,bot);
  setcolor(AllColors[MenuActiveItem]);
  outtextxy(tp->itemdispxy[select*2],tp->itemdispxy[select*2+1],tp->itemname
[select]);
 }
}

void InitialGraphics(void)
{
 int driver=VGA, mode=VGAHI;
 int  errorcode;

 registerbgidriver(EGAVGA_driver);
 initgraph(&driver,&mode,"");
 errorcode=graphresult();
 if(errorcode!=grOk)
 {
  printf("Graphics error:%s\n",grapherrormsg(errorcode));
  printf("Press any key to halt:");
  getch();
  QuitTo("Graphics error.","Try again!","Good Luck!");
 }
}

int GetKey(void)
{
 int key;

 key=bioskey(0);
 if(key==F1)
  prns();
 else if (key<<8)
 {
    key=key&0x00ff;
  if (isalpha(key))
   key=toupper(key);
 }
 return key;
}

static void getitemcount (char **s,int *count,int *len,int *maxlen)
{
 int i,j;

 for((*maxlen)=0,(*len)=0,i=0;s[i]!=NULL;i++)
 {
  j=strlen(s[i]);
  (*len)+=j;
  if(j>(*maxlen)) (*maxlen)=j;
 }
 (*count)=i;
}

void Draw(void)
{
 int x,x1,x2,y1,y2,ch;

 randomize();


  x1=random(WindowMidX-WindowMinX)+WindowMinX;
  y1=random(WindowMaxY-WindowMinY)+WindowMinY;
  x2=random(WindowMidX-WindowMinX)+WindowMinX;
  y2=random(WindowMaxY-WindowMinY)+WindowMinY;
  ch=random(16);
  setfillstyle(SOLID_FILL,ch);
  if (x1>x2)
  {
   x=x1;x1=x2;x2=x;
  }
  if (y1>y2)
  {
   x=y1;y1=y2;y2=x;
  }
  bar(x1,y1,x2,y2);
  delay(1);

 }

void SaveAs(int ID)
{
 int MidX, MidY, x1,y1,x2,y2,size,Len,stepi,key2,stlen[]={0,12,26,0,6,6,6,6,6,6,6};
 float tempsetupp=0;
 void *Buffer;
 static char *fname="amsup",*dfname="",*dname="d:\\user\\hfr\\";
 int l1,l2;


 MidX=(WindowMaxX-WindowMinX)/2;
 MidY=(WindowMaxY-WindowMinY)/2;
 if (ID<10)Len=8*(strlen(message[ID])+1)/2;
 else Len=8*(strlen(setuptext[ID-100][0])+1)/2;
 x1=MidX-Len-5*8;x2=MidX+Len+5*8;y1=MidY-100;y2=MidY;
 size=imagesize(x1,y1,x2,y2);
 if ((Buffer=malloc(size))==NULL)
  {
    outtextxy(100,100,"Out of memorry! Press any key to continue.");
    getch();
  }
 else
  {
    getimage(x1,y1,x2,y2,Buffer);
    setfillstyle(SOLID_FILL,LIGHTRED);
    setcolor(0);
    bar3d(x1,y1,x2-13,y2-13,0,0);
    setfillstyle(SOLID_FILL,BLACK);
    bar(x1+12,y2-12,x2,y2);
    bar(x2-12,y1+12,x2,y2);

    setfillstyle(SOLID_FILL,WHITE);
    setcolor(0);
    bar3d(x1+45,y2-60,x1+45+(stlen[ID&0x0f]+1)*textw,y2-45,0,0);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    if (ID<10)outtextxy(MidX, MidY-75, message[ID]);
    else {tempsetupp=setupp[ID-100];setupp[ID-100]=0;
	  outtextxy(MidX,MidY-75,setuptext[ID-100][0]);
	  outtextxy(x1+45+(stlen[ID&0x0f]+2)*textw,y2-50,setuptext[ID-100][1]);
		 }
     for(stepi=0;;)
      {
      key2=bioskey(0);
	    if (key2<<8)key2=key2&0x00ff;
      if(key2==ESC)break;
			      else
	{
	  if(key2==RETURN)
	   { if(ID==2)*(dname+stepi)='\0';
		else *(fname+stepi)='\0';
		tempsetupp=0;	break;
	   }
	 if(stepi>0&&key2==BS)
	  {
		stepi--;
		setfillstyle(SOLID_FILL,15);
		bar(x1+52+stepi*textw-2,155,x1+52+(stepi+1)*textw-2,165);
	  if (ID>99)setupp[ID-100]=(int)setupp[ID-100]/10;
	  }
	 else if(((key2>47&&key2<58||key2==0x002e)||(ID<10&&isalpha(key2))
		   ||key2==0x003a||key2==0x005c) &&(stepi<stlen[ID&0x0f]))
	  { if(ID<10)
	    { key2=toupper(key2);

	      if(ID==2)*(dname+stepi)=key2;
	      else *(fname+stepi)=key2;

	     }
	      else
	      {
	       setupp[ID-100]=setupp[ID-100]*10+key2-48;
	      }
	       stepi++;
	       outtextxy(x1+45+stepi*textw,160,&key2);
	  }
       }
     } /* end for*/
	/*printf("%d",setupp[ID-100]);    */
    if (!(key2==ESC)&&ID==1)
    {    l1=strlen(dname);
	 l2=strlen(fname);
	 for(i=0;i<l1;i++)
	*(dfname+i)=*(dname+i);
	for(i=0;i<l2;i++)
	*(dfname+l1+i)=*(fname+i);
	*(dfname+l1+l2)='\0';

	 SaveData(dfname);
	/* printf("%s",dfname);   */
	 fname="ultra.dat";
	 dname="d:\\";
	 dfname="d:\\ultra.dat";
	 SavePict(dfname);

   }

  if(ID>99)setupp[ID-100]+=tempsetupp;
  putimage(x1,y1,Buffer,COPY_PUT);
  free(Buffer);
  settextjustify(LEFT_TEXT,TOP_TEXT);




   }
}


void SaveData(char *filename)
{
FILE *fpd;
int i;
char *fn;



if(!(fpd=fopen(filename,"w")))
 {
  outtextxy(270,180,"       Cannot Open This File...");
  getch();
  return;
 }
 else
{
 for (i=1;i<dissteps;i++)
  {
   fprintf(fpd,"%f",i*setupp[0]/Mindex[motornum]);
   putc(' ',fpd);
    fprintf(fpd,"%d",datastream[i]);
   putc('\n',fpd);
 } }
 fclose(fpd);
 return;
}

void SavePict(char *fname)
{
/*FILE *fpd
if(!(fpd=fopen(strcat(fname,".fig"),"w")))
 {
  outtextxy(180+textw,190,"Cannot Open This File...");
  return;
 }
else
{for (i=1;datasream[i]=-1;i++)
  {
   putc(datastream[i],fpd);
   putc(" ",fpd);
  }
 }
 fclose(fpd);
*/ return;}

void About(void)
{
 int MidX, MidY, x1, y1, x2, y2, size;
 void *Buffer;

 MidX=(WindowMidX-WindowMinX)/2;
 MidY=(WindowMaxY-WindowMinY)/2;

 x1=MidX-90; x2=MidX+90; y1=MidY-100; y2=MidY+100;
 size=imagesize(x1,y1,x2,y2);
 setfillstyle(SOLID_FILL,0);
 bar(WindowMinX+1,WindowMinY+1,WindowMidX-1,WindowMaxY-1);
 if ((Buffer=malloc(size))==NULL)
 {
  outtextxy(100,100,"Out of memorry! Press any key to continue.");
  getch();
 }
  else
 {
  getimage(x1,y1,x2,y2,Buffer);
  setfillstyle(SOLID_FILL,BLUE);
  bar(x1,y1,x2-9,y2-9);
  setfillstyle(SOLID_FILL,BLACK);
  bar(x1+8,y2-8,x2,y2);
  bar(x2-8,y1+8,x2,y2);
  setcolor(WHITE);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  outtextxy(MidX-4,MidY-40,"ULTRA Menu");
  outtextxy(MidX-4,MidY-20,"Now you see!");
  outtextxy(MidX-4,MidY+25,"By Feiran Huang");
  outtextxy(MidX-4,MidY+45,"In 3.8, 1997 ");
  getch();

  putimage(x1,y1,Buffer,COPY_PUT);
  free(Buffer);
  settextjustify(LEFT_TEXT,TOP_TEXT);
 }
}

void SaveVram(void)
{
  FILE *fp;
  int i;
  register long j;
  char far *ptr;
  fp=fopen("d:\\temp01.dat","wb");
  for(i=0;i<4;i++)
   {
    outportb(0x3ce,4);
    outportb(0x3cf,i);
    ptr=(char far *)0xa0000000l;
    for(j=0;j<38400l;j++)
     {putc(*ptr,fp);
      ptr++;
     }
   }
 fclose(fp);
 outportb(0x3cf,0);
}

void PutVram(void)
{
  FILE *fp;
  int k=1;
  register int i;
  register long j;
  char far *ptr;
  fp=fopen("d:\\temp01.dat","rb");
  for(i=0;i<4;i++)
   {
    outportb(0x3c4,2);
    outportb(0x3c5,k);
    ptr=(char far*)0xa0000000l;
    for(j=0;j<38400l;j++)
     {
      *ptr=getc(fp);
      ptr++;
     }

   k*=2;
  }
  fclose(fp);
  outportb(0x3c5,0xf);
}


void MessageBox(char *Message,int sign)
{
 int MidX, MidY, x1,y1,x2,y2,size,Len;
 void *Buffer;

 MidX=(WindowMaxX-WindowMinX)/2;
 MidY=(WindowMaxY-WindowMinY)/2;
 Len=8*(strlen(Message)+1)/2;
 x1=MidX-Len-5*8;x2=MidX+Len+5*8;y1=MidY-100;y2=MidY;
 size=imagesize(x1,y1,x2,y2);
 if ((Buffer=malloc(size))==NULL)
 {
  outtextxy(100,100,"Out of memorry! Press any key to continue.");
  getch();
 }
 else
 {
  getimage(x1,y1,x2,y2,Buffer);
  setfillstyle(SOLID_FILL,LIGHTGREEN);
  setcolor(0);
  bar3d(x1,y1,x2-13,y2-13,0,0);
  setfillstyle(SOLID_FILL,BLACK);
  bar(x1+12,y2-12,x2,y2);
  bar(x2-12,y1+12,x2,y2);
  setcolor(0);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  outtextxy(MidX, MidY-70, Message);
  if(sign==0)
  {
  setfillstyle(SOLID_FILL,AllColors[5]);
  setcolor(0);
  bar3d(MidX-3*8,MidY-34,MidX+3*8,MidY-16,0,0);
  outtextxy(MidX,MidY-24,"OK");
  }

  getch();
  putimage(x1,y1,Buffer,COPY_PUT);
  free(Buffer);
  settextjustify(LEFT_TEXT,TOP_TEXT);
 }
}

void Go1(void)
{long i,j,n=1,key4,currentstep=0,stop=0;

 setcolor(RED);
  dtemp=-1;

 motornum=1;
  ClearAll();
  SetDuration();
   DataLong=Mindex[motornum]*setupp[motornum+1]/setupp[0] ;
  for(;;)
  {           /*  	ClearAll();   */
  setfillstyle(SOLID_FILL,YELLOW);
     Showing("Measuring pulses duration",RED);
     bar(WindowMidX+10,WindowMinY+5,WindowMaxX-5,WindowMinY+55);
     MotorInitial();
   /* printf("%d,%f,%d",setupp[motornum+1],Mindex[motornum],setupp[0]);
   */

   dissteps=(long)(setupp[motornum+1]*Mindex[motornum]/setupp[0]);
   for(i=0;i<dissteps;i++)
    { if(!bioskey(1)==0){if(GetKey()==ESC){break;}}
    for(j=1;j<=setupp[0];j++){MotorStep(1);}
     delay(10*setupp[1]);

    steps[motornum]+=setupp[0];
    IndicateWin(motornum*2);

    datastream[i]=GetData()/n+datastream[i]*(n-1)/n;
  DisplayData(i);


    }
   stop=i;
   n++;
  setfillstyle(SOLID_FILL,YELLOW);
  bar(WindowMidX+10,WindowMinY+55,WindowMaxX-5,WindowMinY+75);


  setcolor(BLUE);

  outtextxy(WindowMidX+10,WindowMinY+20,"Press RETURN to Move Back");
  outtextxy(WindowMidX+10,WindowMinY+40,"      ESC    to Quit    ");

   for(;;){
   key4=GetKey();
   if(key4==ESC||key4==RETURN)break;
	   }

  if(key4==ESC)break;
   setfillstyle(SOLID_FILL,YELLOW);
 bar(WindowMidX+10,WindowMinY+5,WindowMaxX-5,WindowMinY+55);
  Showing("Moving Back...",RED);
  currentstep=setupp[motornum+1]*Mindex[motornum];

  for(i=stop-1;i>=0;i--)
      {for(j=1;j<=setupp[0];j++){MotorStep(0);}
       delay(5);

       steps[motornum]-=setupp[0];
       IndicateWin(motornum*2);

       }

  setcolor(BLUE);
  outtextxy(WindowMidX+10,WindowMinY+20,"Press RETURN to Continue");
  outtextxy(WindowMidX+10,WindowMinY+40,"      ESC key to Quit    ");
  setfillstyle(SOLID_FILL,YELLOW);
  bar(WindowMidX+10,WindowMinY+55,WindowMaxX-5,WindowMinY+75);

 dtemp=-1;
  for(;;)
  {key4=GetKey();
   if(key4==ESC||key4==RETURN)break;
   }
  if(key4==ESC)break;

  }


  bar(WindowMidX+10,WindowMinY+5,WindowMaxX-5,WindowMinY+55);
 setfillstyle(SOLID_FILL,0);
bar(288,460,320,470);

}

void Go2(void)
{}

void MoveM(void)
{
 int i;

 setfillstyle(SOLID_FILL,LIGHTGRAY);
 setcolor(0);
 for(i=1;i<(WindowMaxX-WindowMidX)/2;i+=2)
 bar3d(633-115+i,WindowMidY+50,633-115-i,WindowMaxY,0,0);
 setcolor(0);
 outtextxy(WindowMidX+10,WindowMidY+70,"* Press Keys to      ");
 outtextxy(WindowMidX+10,WindowMidY+80,"  Move or Stop the Motor");
 outtextxy(WindowMidX+10,WindowMidY+147,"*");
 outtextxy(WindowMidX+10,WindowMidY+167,"  back to starting position");
 outtextxy(WindowMidX+10,WindowMidY+207,"*");
 ControlWin();
 setfillstyle(SOLID_FILL,LIGHTGREEN);
 setcolor(0);
 setfillstyle(SOLID_FILL,WHITE);
 for(i=WindowMidY+50;i<WindowMaxY;i+=4)
 bar3d(WindowMidX+3,WindowMidY+50,WindowMaxX,i,0,0);
 outtextxy(WindowMidX+40,WindowMidY+100,"Select ");
 setcolor(RED);
 outtextxy(WindowMidX+90,WindowMidY+100,"Measure. ");
 outtextxy(WindowMidX+90,WindowMidY+120,"Move");
 setcolor(0);
 outtextxy(WindowMidX+40,WindowMidY+140,"to Open This Windows");
 outtextxy(WindowMidX+40,WindowMidY+160,"and Move The Motor.");
 return;
}


void ControlWin(void)
{
  int i=0,j,key3,C_ID,horo=1,viter=0;
  Keyselect(horo,viter);
  for(;;)
  {if(kbhit())
   {
    key3=GetKey();
    switch(key3)
    {
      case LEFT:
       if(viter==0&&horo>0) horo--;
       break;
      case RIGHT:
       if(viter==0&&horo<2)horo++;
       break;
      case UP:
       if(viter>0)
	{horo=1;viter--;}
       break;
      case DOWN:
       if(viter<2)
	{horo=1;viter++;}
       break;
      case RETURN:
       if(viter>0)horo=0;
       break;
      }
    C_ID=viter*10+horo;
    Keyselect(horo,viter);
    /*if(C_ID==10)MoveBack();*/
   if(C_ID==0||C_ID==2)Showing("Motor Now Moving",BLUE);
   else Showing("Motor Now Stop",GREEN);
   } /*end if*/
   if(steps[motornum]>0||C_ID>0)
   switch(C_ID)
    {
     case 0:
      steps[motornum]-=setupp[0];
      IndicateWin(motornum*2);
      for(j=1;j<=setupp[0];j++)MotorStep(0);



      break;

     case 2:
      steps[motornum]+=setupp[0];
    IndicateWin(motornum*2);
      for(j=1;j<=setupp[0];j++)MotorStep(1);



      break;
     }
   if(C_ID==20||key3==ESC)break;
  }/*end for*/
}



void Keyselect(int horo,int viter)
{int i,j,colors;
char *KeyItem[]={" Left"," Stop","Right"};
char *KeyItemv[][2]={0,0,"Press Enter Key to","Move Mirror",
			 "Press Enter to Exit","Finished & Exit"};

 for(i=0;i<3;i++)
  {if (i==horo&&viter==0)
   { if (i==1)colors=12;
     else colors=2;
     setfillstyle(SOLID_FILL,colors);
     setcolor(15);
     bar(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+i*70+65,WindowMidY+120);
     outtextxy(WindowMidX+i*70+30,WindowMidY+107,KeyItem[i]);
     setcolor(0);
     line(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+i*70+65,WindowMidY+100);
     line(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+i*70,WindowMidY+120);
   }
  else
   { setfillstyle(SOLID_FILL,LIGHTGRAY);
     setcolor(0);
     bar(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+65+i*70,WindowMidY+120);
     outtextxy(WindowMidX+i*70+30,WindowMidY+107,KeyItem[i]);
     setcolor(15);
     line(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+i*70+65,WindowMidY+100);
     line(WindowMidX+20+i*70,WindowMidY+100,WindowMidX+20+i*70,WindowMidY+120);
     setcolor(0);
     line(WindowMidX+20+i*70+65,WindowMidY+100,WindowMidX+20+i*70+65,WindowMidY+120);
     line(WindowMidX+20+i*70,WindowMidY+120,WindowMidX+20+i*70+65,WindowMidY+120);
   }
 }

  for(j=1;j<3;j++)
  {if (j==viter)
   { setfillstyle(SOLID_FILL,BLUE);
     setcolor(15);
     bar(WindowMidX+20,WindowMidY+j*60+80,WindowMidX+20+170,WindowMidY+j*60+100);
     outtextxy(WindowMidX+30,WindowMidY+j*60+87,KeyItemv[j][0]);
     setcolor(0);
     line(WindowMidX+20,WindowMidY+j*60+80,WindowMidX+20,WindowMidY+j*60+100);
     line(WindowMidX+20+170,WindowMidY+j*60+80,WindowMidX+20,WindowMidY+j*60+80);
   }
  else
   { setfillstyle(SOLID_FILL,LIGHTGRAY);
     setcolor(0);
     bar(WindowMidX+20,WindowMidY+j*60+80,WindowMidX+20+170,WindowMidY+j*60+100);
     outtextxy(WindowMidX+30,WindowMidY+j*60+87,KeyItemv[j][1]);
     setcolor(15);
     line(WindowMidX+20,WindowMidY+j*60+80,WindowMidX+20+170,WindowMidY+j*60+80);
     line(WindowMidX+20,WindowMidY+j*60+80,WindowMidX+20,WindowMidY+j*60+100);
     setcolor(0);
     line(WindowMidX+20+170,WindowMidY+j*60+80,WindowMidX+20+170,WindowMidY+j*60+100);
     line(WindowMidX+20,WindowMidY+j*60+100,WindowMidX+20+170,WindowMidY+j*60+100);
     }
   }

 }


int GetData(void)
{int x,a,b;

  outportb(0x100,0);
    outportb(0x101,0);
    for(x=0;x<100;x++){}
    a=inportb(0x102)*16;
    b=(inportb(0x103)&0xf0)/16;
    x=b+a;
    x-=2047;

    return(x);



 }

void DisplayData(int x)
{
float x0=x/DataLong*360+20;
 int y0=250;
 line(10,250,396,250);
setcolor(0);   /* printf(" %d,%f ",x/,DataLong);  */

if (!(x0==x0temp))line(x0,y0-190,x0,y0+198);
setcolor(LIGHTGRAY);
putpixel(x0,y0-datastream[x]/15,LIGHTGRAY);
if (x>1)line(x0temp,y0-dtemp,x0,y0-datastream[x]/15);
(int)dtemp=datastream[x]/15;
x0temp=x0;
setfillstyle(SOLID_FILL,0);
bar(288,460,320,470);
setcolor(LIGHTRED);
outtextxy(290,460,ToString(dtemp));

}

void MotorInitial(void)
{outportb(0x107,0x80);
 outportb(0x104,0);}

void MotorStep(int direction)
{    int i=0,speed;
  stat=0+2*direction;
  outportb(0x104,stat);
  delay(10);
 stat=1+2*direction;
   outportb(0x104,stat);
  delay(10);
   stat=0+2*direction;
   outportb(0x104,stat);
  delay(10);


}
void InitialInfo(void)
{int i;
 setcolor(0);
 outtextxy(WindowMidX+10,WindowMidY-100,"        Resolution:");
 outtextxy(WindowMidX+10,WindowMidY-80 ,"   Measuring Time:");
 outtextxy(WindowMidX+10,WindowMidY-60, "  Current Position:");
 outtextxy(WindowMidX+10,WindowMidY-40, "   Measure Length:");
 outtextxy(WindowMidX+10,WindowMidY-20, "Current Wavelength:");
 outtextxy(WindowMidX+10,WindowMidY,    "  Total Wavelength:");
 for(i=0;i<6;i++)IndicateWin(i);
 motornum=1;IndicateWin(2);
 motornum=2;IndicateWin(4);
 motornum=0;
}

void IndicateWin(int number)
{long tmp;
setfillstyle(SOLID_FILL,YELLOW);

 bar(WindowMidX+170,WindowMidY-100+number*20,WindowMaxX-10,WindowMidY-100+number*20+15);
 setcolor(CYAN);
 switch(number)
 case 0:
 {outtextxy(WindowMidX+165,WindowMidY-100,strcat(ToString(setupp[0]),"*.28fs"));
 break;
 case 1:
 outtextxy(WindowMidX+170,WindowMidY-80,strcat(ToString(setupp[1])," ms"));
 break;
 case 2:
 outtextxy(WindowMidX+170,WindowMidY-60,strcat(ToString((long)tmp=steps[motornum]/Mindex[1])," fs"));
 break;
 case 3:
 outtextxy(WindowMidX+170,WindowMidY-40,strcat(ToString(setupp[2])," fs"));
 break;
 case 4:
 outtextxy(WindowMidX+170,WindowMidY-20,strcat(ToString((int)tmp=steps[motornum]/Mindex[2])," nm"));
 break;
 case 5:
 outtextxy(WindowMidX+170,WindowMidY-0,strcat(ToString(setupp[3])," nm"));
 break;
 }
}
void PowerShow(void)
{ setcolor(0);
  outtextxy(WindowMidX+30,WindowMinY+30,"Press Esc to Stop");
  Showing("Indicating Power...",RED);
  setfillstyle(SOLID_FILL,0);
  setcolor(YELLOW);
  for(;;)
  {if(kbhit())if(GetKey()==ESC)break;

   bar(70,460,100,470);
   outtextxy(70,460,ToString(GetData()));
   delay(10);
}
  setfillstyle(SOLID_FILL,YELLOW);
  bar(WindowMidX+30,WindowMinY+20,WindowMaxX-10,WindowMinY+50);
  Showing(doing,GREEN);
 setfillstyle(SOLID_FILL,0);
bar(70,460,90,470);

}

void Showing(char *showtext,int textcolor)
{ setfillstyle(SOLID_FILL,YELLOW);
 bar(WindowMidX+10,WindowMinY+55,WindowMaxX-5,WindowMinY+75);
   setcolor(textcolor);
   outtextxy(WindowMidX+20,WindowMinY+60,showtext);
   }

 char *ToString(int x0)
 {int x1,x=x0,i=0;
  char j;
  static char *string1;
  if(x0<0)x=-x;
  for(;;)
  {
   int x1=x/10;
   *(string1+i)=x-x1*10+48;
   i++;
   x=x1;
   if(x==0)break;
   }
  if(x0<0)
  {
  *(string1+i)='-';
  i++;
  }
  *(string1+i)='\0';

  for(x=0;x<=i/2-1;x++)
  {
    j=*(string1+x);
    *(string1+x)=*(string1+i-1-x);
    *(string1+i-1-x)=j;
  }

  return(string1);

 }

void QuitTo(char *pcInf1,char *pcInf2,char *pcInf3)
{
 int f;

 clrscr();
 window(1,1,80,1);
 textbackground(LIGHTBLUE);
 clrscr();

 textcolor(WHITE);
 cprintf(pcInf1);
 textcolor(YELLOW);
 cprintf(pcInf2);
 textcolor(LIGHTRED);
 cprintf(pcInf3);
 window(1,1,80,25);
 printf("\n");
 for(f=400;f<400;f+=100)
  {
    sound(f);
    delay(100);
   }
 nosound();
 exit(0);
}

void durations(void)
{/*int dataa[300],total=1000;
float equv=.5,dr;
 int k=0,i,j,fw=0,datamax=0,dur=0;


 for(i=0;i<total/8;i++)dataa[i]=0;

 for(i=0;i<total;i+=8)
   {for(j=i;j<i+8;j++)dataa[k]+=datastream[j];
    dataa[k]/=8;

    if (dataa[k]>datamax)datamax=dataa[k];
    k++ ;
  }
  fw=datamax*equv;

  for(i=0;i<k;i++)
  {if((dataa[i]-fw)*(dataa[i+1]-fw)<=0)
  {dur=(fw-dataa[i])/(dataa[i+1]-dataa[i])+i;
  break;
  }}
  for(i=k-2;i>0;i--)
  {if((dataa[i]-fw)*(dataa[i+1]-fw)<=0)
  {dur=(fw-dataa[i])/(dataa[i+1]-dataa[i])+i-dur;
  break;}}  */
  /*dr=dur*8*.28*setupp[0];
    */
   setcolor(WHITE);
  /* outtextxy(WindowMidX-100,WindowMinY+20,strcat(ToString(dr),"fs"));
    */
  }

void  SetDuration(void)
{

}







