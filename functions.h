
#ifndef __CONIO4LINUX_H
#define __CONIO4LINUX_H
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15
#define BLINK        128
#define BOLD  "\x1B[1;34m"


#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

void textcolor(int);
void textbackground(int);
char* strupr(char*);
int bgc=10;

static struct termios old,ne;
void initTermios(int echo)
{
 tcgetattr(0,&old);
 ne=old;
 ne.c_lflag &=~ICANON;
 ne.c_lflag &=echo ? ECHO: ~ECHO;
 tcsetattr(0, TCSANOW, &ne);
}
void resetTermios(void)
{
 tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo)
{
 char ch;
 initTermios(echo);
 ch=getchar();
 resetTermios();
 return ch;
}
char getch(void)
{
 return getch_(0);
}

 char getche(){
      return getch_(1);
      }

void gotoxy(int x,int y)
{
 printf("%c[%d;%df",0x1B,y,x);
}

void clrscr(){
printf( "\033[%dm\033[2J\033[1;1f",bgc);
//printf("\033[m");

}


char *strupr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper(*p);
      p++;
  }

  return str;
}

int getpassword(char *pass)
{
  struct termios oflags, nflags; 
 /* disabling echo */
 tcgetattr(fileno(stdin), &oflags);
 nflags = oflags;
 nflags.c_lflag &= ~ECHO;
 nflags.c_lflag |= ECHONL;

 if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
     perror("tcsetattr");
     return EXIT_FAILURE;
 }

 fgets(pass, sizeof(pass), stdin);
 pass[strlen(pass) - 1] = 0;

 /* restore terminal */
 if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
     perror("tcsetattr");
     return EXIT_FAILURE;
 }
}

int readint()
{
 char line[10];
 int arg1;
 int isint;

  while (1) {
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&arg1);
    if (isint>=0 && isint<=INT_MAX) break;
  }
  return arg1;
}


void readdate(int *dd,int *mm,int *yy,int cf)
{
  int i,age=0;
  int check=0;
  char buf[15],c='\0';
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  Label:
  printf("\n\t PLASE ENTER IN [DD-MM-YYYY] FORMAT: ");
  for(i=0;c!='\n';)
  {
   if((i==2 || i==5) && check<2) c='-';
   else c=getch();

   if(c!=127)
   { 
    buf[i]=c;
    printf("%c",buf[i]);
    if(i!=2 && i!=5)
     if(!isdigit(c) && c!='\n')
     {
      printf("\n\t DATES ARE GENERALLY IN DIGIT!");
      goto Label;
     }
    i++;
   }
   else
   {
    i--;
    if(i<0)
     i++;
    else
     printf("\b \b");
    check=i;
   }
  }
  buf[i]='\0'; c='\0';
  sscanf(buf,"%d%*c%d%*c%d",dd,mm,yy);

  if(*yy<1900 || *yy>9999)
  {
   printf("\n\t DATE DOESN'T LOOK RIGHT ");
   goto Label;
  }
  
  if(*mm<=0 || *mm>12)
  {
   printf("\n\t DATE DOESN'T LOOK RIGHT ");
   goto Label;
  }
  
  if(*mm==1||*mm==3||*mm==5||*mm==7||*mm==8||*mm==10||*mm==12)
  {
   if(*dd==0 || *dd>31)
   {
    printf("\n\t DATE DOESN'T LOOK RIGHT ");
    goto Label;
   }
  }
  else
  if(*mm==4||*mm==6||*mm==9||*mm==11)
  {
   if(*dd==0|| *dd>30)
   {
    printf("\n\t DATE DOESN'T LOOK RIGHT ");
    goto Label;
   }
  }
  else
  if(*mm==2)
  {
   if(*yy%4==0)
   {
    if(*dd==0 || *dd>29)
    {
     printf("\n\t DATE DOESN'T LOOK RIGHT ");
     goto Label;
    }
   }
   else if(*dd==0 || *dd>28)
   {
    printf("\n\t DATE DOESN'T LOOK RIGHT ");
    goto Label;
   }
  }
  
  age=( (((tm.tm_year+1900)*10000) + ((tm.tm_mon+1)*100) + tm.tm_mday) - (((*yy)*10000) + ((*mm)*100) + (*dd)) )/10000;
  
  if(cf==0)
  {
   if(age<25) 
   {
    printf("\n\t AGE SHOULD BE GREATER THAN 25");
    goto Label;
   }
  }
  else
  {
   if(*yy>(tm.tm_year+1900)){ 
    printf("\n\t SOMETHING'S NOT RIGHT!! ");
    goto Label;
   }
   if(*yy==(tm.tm_year+1900))
   {
    if(*mm==(tm.tm_mon+1))
    {
     if(*dd>tm.tm_mday)
     {
      printf("\n\t SOMETHING'S NOT RIGHT!! ");
      goto Label;
     }
    } 
    else
    {
     if(*mm>(tm.tm_mon+1)){
     printf("\n\t SOMETHING'S NOT RIGHT!! ");
     goto Label;
    }
   }
  }
 }
}

void backup()
{
 struct stat st = {0};
 struct passwd *p = getpwuid(getuid());
 char bkp_dir[50],cmd[50];
 if(p!=NULL)
  sprintf(bkp_dir,"/home/%s/bkp_project_006_pjt", p->pw_name);
 if (stat(bkp_dir, &st) == -1) 
  mkdir(bkp_dir, 0700);
 sprintf(cmd,"cp Aishwary_Kumar.c %s",bkp_dir);
 system(cmd);
}


void textbackground(int color){
      switch(color%16){
          case BLACK:          bgc=40;break;
          case BLUE:           bgc=44;break;
          case GREEN:          bgc=42;break;
          case CYAN:           bgc=46;break;
          case RED:            bgc=41;break;
          case MAGENTA:        bgc=45;break;
          case BROWN:          bgc=43;break;
          case LIGHTGRAY:      bgc=47;break;
          case DARKGRAY:       bgc=40;break;
          case LIGHTBLUE:      bgc=44;break;
          case LIGHTGREEN:     bgc=42;break;
          case LIGHTCYAN:      bgc=46;break;
          case LIGHTRED:       bgc=41;break;
          case LIGHTMAGENTA:   bgc=45;break;
          case YELLOW:         bgc=43;break;
          case WHITE:          bgc=47;break;
        }
      }



void textcolor(int color){  
      switch(color%16){
          case BLACK:          printf("\033[0;%d;%dm",30,bgc);break;
          case BLUE:           printf("\033[0;%d;%dm",34,bgc);break;
          case GREEN:          printf("\033[0;%d;%dm",32,bgc);break;
          case CYAN:           printf("\033[0;%d;%dm",36,bgc);break;
          case RED:            printf("\033[0;%d;%dm",31,bgc);break;
          case MAGENTA:        printf("\033[0;%d;%dm",35,bgc);break;
          case BROWN:          printf("\033[0;%d;%dm",33,bgc);break;
          case LIGHTGRAY:      printf("\033[0;%d;%dm",37,bgc);break;
          case DARKGRAY:       printf("\033[0;%d;%dm",30,bgc);break;
          case LIGHTBLUE:      printf("\033[0;%d;%dm",34,bgc);break;
          case LIGHTGREEN:     printf("\033[0;%d;%dm",32,bgc);break;
          case LIGHTCYAN:      printf("\033[0;%d;%dm",36,bgc);break;
          case LIGHTRED:       printf("\033[0;%d;%dm",31,bgc);break;
          case LIGHTMAGENTA:   printf("\033[0;%d;%dm",35,bgc);break;
          case YELLOW:         printf("\033[0;%d;%dm",33,bgc);break;
          case WHITE:          printf("\033[0m");break;
        }
      }


    int wherexy(int *x,int *y){
      printf("\033[6n");
      if(getch() != '\x1B') return 0;
      if(getch() != '\x5B') return 0;
      int in;
      int ly = 0;
      while((in = getch()) != ';')
      ly = ly * 10 + in - '0';	
      int lx = 0;
      while((in = getch()) != 'R')
      lx = lx * 10 + in - '0';
      *x = lx;
      *y = ly;
      }

    int wherex(){
      int x=0,y=0;
      wherexy(&x,&y);
      return x;
      }
      
    int wherey(){
      int x=0,y=0;
      wherexy(&x,&y);
      return y;
      }


    int kbhit(){
      struct termios oldt, newt;
      int ch;
      int oldf;    
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
      fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);    
      ch = getchar();    
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      fcntl(STDIN_FILENO, F_SETFL, oldf);    
      if(ch != EOF){
	ungetc(ch, stdin);
	return 1;
	}    
      return 0;
      }


#define backup		backup
#define clrscr          clrscr
#define getch           getch
#define getche          getche
#define getpassword     getpassword
#define gotoxy          gotoxy
#define readint         readint
#define readdate        readdate
#define strupr		strupr
#define textcolor       textcolor
#define textbackground  textbackground


#define cprintf printf
#define cscanf scanf
#define cgets gets
  
#define wherex          wherex
#define wherey          wherey
#define ungetch         ___CONIO_H.ungetch  
#define kbhit           kbhit
#define putch           ___CONIO_H.putch
#define putchar         ___CONIO_H.putch
#define cputs           ___CONIO_H.cputs
#define clreol          ___CONIO_H.clreol
#define insline         ___CONIO_H.insline
#define delline         ___CONIO_H.delline
#define gettext         ___CONIO_H.gettext


//DEV C++ only have the next funtions (version: 4.9.9.2)
#define _cprintf        cprintf
#define _cscanf         cscanf
#define _cputs          cputs
#define _getche         getche
#define _kbhit          kbhit
#define _putch          putch
#define _ungetch        ungetch

#endif
