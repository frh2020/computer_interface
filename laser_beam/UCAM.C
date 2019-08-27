			
#include "windows.h" 
#include "dos.h"
#include "UCam.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "conio.h"
#include "math.h"
#include "commdlg.h" 
#include "string.h"
//#include "fcntl.h"    
#include "mmsystem.h"
//#include "sys\types.h"
//#include "sys\stat.h"
//#include "io.h"
                                                      
HANDLE hInst;

HWND hMCIVideo;             /* video window handle      */
WORD wDeviceID;             /* Device ID                */
int  nCardType;             /* Card type, VB SE or VBSE100:2, VB FS200:3 */

BOOL fFreeze=FALSE,firsttime=TRUE;  

     char szMciString[50], temp[32];

/* String for file name and type */
char FileName[50],PathName[100]; 
char *ExtName[]={  "DAT FILE",
                      "BMP 24-bit",
                      };   
                      
int DataId=1;                      

/* for Save/Open file dialogue box */
//OPENFILENAME ofn;
/* file type filters */
//char szFilterSpec[]= "DIB 24-bit(*.BMP)\0*.BMP\0Targa 24-bit(*.TGA)\0*.TGA\0TIFF 24-bit(*.TIF)\0*.TIF\0IBM mmotion(*.MMP)\0*.MMP\0PCX 8-bit(*.PCX)\0*.PCX\0GIF 8-bit(*.GIF)\0*.GIF\0JPEG(*.JPG)\0*.JPG\00";  

       

     static HWND hCamBox, hDrawBox, hCamera, hNew,hExit,hRealtime,
                hRbox,hRyinteg,hRxinteg,hR3d,
                hEffbox,hYinteg,hXinteg,h3d,
                hTrans,hTranstext,hTransy,hLongi,hLongitext,hLongix,
                hFlbox,hPathtext,hPath,hFiletext,hFilename,hExttext,hExtname,
                hSave,hOpen    ;
//measrue value
static int *nImgDat;                           

//open file 
//static char lpFilt[]="DATA File(*.dat)\0*.dat\0BMP Files(*.bmp)\0*.*\0";
                                     

static int xTV=30,  yTV=30, xDw=30,yDw=540;

int nWorking=0,SaveStat=0,CamStat=0; NewImg=0; s3dsight=1;
   
FILE hFile;              

                                                           
   HGLOBAL handle;

//char filename[10],pathname[20];   


char *lp;
char lpbuff[29];
long lpcount;

static HDC hdcpen,hdcpenx1,hdcpenx2,hdcpenx21;
PAINTSTRUCT ps;
static HBRUSH hBrsh1,hBrsh2,hBrshx1,hBrshx2;
static HPEN hPen1,hPen2,hPenx2,hPenx1,hPenx3;           

unsigned long yIn[320],yIn0[320],xIn[240],xIn0[240],xyIn[320];
long yInTemp,xInTemp,xyInTemp;



/*
static char *RepeatItem[]={"50","100","200","500"};  //measure selection (ms)
static char *PointsItem[]={"20","50","100","200"};
static int nRepeatId=1,nPointsId=1;             //Item number
static int nRepeatVl=100,nPointsVl=20;           //Item value
static int nAverage;

static int nWorking=0,nFileStat=0,nScale=0;              //computer status

int Peaks[3][2],PeakThd=100,PeakNumber=3, IccdData[2260],  IccdNumber=2160;
float xIccdData[2260],kp[3]={0,1,0};
long kpint[3];
int PulseProfile=0;
static int Lamda0=800,cpumf=200;;           


int stb=0x37a,com=0x378,dat=0x379;      
int int_time=2;

int high255=0, high2552=0,allowstop=1;

int trace=0;
   */
   
   
                            
/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

    PURPOSE: calls initialization function, processes message loop

****************************************************************************/

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;
HANDLE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
{
    MSG msg; 
    //hInst=hInstance;

    if (!hPrevInstance)
       if (!InitApplication(hInstance))
          return (FALSE);

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);
    
    while (GetMessage(&msg, NULL, NULL, NULL)) 
      
       {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
       }
     
    return (msg.wParam);
}


/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

****************************************************************************/

BOOL InitApplication(hInstance)
HANDLE hInstance;
{
    WNDCLASS  wc;

    wc.style = CS_DBLCLKS;          /* double-click messages */
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = COLOR_WINDOW+1;
    wc.lpszMenuName = 0;
    wc.lpszClassName = "InputWClass";

    return (RegisterClass(&wc));
}


/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

****************************************************************************/

BOOL InitInstance(hInstance, nCmdShow)
HANDLE          hInstance;
int             nCmdShow;
{
    HWND            hWnd;
 

    hInst = hInstance;

    hWnd = CreateWindow(
        "InputWClass",
        "中山大学超快速激光光谱学国家重点实验室光信号测量系统",
        WS_OVERLAPPEDWINDOW ,
        100,20,
        650,
        560,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
        return (FALSE);

      
  //  if (!TranslateAccelerator(hWnd,LoadAccelerators(hInst,"ULTRAMACC"),&msg))                  
  //      return (FALSE);
        
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);

}


/****************************************************************************

    FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)

****************************************************************************/

long FAR PASCAL __export MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
UINT message;
WPARAM wParam;
LPARAM lParam;
{   

 
   
   
    //char Indicate[][]={"Time","Points","Signal","Reference","Aver Ref"};
    int i,j,k,x,y;
 /*   int xScale;
    int k=0;
    double x1=12.0,x2=380.0;
    int Temp[3];
    static FARPROC lpProcAbout;
    static HANDLE hInst;
    HMENU  hMenu; 
    
    FILE  *hFileS,*hFileC; 
    
    char StringScale[20];
    BOOL   bSet;
    FARPROC fpTime;
    
    int mx,my;
    static HCURSOR  hCur;
    static HANDLE hInst1;
    HDC hdc;
   */ 
    //new
    
static int xButton=400,yButton=10, lButton=80,wButton=30;    
    
    switch (message)
     {
       case WM_PAINT:
         hdcpen=BeginPaint(hWnd,&ps);
        
    
         
            
            hPen1=CreatePen(PS_SOLID,2,RGB(255,0,0));
            hPen2=CreatePen(PS_SOLID,2,RGB(255,255,255));
            hBrsh1=CreateSolidBrush(RGB(0,0,0));
            SelectObject(hdcpen,hBrsh1);
          //  Rectangle(hdcpen,8,8,412,377);            
          //  Rectangle(hdcpen,418,8,622,212);            
           
            
         //   Rectangle(hdcpen,50,10,550,410);            
                                                 
                                                 
          // DRAW UNIT                                       
                                                 
                                                 
           /* for(j=0;j<11;j++)
            { MoveTo(hdcpen,50,j*40+10);
             LineTo(hdcpen,45,j*40+10);
                   _itoa(j*40,StringScale,10);
                   
            TextOut(hdcpen,20,400-j*40,StringScale,strlen(StringScale));                   MoveTo(hdcpen,j*50+50,409);
             LineTo(hdcpen,j*50+50,409);
             }
             
          if(nScale==3)
          {  for (i=0;i<11;i+=2)
            {
             xScale=kp[0]+kp[1]*(i*200)+kp[2]*i*i*4000;
             _itoa(xScale,StringScale,10);
               TextOut(hdcpen,i*50+40,410,StringScale,strlen(StringScale));
             }
          } 
                 */
               DeleteObject(hPen1); 
                  
                          
                          
             
           EndPaint(hdcpen,&ps);
       
         break;                     

       case WM_CREATE:
          
          hCamBox=CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,19,9,322,242,hWnd,1,hInst,0); 
           
          hDrawBox=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|
          WS_BORDER,19,259,322,262,hWnd,2,hInst,0); 
         
          hRbox=CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|
          BS_GROUPBOX,xButton-10,yButton-10,lButton+20,(10+wButton)*4+10,hWnd,3,hInst,0); 
         
         hNew=CreateWindow("BUTTON","校正",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,yButton+210,lButton,wButton,hWnd,4,hInst,0); 
          
            hEffbox=CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|
          BS_GROUPBOX,xButton+100,yButton-10,lButton+20,(10+wButton)*7+10,hWnd,5,hInst,0); 
        
          hCamera=CreateWindow("BUTTON","拍摄",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,yButton+170,lButton,wButton,hWnd,6,hInst,0); 
          
     
          
          hRealtime=CreateWindow("BUTTON","实时",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,130,lButton,wButton,hWnd,7,hInst,0); 
          
          hRyinteg=CreateWindow("BUTTON","纵积分",WS_CHILD|WS_VISIBLE|
          BS_AUTORADIOBUTTON,xButton,10,lButton,wButton,hWnd,8,hInst,0); 
          
          hRxinteg=CreateWindow("BUTTON","横积分",WS_CHILD|WS_VISIBLE|
          BS_AUTORADIOBUTTON,xButton,50,lButton,wButton,hWnd,9,hInst,0); 
         
          hR3d=CreateWindow("BUTTON","三维图",WS_CHILD|WS_VISIBLE|
          BS_AUTORADIOBUTTON,xButton,90,lButton,wButton,hWnd,10,hInst,0); 
          
          hExit=CreateWindow("BUTTON","退出",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,yButton+250,lButton,wButton,hWnd,11,hInst,0); 
         
         
          
                  hYinteg=CreateWindow("BUTTON","纵积分",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton+110,yButton,lButton,wButton,hWnd,12,hInst,0); 
         
                                          
          hXinteg=CreateWindow("BUTTON","横积分",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton+110,yButton+40,lButton,wButton,hWnd,13,hInst,0); 
         
          h3d=CreateWindow("BUTTON","三维图",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton+110,yButton+80,lButton,wButton,hWnd,14,hInst,0); 
         
          hTrans=CreateWindow("BUTTON","横截面",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton+110,yButton+160,lButton,wButton,hWnd,15,hInst,0); 
         
          hTranstext=CreateWindow("STATIC"," Y: ",WS_CHILD|WS_VISIBLE
          |SS_RIGHT,xButton+110,yButton+128,20,wButton-10,hWnd,16,hInst,0); 
         
          hTransy=CreateWindow("EDIT",0,WS_CHILD|WS_VISIBLE|
          WS_BORDER,xButton+110+25,yButton+125,lButton-25,wButton-5,hWnd,17,hInst,0); 
         
          
          hLongi=CreateWindow("BUTTON","纵截面",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton+110,yButton+240,lButton,wButton,hWnd,18,hInst,0); 
         
          hLongitext=CreateWindow("STATIC"," X: ",WS_CHILD|WS_VISIBLE
          |SS_RIGHT,xButton+110,yButton+208,20,wButton-10,hWnd,19,hInst,0); 
         
          hLongix=CreateWindow("EDIT",0,WS_CHILD|WS_VISIBLE|
          WS_BORDER,xButton+110+25,yButton+205,lButton-25,wButton-5,hWnd,20,hInst,0); 
         
          
          hFlbox=CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|
          BS_GROUPBOX,xButton-10,yButton+290,2*lButton+50,(10+wButton)*5+10,hWnd,21,hInst,0); 
          
          hPathtext=CreateWindow("STATIC","文件路径:",WS_CHILD|WS_VISIBLE
          |SS_LEFT,xButton,yButton+300,lButton,wButton-10,hWnd,22,hInst,0); 
        
          
          hPath=CreateWindow("EDIT",0,WS_CHILD|WS_VISIBLE|
          WS_BORDER,xButton,yButton+320,lButton*2+10,wButton-5,hWnd,23,hInst,0); 
         
          hFiletext=CreateWindow("STATIC","文件名:",WS_CHILD|WS_VISIBLE
          |SS_LEFT,xButton,yButton+360,lButton,wButton-10,hWnd,24,hInst,0); 
        
         
          hFilename=CreateWindow("EDIT",0,WS_CHILD|WS_VISIBLE|
          WS_BORDER,xButton,yButton+380,lButton,wButton-5,hWnd,25,hInst,0); 
         
          
          hExttext=CreateWindow("STATIC","文件类型:",WS_CHILD|WS_VISIBLE
          |SS_LEFT,xButton+110,yButton+360,lButton,wButton,hWnd,26,hInst,0); 
        
         
          hExtname=CreateWindow("COMBOBOX",0,WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL
          |WS_BORDER,xButton+110,yButton+380,lButton,3*wButton,hWnd,27,hInst,0); 
         
          SendMessage(hExtname,CB_ADDSTRING,0,(LONG)(LPSTR)ExtName[0]);
          SendMessage(hExtname,CB_ADDSTRING,0,(LONG)(LPSTR)ExtName[1]);
          SendMessage(hExtname,CB_SETCURSEL,DataId,0L);
          
          
          hOpen=CreateWindow("BUTTON","打开",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,yButton+420,lButton,wButton,hWnd,28,hInst,0); 
        
          hSave=CreateWindow("BUTTON","保存",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,xButton,yButton+460,lButton,wButton,hWnd,29,hInst,0); 
        
           
          EnableWindow(hCamBox,FALSE); 
           
            hdcpenx1=GetDC(hCamBox);
            hdcpenx2=GetDC(hDrawBox);
           // hdcpenx21=GetDC(hDrawBox);
            hPenx1=CreatePen(PS_SOLID,2,RGB(0,0,255));
            hPenx2=CreatePen(PS_SOLID,2,RGB(255,255,255)); 
            hPenx3=CreatePen(PS_SOLID,2,RGB(255,0,0)); 
            hBrshx1=CreateSolidBrush(RGB(0,0,255));
            hBrshx2=CreateSolidBrush(RGB(255,255,255));
           // SelectObject(hdcpenx1,hBrshx1);
            SelectObject(hdcpenx2,hBrshx2);      
            
                  /*  SelectObject(hdcpenx1,hPenx1);
                    SelectObject(hdcpenx2,hPenx2);
                    Rectangle(hdcpenx1,2,2,320,240);
                    Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx1);
                    DeleteObject(hPenx2);
                   */
          //hInst1=GetWindowWord(hWnd,GWW_HINSTANCE);
          
             // Open the MCI video overlay driver. 
                                                                  
       
                                             
                                                
  handle=GlobalAlloc(GMEM_MOVEABLE,(long)((long)320*(long)240*4));
  
    
       lp=(char *)GlobalLock(handle);
        
                                              
                                                                  
                                                                  
           
       break;  
       
       case WM_MOVE:    
       
            mciSendString("set vb window at 0 0 320 240",NULL,NULL,NULL);
       
       break;   
               
       

       case WM_COMMAND:
             
              /*if (LOWORD(lParam)==hCamBox)
                 {     
                   EnableWindow(hCamBox,FALSE);
                   EnableWindow(hRealtime,FALSE);
                   EnDis(FALSE);
                   SetWindowText(hCamera,"Pause");
                   CamStat=1;
                   
             
                  }    */                    
            
            if (LOWORD(lParam)==hNew)
              {   
                   SelectObject(hdcpenx2,hPenx2);
                   Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx2);
                                        
                   if(NewImg==1)
                       {NewImg=0;
                        GetImage(hWnd);
                        }  
                  
                       
                          lpcount=0;
                        
                        for(i=0;i<320;i++)yIn0[i]=0;
                        for(i=0;i<240;i++)xIn0[i]=0;
                        
                         for(j=0;j<240;j++)
                           for(i=0;i<320;i++)
                       { 
                         yInTemp=(*(lp+lpcount));
                         yInTemp=yInTemp&0x00ff;
                         yIn0[i]+=yInTemp;
                         xIn0[j]+=yInTemp;
                          lpcount+=3;
                        }     
                        
                          for(i=0;i<320;i++)yIn0[i]=yIn0[i]/240;
                         
                        
                          for(i=0;i<240;i++)xIn0[i]=xIn0[i]/320;
                    
                            }
               
             if (LOWORD(lParam)==hExit)                     
                {
                           GlobalUnlock(handle);
                          GlobalFree(handle);                                                     
                               ReleaseDC(hCamBox,hdcpenx1);
                               ReleaseDC(hDrawBox,hdcpenx2);
                       mciSendString("close vb ",NULL,NULL,NULL);
                       PostQuitMessage(0);
                 }
                 
              if (LOWORD(lParam)==hCamera)
                 { if(firsttime==TRUE)
                     {
                      InitCam(hWnd);
                      firsttime=FALSE;
                      
                      }		    
                   if (CamStat==1)
                     {
                        EnDis(TRUE);
                        EnableWindow(hRealtime,TRUE);
                        SetWindowText(hCamera,"拍摄");
                        CamStat=0;         
                        mciSendString("freeze vb ",NULL,NULL,NULL);    
                       NewImg=1;
                      SaveStat=0;
                      }                                  
                     
                     else{
                    if (CamStat==0)
                     {
                        EnDis(FALSE);
                        EnableWindow(hRealtime,FALSE);
                        SetWindowText(hCamera,"定格");
                        CamStat=1;   
                        mciSendString("unfreeze vb",NULL,NULL,NULL);
                     }    
                     
                         }
                                  
                   
                  }                          
               
               if (LOWORD(lParam)==hRealtime)  
                  { //  EnDis(FALSE);
                   
                   if(firsttime==TRUE)
                     {InitCam(hWnd);	
                       firsttime=FALSE;
                     }  	    
                   
                    if (CamStat==2)
                     {
                        EnDis(TRUE);
                        EnableWindow(hCamera,TRUE);
                        SetWindowText(hRealtime,"实时");
                        CamStat=0;    
                        mciSendString("freeze vb",NULL,NULL,NULL);
                                    }                                  
                     
                     else{
                    if (CamStat==0)
                     {
                        EnDis(FALSE);
                        EnableWindow(hCamera,FALSE);
                        SetWindowText(hRealtime,"停止");
                        CamStat=2;     
                        mciSendString("unfreeze vb",NULL,NULL,NULL);
                                        //options
                  //open camera
                  //set timer
                  
                      }
                      }
                     
                  }
                  
                if (LOWORD(lParam)==hYinteg)   
                  {           
                     if(NewImg==1)
                       {NewImg=0;
                        GetImage(hWnd);
                        }  
                  
                       
                          lpcount=0;
                        
                        for(i=0;i<320;i++)yIn[i]=0;
                         for(j=0;j<240;j++)
                           for(i=0;i<320;i++)
                       { 
                         yInTemp=(*(lp+lpcount));
                         yInTemp=yInTemp&0x00ff;
                         yIn[i]+=yInTemp;
                         
                          lpcount+=3;
                        }     
                                         
                       for(i=0;i<320;i++) yIn[i]=yIn[i]/240-yIn0[i];                  
                                         
                         SelectObject(hdcpenx2,hPenx2);
                 
                   Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx2);  
                 
                    SelectObject(hdcpenx2,hPenx3);     
                     MoveTo(hdcpenx2,1,259-yIn[1]);
                     for(i=2;i<318;i++)
                     { 
                     LineTo(hdcpenx2,i,259-yIn[i]);   
                     MoveTo(hdcpenx2,i,259-yIn[i]);   
                      }
                      
                            
                           
                      DeleteObject(hPenx3);
                  
                  
                         SaveStat=1;
                  
                  }
                  
                    
                 if (LOWORD(lParam)==hXinteg) 
                   {  
                      //state
                      //convert
                      //draw     
                      
                      if(NewImg==1)
                       {NewImg=0;
                        GetImage(hWnd);
                        }  
                  
                       
                          lpcount=0;
                        
                        for(i=0;i<240;i++)xIn[i]=0;
                         for(j=0;j<240;j++)
                           for(i=0;i<320;i++)
                       { 
                         yInTemp=(*(lp+lpcount));
                         yInTemp=yInTemp&0x00ff;
                         xIn[j]+=yInTemp;
                         
                          lpcount+=3;
                        }     
                                         
                       for(i=0;i<240;i++) xIn[i]=xIn[i]/320-xIn0[i];                  
                                         
                         SelectObject(hdcpenx2,hPenx2);
                 
                   Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx2);  
                 
                    SelectObject(hdcpenx2,hPenx3);     
                     MoveTo(hdcpenx2,xIn[1],250-1);
                  /*for(i=0;i<3;i++)
                  { yIn[i]=yIn[i]; _ltoa(yIn[i],lpbuff,16); 
                  TextOut(hdcpenx2,i*150,40,lpbuff,strlen(lpbuff));   
                   }*/
                   
                   
                     for(i=2;i<237;i++)
                     { 
                     LineTo(hdcpenx2,xIn[i],250-i);   
                     MoveTo(hdcpenx2,xIn[i],250-i);   
                      }
                      
                      //state
                      //convert
                      //draw      
                           
                      DeleteObject(hPenx3);
                  
                              SaveStat=2;
                
                  
                    } 
                    
                    
                   if (LOWORD(lParam)==h3d)
                   {
                       if(NewImg==1)
                       {NewImg=0;
                        GetImage(hWnd);
                        }  
                                  
                       if(s3dsight==1)
                       
                       {s3dsight=0;
                        s3d1();
                        
                        }
                        
                        else{
                        if(s3dsight==0)
                        { s3dsight=1;
                          s3d2();
                          
                         }  
                         }
                        
                              SaveStat=3;
                
                    
                   
                 
                 
                  } 
                   
                  if (LOWORD(lParam)==hTrans)
                    {
                      
                               SaveStat=4;
                
                      //state
                      //read y
                      //draw 
                      
                    }
                    
                  if (LOWORD(lParam)==hLongi)
                     {
                      
                      //state
                      //read x
                      //draw 
                              SaveStat=5;
                 
                  }
                  
                  
                   if (LOWORD(lParam)==hSave)
                   {   
                       GetWindowText(hFilename,(LPSTR)FileName,50);
                       GetWindowText(hPath,(LPSTR)PathName,50);
                   
                      if(DataId==1)
                        {
                         
                        wsprintf(szMciString,"save vb %s%s.bmp type DIB 24-bit at 0 0 320 240",(LPSTR)PathName,(LPSTR)FileName);
                         mciSendString(szMciString,NULL,NULL,NULL);   
  
                         }
                         
                       else 
                       { //strcat(FileName,".dat");
                         SaveData(SaveStat);
                       
                       
                       
                       
                       }
                         
                      //state
                      //read ext
                      //img--mci,   dat-save() 
                      
                  }
                  
                     if (LOWORD(lParam)==hOpen)
                   {   
                       
                      //   GlobalUnlock(handle);
                      //   GlobalFree(handle);                                                     
      
                      // mciSendString("close vb ",NULL,NULL,NULL);
                       
                       GetWindowText(hFilename,(LPSTR)FileName,50);
                       GetWindowText(hPath,(LPSTR)PathName,50);
                   
                        wsprintf(szMciString,"%s%s.bmp",(LPSTR)PathName,(LPSTR)FileName);
                        OpenBmp(szMciString); 
                      
                                ReleaseDC(hCamBox,hdcpenx1);
                              
                              
                                  hdcpenx1=GetDC(hCamBox);
                       
                         
                         // SelectObject(hdcpenx2,hPenx3);
                          lpcount=0;
                         for(j=0;j<240;j++)
                           for(i=0;i<320;i++)
                       { //    _itoa(*(lp+i+10*j),lpbuff,10);        
                                               
                        
                         SetPixel(hdcpenx2,i,j,RGB(*(lp+lpcount)
                         ,*(lp+lpcount+1),*(lp+lpcount+2)));
                          lpcount+=3;
                        } 
                           
                      //DeleteObject(hPenx3);
                      
                      SaveStat=0;
                        ReleaseDC(hCamBox,hdcpenx1);
                                   hdcpenx1=GetDC(hCamBox);
                       
                      //state
                      
                      //draw
                  
                         
                      //state
                      //read ext
                      //img--mci,   dat-save() 
                      
                  }
                 
                   
                   
                   if (LOWORD(lParam)==hExtname)
                   {   
                      if(HIWORD(lParam)==CBN_CLOSEUP)
                        DataId=(WORD)SendMessage(hExtname,CB_GETCURSEL,0,0L); 
                      //state
                      //read ext
                      //img--mci,   dat-save() 
                      
                  }
                
             /* case IDM_SPECTRUM:  
                   nFileStat=0;nScale=3;
               InvalidateRect(hWnd,NULL,TRUE);   
             //   hPen3=CreatePen(PS_SOLID,2,RGB(255,0,0));
            hdcpen2=GetDC(hWnd);
            hdcpen3=GetDC(hWnd);
            hPenx2=CreatePen(PS_SOLID,2,RGB(255,0,0));
            hPenx=CreatePen(PS_SOLID,2,RGB(255,255,255)); 
            hBrsh2=CreateSolidBrush(RGB(255,255,255));
            SelectObject(hdcpen3,hBrsh2)  ;
            SelectObject(hdcpen3,hPenx);  
            SelectObject(hdcpen2,hPenx2);
            nWorking=1;  
              
             _outp(int_time,com);
             _outp(stb,0x00);
             _outp(stb,0x01);
                  //setting clock           
                  fpTime=MakeProcInstance((FARPROC)Clock2,hInst);
                  SetTimer(hWnd,2,500,fpTime);

     
              break;
            */
     
       break;          
       
      case WM_DESTROY:
           //close hpen3
          //close vb           
                    ReleaseDC(hCamBox,hdcpenx1);
                               ReleaseDC(hDrawBox,hdcpenx2);
                       
           GlobalUnlock(handle);
     GlobalFree(handle);                                                     
                        
          mciSendString("close vb",NULL,NULL,NULL);
          PostQuitMessage(0);
      break;

      default:
         return (DefWindowProc(hWnd, message, wParam, lParam));
      }
      
      
      
    return (NULL);
}




UINT FAR PASCAL __export Clock1(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)  

{   /*
   //HDC    hdcpen,hdcpen2;   
   HMENU  hMenu;   
   //HPEN   hPenx,hPenx2; 
   //static HBRUSH hBrsh;
   static HANDLE hInst;
 
    
  
  // int dec,sgn;  
   int j,cnt2,cnt;
  // double dLog2;        
  // set pluse=0
           
         
       if (nWorking==2) return (0);
       if (nWorking==0)
         {  
            hMenu=GetMenu(hWnd);
            KillTimer(hWnd,1);
            for(j=0;j<2;j++)EnableMenuItem(hMenu,IDM_RUN+j*10,MF_ENABLED); 
                
                nFileStat=1;    nScale=1;
                
                     ReleaseDC(hWnd,hdcpen2);
               ReleaseDC(hWnd,hdcpen3);
            return 0;
           }   
      
        Rectangle(hdcpen3,55,15,545,405);     
        
        
          _outp(0x105,0x04);  
               for(nP=0;nP<nPointsVl;nP++)
          {      
           
            
            GetData();
         
              for(cnt=0;cnt<100*nRepeatVl/nPointsVl;cnt++)for(cnt2=0;cnt2<cpumf;cnt2++){}
             
        
             //MessageBeep(-1); 
             MoveTo(hdcpen2,xOrgVm+xUnit*(__max(nP-1,0)),yOrgVm-yUnit*nIntensity[__max(nP-1,0)]);    
             LineTo(hdcpen2,xOrgVm+xUnit*nP,yOrgVm-yUnit*nIntensity[nP]); 
               
               } 
               
            _outp(0x105,0x00);
      
           //set pulse=1    
           
     */
 return 0;
}   
            


BOOL SaveData(int dataid)
{
  FILE *File;
  int i,totalcount;
  
  
  wsprintf(szMciString,"%s%s.dat",(LPSTR)PathName,(LPSTR)FileName);
  File=fopen(szMciString,"wt");
   
    if(dataid==1)
    {
     for(i=0;i<320;i++)
      {
        fprintf(File,"%d",i+1);
        putc(' ',File);
        fprintf(File,"%d",yIn[i]);
        if(i==(320-1))putc(' ',File);
        else putc('\n',File);
      }
     }  
  
     if(dataid==2)
    {
     for(i=0;i<240;i++)
      {
        fprintf(File,"%d",i+1);
        putc(' ',File);
        fprintf(File,"%d",xIn[i]);
        if(i==(240-1))putc(' ',File);
        else putc('\n',File);
      }
     }  
    
      
     
     
         fclose(File);  
   return TRUE;  
}   


  
  

void ReadPixel()
{
  
  
  
}                    

void Yintegrate()
{




}
void xintegrate()
 {
 
// GetImage();
 
 
 }



 void EnDis(BOOL tf)     
 {              
                
  EnableWindow(hNew,tf);
  EnableWindow(hSave,tf);
  EnableWindow(hOpen,tf);
  EnableWindow(hExit,tf);
  EnableWindow(hYinteg,tf);
  EnableWindow(hXinteg,tf);
  EnableWindow(h3d,tf);
  EnableWindow(hTrans,tf);
  EnableWindow(hLongi,tf);
 
 }                    
 void InitCam(HWND hWnd)
 {
            lstrcpy(szMciString, "open overlay alias vb style child parent ");
            lstrcat(szMciString, ltoa(hWnd, temp, 10));
            if (mciSendString(szMciString, NULL,NULL,NULL)) {
                DefWindowProc(hWnd, WM_DESTROY, 0, 0);
               
            }
            
            mciSendString("capability vb card type", temp, 16, NULL);
            nCardType=atoi(temp);
            
            

            // Get the handle of the window used for the video overlay display. 
            if (mciSendString("status vb window handle", 
                               temp, 16, NULL)) {
                DefWindowProc(hWnd, WM_DESTROY, 0, 0);
               
            }
            hMCIVideo=(HWND)atol(temp);
            
        //    Show the video overlay window. 
            if (mciSendString("window vb state show",NULL,NULL,NULL)) {
                DefWindowProc(hWnd, WM_DESTROY, 0, 0);
               
            }
            
          //MoveWindow(hMCOVideo,0,0,320,240,TRUE);
           mciSendString("set vb window at 20 10 320 240",NULL,NULL,NULL);
           
 }                                
 
 

void GetImage(HWND hWnd)
{ /*HGLOBAL handle;
  int offset,width,height;
  LPVOID lphandle;
  long i,j,k=0;
  char *lp;
  RECT rc; 
 
  //GetClientRect(hWnd,&rc);
 
 //offset=rc.right/8;
 // offset=offset/4*4;
 // width=(rc.right-2*offset)/4*4;
  //height=(rc.bottom-2*offset);
  
                                                
  offset=40;
   width=320-80;
   height=240-80;                                             
                                                
  handle=GlobalAlloc(GMEM_MOVEABLE,(long)((long)width*(long)height*3));
  
  lphandle=(LPVOID)GlobalLock(handle);
  
  wsprintf(szMciString,"save vb memory buffer at %d %d %d %d type dib 8-bit address %1u",
            offset,offset,width,height,(unsigned long)(LPVOID)lphandle);
  mciSendString(szMciString,NULL,NULL,NULL);
  
  
       lp=(char *)GlobalLock(handle);
        
      for(j=0;j<240;j++)
       for(i=0;i<(320);i++)
         {              
          SetPixel(hdcpenx2,i,j,RGB(*lp,*(lp+1),*(lp+2)));
           lp+=3;
          }
          
       GlobalUnlock(handle);
      GlobalUnlock(handle);
     GlobalFree(handle);
    */     
 mciSendString("save vb temp01.bmp type DIB 24-bit at 0 0 320 240",NULL,NULL,NULL);   
 
 OpenBmp("temp01.bmp");    
     
}               

void OpenBmp(LPSTR Filename)

{
  FILE *hFile2;
  int i;      
   char temp[100];
  
  
  hFile2=fopen(Filename,"r");
 
  //if(hFile2==NULL){*lp=11;}
  
  fread(temp,1,0x36,hFile2);
  for(i=0;i<6;i++)fread(lp+i*38400,1,(long)320*(long)40*3,hFile2);
  //fread(lp1,1,320*120,hFile2);
  fclose(hFile2);
            // lp+=0x36;


}                      

      
      
void s3d1()
{ int i,j,k,x,y;

                          lpcount=0;
                        
                    SelectObject(hdcpenx2,hPenx2);
                 
                   Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx2);  
                    
                    
                    SelectObject(hdcpenx2,hPenx3);     
                     
                       
                     
                    
                    for(j=236;j>1;j-=10)
                      {
                       lpcount=3*(long)j*320+3;
                       xyInTemp=(*(lp+lpcount));
                         xyInTemp=xyInTemp&0x00ff-yIn0[1]         ;
                     MoveTo(hdcpenx2,(int)(1*2/3+j*11/24),259-xyInTemp/2-j*13/24);
                        lpcount+=3;
                     for(i=2;i<318;i++)
                     { 
                        xyInTemp=(*(lp+lpcount));
                         xyInTemp=(xyInTemp&0x00ff)-yIn0[i]          ;
                          lpcount+=3;
                          x=(int)(i*2/3+1+j*11/24);
                          y=259-(int)(xyInTemp/2)-j*13/24;
                       LineTo(hdcpenx2,(int)x,y);   
                       MoveTo(hdcpenx2,(int)x,y);   
                           DeleteObject(hPenx3);
                          SelectObject(hdcpenx2,hPenx2);  
                          Rectangle(hdcpenx2,x,y,x+1,259);
                    //   LineTo(hdcpenx2,(int)(i*2/3+j*11/24),259);   
                          DeleteObject(hPenx2);
                            SelectObject(hdcpenx2,hPenx3);  
                      }
                      
                      }     
                      
                      
                        MoveTo(hdcpenx2,(int)(318*2/3+236*11/24),259-238*13/24);
                   LineTo(hdcpenx2,(int)(318*2/3+2*11/24),259-2*13/24);
                   MoveTo(hdcpenx2,(int)(318*2/3+2*11/24),259-2*13/24);
                   LineTo(hdcpenx2,(int)(2*2/3+2*11/24),259-2*13/24);
                   
                           
                      DeleteObject(hPenx3);
                   
  


}


      
      
void s3d2()
{ int i,j,k,x,y;

                          lpcount=0;
                        
                    SelectObject(hdcpenx2,hPenx2);
                 
                   Rectangle(hdcpenx2,2,2,320,260);
                    DeleteObject(hPenx2);  
                    
                    
                    SelectObject(hdcpenx2,hPenx3);     
                     
                     
                    
                    for(j=236;j>1;j-=10)
                      {
                       lpcount=3*(long)j*320+3;
                       xyInTemp=(*(lp+lpcount));
                         xyInTemp=xyInTemp&0x00ff-yIn0[1]         ;
                     MoveTo(hdcpenx2,(int)(1*2/3-j*11/24+110),259-xyInTemp/2-j*13/24);
                        lpcount+=3;
                     for(i=2;i<318;i++)
                     { 
                        xyInTemp=(*(lp+lpcount));
                         xyInTemp=(xyInTemp&0x00ff)-yIn0[i]          ;
                          lpcount+=3;
                          x=(int)(i*2/3+1-j*11/24+110);
                          y=259-(int)(xyInTemp/2)-j*13/24;
                       LineTo(hdcpenx2,(int)x,y);   
                       MoveTo(hdcpenx2,(int)x,y);   
                           DeleteObject(hPenx3);
                          SelectObject(hdcpenx2,hPenx2);  
                          Rectangle(hdcpenx2,x,y,x+1,259);
                    //   LineTo(hdcpenx2,(int)(i*2/3+j*11/24),259);   
                          DeleteObject(hPenx2);
                            SelectObject(hdcpenx2,hPenx3);  
                      }
                      
                      }
                       MoveTo(hdcpenx2,(int)(2*2/3-236*11/24+110),259-238*13/24);
                   LineTo(hdcpenx2,(int)(2*2/3-2*11/24+110),259-2*13/24);
                   MoveTo(hdcpenx2,(int)(2*2/3-2*11/24+110),259-2*13/24);
                   LineTo(hdcpenx2,(int)(318*2/3-2*11/24+110),259-2*13/24);
                      
                    
                           
                      DeleteObject(hPenx3);
                   
                  

}



      