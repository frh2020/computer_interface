
#include "windows.h"
#include "shg.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "conio.h"
#include "math.h"
#include "commdlg.h" 
#include "string.h"
#include "fcntl.h"
#include "sys\types.h"
#include "sys\stat.h"
#include "io.h"

HANDLE hInst;






//measrue value
static int nP,nIntensity0[2500],nIntensity[2500],nIntensity2[2500];
float nIntensity00; 
static int lineAm=10,logidx=20;
                           

//open file 
static char lpFilt[]="DATA File(*.dat)\0*.dat\0All Files(*.*)\0*.*\0";
static char lpFileNm[30], lpFileTit[30];
static OPENFILENAME ofn,ofn1;
 OFSTRUCT ofstruct;
      FILE  *File,*hFile; 

                                            
static int x0=20,x1=20,y00=3,y001=203,width=500,heigh=180;
static int xOrg1=50,  yOrg1=420;
static int xOrg2=345, yOrg2=124;
         

static int   yUnitVm=1;  
double           xUnit=1; 

int iobase=0x280;
 
 
static HDC hdcpen,hdcpen2,hdcpen3,hdcpen4;
PAINTSTRUCT ps;
static HBRUSH hBrsh,hBrsh2;
static HPEN hPen1,hPen2,hPen3,hPenx,hPenx2,hPenx3,hPenx4;

static char *RepeatItem[]={"50","1000","1500","2000"};  //measure selection (ms)
static char *PointsItem[]={"100","500","1000","2500"};
static int nRepeatId=1,nPointsId=0;             //Item number
static int nRepeatVl=1000,nPointsVl=100;           //Item value


static int nWorking=0,nDisplay=0,TempS,nFrame=0;              //computer status



HANDLE hCurIns;  

 static HWND hStop,  hPause, hZoom, hLeft, hRight;                          
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
    hCurIns=hInstance;

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
    wc.hIcon = LoadIcon(hInstance, "IDC_ICON0");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = COLOR_WINDOW+1;
    wc.lpszMenuName =  "SHG";
    wc.lpszClassName = "InPutWClass";

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
        "InPutWClass",
        "中山大学超快速激光光谱学国家重点实验室",
        WS_OVERLAPPEDWINDOW ,
        50,5,
        550,
        470,
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

   
 
    int i,j;
   // int xScale;
    int k=0;
    double x1=12.0,x2=380.0;
   // int Temp[3];
    static FARPROC lpProcAbout;
    static HANDLE hInst;
    HMENU  hMenu; 
    
   // FILE  *hFileS,*hFileC; 
    
    char StringScale[20];
    BOOL   bSet;
    FARPROC fpTime;
    
    int mx,my;
    static HCURSOR  hCur;
    static HANDLE hInst1;
    
    HDC hdc1,hdc;
    
    switch (message)
     {
       case WM_PAINT:
          //intigraoh
            hdcpen=BeginPaint(hWnd,&ps);
            hPen1=CreatePen(PS_SOLID,2,RGB(0,0,255));
            hPen2=CreatePen(PS_SOLID,2,RGB(255,255,255));
            hPen3=CreatePen(PS_SOLID,2,RGB(255,0,0));
            hBrsh=CreateSolidBrush(RGB(0,0,0));
            SelectObject(hdcpen,hPen1);
            
           
            
            Rectangle(hdcpen,x0,y00+2,x0+width+1,y00+heigh+3);            
            Rectangle(hdcpen,x0,y001+2,x0+width+1,y001+heigh+3);            
                                                 
                                                 
          // DRAW UNIT                                       
                                                 
                                                 
            for(j=0;j<11;j++)
            { MoveTo(hdcpen,20,j*18+5);
             LineTo(hdcpen,25,j*18+5);
                //   _itoa(j,StringScale,10);
            MoveTo(hdcpen,20,j*18+205);
             LineTo(hdcpen,25,j*18+205);
                  
             if (nDisplay==0) _itoa((int)(j*nPointsVl/10),StringScale,10);
             else  _itoa(j*50+500*nFrame,StringScale,10);
                          
            TextOut(hdcpen,10+j*50,187,StringScale,strlen(StringScale));                   MoveTo(hdcpen,j*50+50,409);
             
            _itoa(j,StringScale,10);
            if(j>0)
              {TextOut(hdcpen,2,180-j*18,StringScale,strlen(StringScale));  
               TextOut(hdcpen,2,380-j*18,StringScale,strlen(StringScale));  
               
               }
             MoveTo(hdcpen,j*50+20,185);
             LineTo(hdcpen,j*50+20,180);  
             MoveTo(hdcpen,j*50+20,205);
             LineTo(hdcpen,j*50+20,210);
             }
             
          if(nWorking>0)
             {
               if (nDisplay==0) 
                 {  for(i=0;i<nP;i++)
                     {SetPixel(hdcpen,x0+xUnit*i,y00+heigh-nIntensity[i],RGB(255,0,0));
       
                     SetPixel(hdcpen,x0+xUnit*i,y001+heigh-nIntensity2[i],RGB(255,0,0));
                     }
                 } 
                  
                else
                  {   
                        
                       for(i=0;i<500;i++)
                        {
                          SetPixel(hdcpen,x0+i,y00+heigh-nIntensity[i+500*nFrame],RGB(255,0,0));
                          SetPixel(hdcpen,x0+i,y001+heigh-nIntensity2[i+500*nFrame],RGB(255,0,0));
                        } 
                  }
             
             
             
             } 
          EndPaint(hWnd,&ps);
         break;                     

       case WM_CREATE:
          ofn.lStructSize=sizeof(OPENFILENAME);
          ofn.hwndOwner=hWnd;
          ofn.lpstrFilter=lpFilt;
          ofn.lpstrCustomFilter=NULL;
          ofn.nMaxCustFilter=0;
          ofn.nFilterIndex=1L;
          ofn.lpstrFile=lpFileNm;
          ofn.nMaxFile=30;
          ofn.lpstrInitialDir=NULL;
          ofn.lpstrFileTitle=lpFileTit;
          ofn.nMaxFileTitle=30;
          ofn.lpstrDefExt="dat";
         
      
        
          hStop=CreateWindow("BUTTON","停止",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,20,400-5,50,20,hWnd,1,hCurIns,0); 
          
          hPause=CreateWindow("BUTTON","暂停",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,70,400-5,50,20,hWnd,2,hCurIns,0); 
          
                   
          hZoom=CreateWindow("BUTTON","X轴放大",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,170,400-5,80,20,hWnd,4,hCurIns,0); 
       
          hLeft=CreateWindow("BUTTON","<<",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,270,400-5,50,20,hWnd,5,hCurIns,0); 
    
          hRight=CreateWindow("BUTTON",">>",WS_CHILD|WS_VISIBLE|
          BS_PUSHBUTTON,320,400-5,50,20,hWnd,6,hCurIns,0); 
    
       hInst1=GetWindowWord(hWnd,GWW_HINSTANCE);
           
       break;

       case WM_COMMAND:

          switch(wParam)
           {  
              case IDM_NEW:
              
               //nFileStat=0;
               nWorking=0;nDisplay=0;  SetWindowText(hZoom,"X轴放大");
                  
               //clear
               InvalidateRect(hWnd,NULL,TRUE);  
              
              break;
            
              case IDM_OPEN:
               
                ofn.lpstrTitle="Open FIle";
                ofn.Flags=OFN_HIDEREADONLY;
                if(!GetOpenFileName(&ofn))return FALSE;
                File=fopen(ofn.lpstrFile,"rt");
                  i=0;
               
               for(i=0;i<2500;i++)
               {nIntensity[i]=0;nIntensity0[i]=1;nIntensity2[i]=0;}
                 i=0;
               for(;;)
                {fscanf(File,"%d",&nP);
                     k=getc(File);
                 fscanf(File,"%f",&nIntensity00);
                  nIntensity0[i]=(int)(1000.0*nIntensity00);
                  nIntensity[i]=nIntensity0[i]/lineAm;
                  nIntensity2[i]=(int)(logidx*log((double)nIntensity0[i]));
 
                   
                 
                 
                     k=getc(File);   
                   i++;
                   if(k==' ')break; 
                    
                 } 
                 nPointsVl=i;i=0;   
                 
                fclose(File);               
                
                   xUnit=500.0/(double)nPointsVl;
               
                

                   nDisplay=0;nFrame=0;nWorking=3;
               
                {EnableWindow(hZoom,TRUE);
                EnableWindow(hLeft,TRUE);
                EnableWindow(hRight,TRUE);
                 }
              
                  SetWindowText(hZoom,"X轴放大");
                  

               // nFileStat=1; nScale=1;  
                
                InvalidateRect(hWnd,NULL,TRUE);

              break;

             
           
           
              case IDM_SAVE:
               //TempS=nScale;
               //nScale=1;
               SaveData();
               //nScale=TempS;
              break;

             
             
      
              case IDM_EXIT:
                 PostQuitMessage(0);
              break;
              
             
              case IDM_RUN:  
                 //indiate setting dialog
                 hInst=(HINSTANCE)GetWindowWord(hWnd,GWW_HINSTANCE);
                 lpProcAbout = MakeProcInstance(RunSet, hInst);
                 bSet=DialogBox(hInst,"RUNSET", hWnd, lpProcAbout);
                 FreeProcInstance(lpProcAbout);
                 if(bSet==FALSE) break;  

               //setting parameter
                 nRepeatVl=atoi(RepeatItem[nRepeatId]);
                 nPointsVl=atoi(PointsItem[nPointsId]);
                 
                 SetWindowText(hPause,"暂停"); 
               //change menu  
               //  hMenu=GetMenu(hWnd);             
               //    for(j=0;j<2;j++)EnableMenuItem(hMenu,IDM_RUN+j*10,MF_GRAYED); 

                //inti Window
                //nFileStat=0;nScale=1;
                    
                    for(i=0;i<2500;i++)
                    {nIntensity[i]=0;nIntensity0[i]=0;nIntensity2[i]=0;}
                nDisplay=0; 
                nP=0;
                nWorking=0;
                InvalidateRect(hWnd,NULL,TRUE);   
                nWorking=1;
               SetWindowText(hZoom,"X轴放大");
                     
                EnableWindow(hStop,TRUE);
                EnableWindow(hPause,TRUE);
                EnableWindow(hZoom,FALSE);
                EnableWindow(hLeft,FALSE);
                EnableWindow(hRight,FALSE);
                
                hdcpen2=GetDC(hWnd);
                hdcpen3=GetDC(hWnd);
                hdcpen4=GetDC(hWnd);
                xUnit=500.0/(double)nPointsVl;
                //yUnit=1;
                hPenx2=CreatePen(PS_SOLID,2,RGB(255,0,0));
                hPenx3=CreatePen(PS_SOLID,2,RGB(0,0,0));
                hPenx4=CreatePen(PS_SOLID,2,RGB(0,0,255));
                hPenx=CreatePen(PS_SOLID,2,RGB(255,255,255)); 
                hBrsh2=CreateSolidBrush(RGB(255,255,255));
                SelectObject(hdcpen3,hBrsh2)  ;
                SelectObject(hdcpen3,hPenx);  
                SelectObject(hdcpen2,hPenx2);
                //SelectObject(hdcpen4,hPenx4);
               //setting clock           
               
                 // _outp(0x107,0x80); 
                    _outp(iobase+4,0x00);       
                  fpTime=MakeProcInstance((FARPROC)Clock1,hInst);
                  SetTimer(hWnd,1,nRepeatVl,fpTime);

                break;

             
              case IDM_ABOUT:               
              hInst=(HINSTANCE)GetWindowWord(hWnd,GWW_HINSTANCE);
              lpProcAbout = MakeProcInstance(About, hInst);
               DialogBox(hInst,
                 "ABOUTBOX",
                 hWnd,
                 lpProcAbout);

              FreeProcInstance(lpProcAbout);
              break;
            }
             
         
          if (LOWORD(lParam)==hStop)
          { nWorking=0;
              
            //hMenu=GetMenu(hWnd);  
            //for(j=0;j<2;j++)EnableMenuItem(hMenu,IDM_RUN+j*10,MF_ENABLED); 
             EnableWindow(hZoom,TRUE);
             EnableWindow(hLeft,TRUE);
             EnableWindow(hRight,TRUE);
                      
          }     
            
         
          if (LOWORD(lParam)==hPause)
          { if (nWorking==1){SetWindowText(hPause,"继续");nWorking=2;  }
            else{
               if (nWorking==2){SetWindowText(hPause,"暂停");nWorking=1; }
                }
                      
          }         
                                        
                                        
         if (LOWORD(lParam)==hZoom&&nWorking==3)
          {//yUnit+=1;
           //if(yUnit>9.0)yUnit=9.0;
           if(nDisplay==0)
             {  nDisplay=1; 
                SetWindowText(hZoom,"全部显示");
                InvalidateRect(hWnd,NULL,TRUE);
             } 
           else 
            {   nDisplay=0;
                SetWindowText(hZoom,"X轴放大");
                InvalidateRect(hWnd,NULL,TRUE);
             }  
                     
          }        
            
           if (LOWORD(lParam)==hLeft&&nDisplay>0)
          {//yUnit-=1;
           //if(yUnit<1.0)yUnit=1.0;
             if(nFrame>0)
                {nFrame--;
                   InvalidateRect(hWnd,NULL,TRUE);
                 }
                       
          }        
          
            if (LOWORD(lParam)==hRight&&nDisplay>0)
          {//yUnit-=1;
           //if(yUnit<1.0)yUnit=1.0;
          if(nFrame<4)
                {nFrame++;
                   InvalidateRect(hWnd,NULL,TRUE);
                 }
                          
          }        
         
      break;              
     
      case WM_LBUTTONDBLCLK:
       mx=LOWORD(lParam);
       my=HIWORD(lParam);
       if(mx>x0-2&&mx<(x0+width)&&my>y00&&my<(y001+heigh)&&nWorking==3&&(nDisplay==1))
         {  nDisplay=2;
         
            hCur=LoadCursor(hInst1,"IDC_CURSOR1");
             SetCursor(hCur);
           
          } 
      
      break;    
      
      case WM_RBUTTONDOWN:
            if(nDisplay==2)
            { 
              nDisplay=1;
            hCur=LoadCursor(NULL,IDC_ARROW);
             SetCursor(hCur);
             }
          
      break;
        
      case WM_LBUTTONDOWN:
         mx=LOWORD(lParam);
       my=HIWORD(lParam);    
       if(mx>x0-2&&mx<(x0+width)&&my>y00&&my<(y001+heigh)&&(nWorking==3)&&(nDisplay==2))
         {   
             hdc1=GetDC(hWnd);
            ChangeData1(hdc1,mx);
            ReleaseDC(hWnd,hdc);
          }
          
      break;
      
      case WM_MOUSEMOVE:
      mx=LOWORD(lParam);
         my=HIWORD(lParam);
        
      
     if(mx>x0&&mx<(x0+width)&&my>y00&&my<(y001+heigh)&&nWorking==3&&nDisplay==2)
         {   
       
        hdc=GetDC(hWnd);
        Displayxy(hdc,mx,my);
         hCur=LoadCursor(hInst1,"IDC_CURSOR1");
             SetCursor(hCur);
        ReleaseDC(hWnd,hdc);
    
       }  
      
      break;
      
      case WM_DESTROY:
          PostQuitMessage(0);
      break;

      default:
         return (DefWindowProc(hWnd, message, wParam, lParam));
      }
    return (NULL);
}



BOOL FAR PASCAL __export About(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
WORD wParam;
LONG lParam;
{

  switch (message)
     {
       case WM_INITDIALOG:
         return (TRUE);

       case WM_COMMAND:
         switch(wParam)
           { case  IDM_OK:
          
                EndDialog(hDlg, NULL);
              return (TRUE);
           }
         break;
        default :
        return FALSE;    
    }
    return (TRUE);
}
 
 
BOOL FAR PASCAL __export RunSet(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
WORD wParam;
LONG lParam;
{ 
  int i;
  switch (message)
     {
       case WM_INITDIALOG:
         for(i=0;i<4;i++)
           { SendDlgItemMessage(hDlg,IDD_REPEAT,CB_ADDSTRING,i,
                    (LPARAM)(LPCSTR)RepeatItem[i]);
             SendDlgItemMessage(hDlg,IDD_POINTS,CB_ADDSTRING,i,
                    (LPARAM)(LPCSTR)PointsItem[i]);
            }
            
           SendDlgItemMessage(hDlg,IDD_REPEAT,CB_SETCURSEL,nRepeatId,0L);
           SendDlgItemMessage(hDlg,IDD_POINTS,CB_SETCURSEL,nPointsId,0L);
                    
       
         return (TRUE);

      case WM_COMMAND:
         switch(wParam)
           { case  IDD_OK:
               nRepeatId=(int)SendDlgItemMessage(hDlg,IDD_REPEAT,CB_GETCURSEL,0,0L);
               nPointsId=(int)SendDlgItemMessage(hDlg,IDD_POINTS,CB_GETCURSEL,0,0L);
               EndDialog(hDlg,TRUE);
             break;
             
             case IDD_CANCEL:
               EndDialog(hDlg,FALSE);
               return FALSE;
             break;  
              
                           
             }
         break;
        default :
        return FALSE;    
    }
    return (TRUE);

}

  
UINT FAR PASCAL __export Clock1(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)  

{  
   
   HMENU  hMenu;   
  
   static HANDLE hInst;
  // HDC hdcpenx;
      int i,j;//,cnt2,cnt,ct1,t,nP1,k,yp,xp,xp1,yp1 ;
 
           
         
       if (nWorking==2) return (0);
       if (nWorking==0||nP==nPointsVl)
         {  
            
            
            
            //hMenu=GetMenu(hWnd);
            KillTimer(hWnd,1);
            //for(j=0;j<2;j++)EnableMenuItem(hMenu,IDM_RUN+j*10,MF_ENABLED); 
                
              //  nFileStat=1;    nScale=1;
              nDisplay=0;nFrame=0;nWorking=3;
               
                {EnableWindow(hZoom,TRUE);
                EnableWindow(hLeft,TRUE);
                EnableWindow(hRight,TRUE);
                 }
              
              ReleaseDC(hWnd,hdcpen4);
                  
                     ReleaseDC(hWnd,hdcpen2);
               ReleaseDC(hWnd,hdcpen3);
            return 0;
           }   
      
        
        // SelectObject(hdcpen4,hPenx);
        
      // Rectangle(hdcpen3,50,10,550,420);     
        GetData();
        
        SetPixel(hdcpen3,x0+xUnit*nP,y00+heigh-nIntensity[nP],RGB(255,0,0));
       
        SetPixel(hdcpen3,x0+xUnit*nP,y001+heigh-nIntensity2[nP],RGB(255,0,0));
       
              
          nP++;       
           
                
           // DeleteObject(hPenx); 
                
      
           //set pulse=1  
     
 return 0;
}   
            
      
            
void GetData()

{     int a,b,x;
      double d1,d2;  
       
     _outp(iobase,0);
     _outp(iobase+4,0x10);
    while((_inp(iobase+1))&0x80==0){}
    
    a=_inp(iobase+2);
    b=_inp(iobase+3)&0xf;
    d1=a*256+b;
    
     _outp(iobase+4,0x00);
    _outp(iobase,1);
     _outp(iobase+1,0x00);
    
    while((_inp(iobase+1))&0x80==0){}
    
    a=_inp(iobase+2);
    b=_inp(iobase+3)&0xf;
    d2=a*256+b; 
       
       
     nIntensity0[nP]=(int)(d2*1000/d1)+1;
     nIntensity[nP]=nIntensity0[nP]/lineAm;
     nIntensity2[nP]=(int)(logidx*log((double)nIntensity0[nP]));

                                                                           
 }


BOOL SaveData()
{ int i;
  char achar=' ';
  
  
  
   ofn.lpstrTitle="Save FIle";
   ofn.Flags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
   if(!GetSaveFileName(&ofn))return FALSE;
    hFile=fopen(ofn.lpstrFile,"wt");
    
    {for(i=0;i<nP;i++)
     {fprintf(hFile,"%d",i+1);
      putc(' ',hFile);
     fprintf(hFile,"%f",(double)nIntensity0[i]/1000);
      if(i==(nPointsVl-1))putc(' ',hFile);
      else putc('\n',hFile);
     }
     }  
     
     
    /* 
    if (nScale==3)
    {for(i=0;i<2160;i++)
     { xIccdData[i]=(kp[0]+kp[1]*i+kp[2]*i*i);
       fprintf(hFile,"%f",xIccdData[i]);
      putc(' ',hFile);
     fprintf(hFile,"%d",IccdData[i]);
      if(i==2159)putc(' ',hFile);
      else putc('\n',hFile);
     }
     }
     
      */
     
         fclose(hFile);
   return TRUE;
}   


  void Displayxy(HDC hdc,int x, int y)
{

  int nl,xx,i;
  char cStr[80]; 
                                     
  
  xx=(x-x0)+nFrame*500;
  nl=wsprintf(cStr," Intensity: %5d     ",        nIntensity[xx]);
  TextOut(hdc ,30,20,cStr,nl);      

   
   
  nl=wsprintf(cStr,"    Points: %5d    ",xx);
  TextOut(hdc ,30,40,cStr,nl);      
} 


  void ChangeData1(HDC hdc,int x)
{

  int nl,xx,i;
  char cStr[80]; 
                                     
  
  xx=(x-x0)+nFrame*500; 
    SetPixel(hdc,x,y00+heigh-nIntensity[xx],RGB(255,255,255));
    SetPixel(hdc,x,y001+heigh-nIntensity2[xx],RGB(255,255,255));
 
  nIntensity0[xx]=(nIntensity0[xx-1]+nIntensity0[xx+1])/2;
  nIntensity[xx]=(nIntensity[xx-1]+nIntensity[xx+1])/2;
  if(nIntensity[xx]!=0)
  nIntensity2[xx]=(int)(logidx*log((double)nIntensity0[xx]));
  SetPixel(hdc,x,y00+heigh-nIntensity[xx],RGB(255,0,0));
  SetPixel(hdc,x,y001+heigh-nIntensity2[xx],RGB(255,0,0));
  
  nl=wsprintf(cStr," Intensity: %5d     ",
        nIntensity[xx]);
  TextOut(hdc ,30,20,cStr,nl);      

   
   
  nl=wsprintf(cStr,"    Points: %5d    ",xx);
  TextOut(hdc ,30,40,cStr,nl);      
}                   