#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "functions.h"
#include <dirent.h>

struct Credentials
{
 char username[20];
 char password[20];
}c;

struct Personal
{
 char Name[50],state[20],FID[20];
 int Dep,gen,ctg,yy,mm,dd,jyy,jmm,jdd,hn;
 char lndmrk[20],city[20],dst[20],strt[20]; 
}p;

struct Qualification
{
 char deg[20];
 char spec[20];
 int yy;
 char noc[50];
 char nou[50];
 char grades[6];
}q;

struct Workshop
{
 char workshop_title[40];
 int duration;
 char venue[30];
 char organizer[50];
 int start_d,start_m,start_y;
 int end_d,end_m,end_y;
}w;

struct conf_paper
{
 char authors[10][50];
 char title[50];
 char doc[500];
 int dd,mm,yy;
}cp,jp;

struct patent
{
 char inventors[5][50];
 char title[50];
 char app_no[15];
 char cof[15];
 char status[25];
 int dd,mm,yy;
 int yof;
}pat;

struct author_details
{
 char title[50];
 char authors[5][50];
 int nop,dd,mm,yy,cdd,cmm,cyy,pages;
}ad;

struct outside_world
{
 int dd,mm,yy,duration;
 char details[200];
}ow;

void qual_details(int);
void workshop_details();
void create_file(char*);
void new_faculty_entry();
void paper_conf();
void paper_journal();
void show_personal();
void show_personal(char []); 
void show_workshop(char []);
void show_conf_paper(char []); 
void show_journal_paper(char []); 
void show_patent(char []); 
void show_outside_world(char []); 
void show_author(char []);
int check_fid(char []);

int row=0,col=0,xmid=0;
static int cur_yy=0,cur_mm=0,cur_dd=0;

void findterminal()
{
 struct winsize w;
 ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
 row = w.ws_row;
 col = w.ws_col;
 xmid = row/2;
}

void print(char *s)
{
 int Remaining = col - 10;
 int Left = Remaining / 2;
 printf("%*s%s",Left,"",s);
}

void print_head(char *s)
{
 int Remaining = col - strlen(s);
 int Left = Remaining / 2;
 printf("%*s%s",Left,"",s);
}

void set_head(char *h)
{ 
  int i;
  char patt[37]="";
  for(i=0;i<35;i++) strcat(patt,"=");
  gotoxy(0,xmid-19);
  print_head(patt);
  gotoxy(0,xmid-18);
  printf("%s",BOLD);
  print_head(h);
  textcolor(WHITE);
  gotoxy(0,xmid-17);
  print_head(patt);  
}


void create_directories()
{
 struct stat st = {0};
 if (stat("f_details", &st) == -1) 
  mkdir("f_details", 0700);
 if (stat("workshop", &st) == -1) 
  mkdir("workshop", 0700);
 if (stat("conf_paper", &st) == -1) 
  mkdir("conf_paper", 0700);
 if (stat("journal_paper", &st) == -1) 
  mkdir("journal_paper", 0700);
 if (stat("patent", &st) == -1) 
  mkdir("patent", 0700);
 if (stat("author", &st) == -1) 
  mkdir("author", 0700);
 if (stat("outside_world_interaction", &st) == -1) 
  mkdir("outside_world_interaction", 0700);
}


char add[20]="";
void new_faculty_entry()
{
 create_directories();
 FILE *fp;
 char ch,FID[10],name[20],eqch;
 int Dep;

 do
 { 
  clrscr();
  gotoxy(72,10);
  set_head("PERSONAL DETAILS");
  printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
  label:
  fflush(stdin);
  scanf(" %[^\n]s",FID);
  if(((FID[0]!='n' &&  FID[0]!='N') || (FID[1]!='i' && FID[1]!='I') || (FID[2]!='T' && FID[2]!='t') || (FID[3]!='t' && FID[3]!='T') || (FID[4]!='E' && FID[4]!='e')) || 
     ((isdigit(FID[6]==0) || isdigit(FID[7])==0 || isdigit(FID[8])==0 || isdigit(FID[9])==0)) || (strlen(FID)<10 || strlen(FID)>10))
  {
   printf("\n\t INVALID FID");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto label;
  }
  sprintf(add,"f_details/%s",strupr(FID));
  if((fp=fopen(add,"r"))!=NULL)
  {
   printf("\n\t THE FID ALREADY EXITS ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto label;
  }
  if(fp!=NULL) fclose(fp);
  
  printf("\n\n\n\t ENTER THE NAME OF FACULTY : ");
  scanf(" %[^\n]s",name);

  printf("\n\t DEPARTMENT ");
  printf("\n\t 1. AERONAUTICAL \n\t 2. CIVIL \n\t 3. CSE \n\t 4. ECE \n\t 5. EEE \n\t 6. ISE \n\t 7. MECH ");
  printf("\n\t ENTER THE DEPARTMENT: ");
  deplabel:
  Dep = readint();
  if(Dep!=1 && Dep!=2 && Dep!=3 && Dep!=4 && Dep!=5 && Dep!=6 && Dep!=7)
  {
   printf("\n\t WRONG INPUT! ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto deplabel;
  }
  p.Dep=Dep;

  printf("\n\t GENDER (1. MALE OR 2. FEMALE): ");
  glabe:
  p.gen = readint();
  if(p.gen!=1 && p.gen!=2)
  {
   printf("\n\t WRONG INPUT ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto glabe;
  }
  printf("\n\t CATEGORY ");
  printf("\n\t 1. GENERAL \n\t 2. SC/ST \n\t 3. OBC \n\t 4. MINORITY \n\t 5. OTHERS ");
  printf("\n\t ENTER CATEGORY: ");
  clabel:
  p.ctg = readint();
  if(p.ctg!=1 && p.ctg!=2 && p.ctg!=3 && p.ctg!=4 && p.ctg!=5)
  {
   printf("\n\t WRONG INPUT ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto clabel;
  }
 
  printf("\n\t ENTER DATE OF BIRTH: ");
  readdate(&p.dd,&p.mm,&p.yy,0);

  printf("\n\t ENTER DATE OF JOINING: ");
  readdate(&p.jdd,&p.jmm,&p.jyy,1);
  
  printf("\n\t ENTER ADDRESS ");
  printf("\n\t HOUSE NUMBER/PLOT NUMBER: ");
  p.hn = readint();
  printf("\n\t STREET: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.strt);
  printf("\n\t LANDMARK: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.lndmrk);
  printf("\n\t CITY: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.city);
  printf("\n\t DISTRICT: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.dst);
  printf("\n\t STATE: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.state);
  strcpy(p.Name,name);
  strcpy(p.FID,FID);
  
  create_file("f_details/");  
  create_file("author/");
  create_file("conf_paper/");
  create_file("journal_paper/");
  create_file("outside_world_interaction/");
  create_file("patent/");
  create_file("workshop/");
  fp=fopen(add,"wb");
  fwrite(&p,sizeof(p),1,fp);
  fclose(fp);
  getch();
 
  clrscr();
  gotoxy(72,10);
  set_head("EDUCATIONAL QUALIFICATION");
  printf("\n\n\n\t UNDERGRADUATE: ");
  qual_details(0);
  printf("\n\n\n\t POSTRGRADUATE(Y/N): ");
  scanf(" %c",&eqch);
  if(eqch=='Y' || eqch=='y')
  {
   qual_details(0);
   printf("\n\n\n\t PhD(Y/N): ");
   scanf(" %c",&eqch);
   if(eqch=='Y' || eqch=='y')
    qual_details(0);
  }
  
  printf("\n\t DO YOU WANT TO ADD MORE DATA?(Y/N): ");
  scanf(" %c",&ch);
 }while(ch=='Y' || ch=='y');
}


void create_file(char *dir)
{
 char loc[40]="";
  
 sprintf(loc,"%s%s",dir,p.FID);
 if(access(loc,F_OK))
 {
  FILE *fp = fopen(loc,"w");
  fclose(fp);
  if (fp==NULL) perror(loc);
 }
}
 
void qual_details(int t)
{
 int size = sizeof(q);
 static int n=0;
 int dnw=0;
 FILE *fp;
 if(t) 
 {
  fp=fopen(add,"r+");
  if(n>=3) n=0;
  fseek(fp,(sizeof(p)+(n*sizeof(q))),SEEK_SET);
  if(!fread(&q,sizeof(q),1,fp)) dnw=1;
 }
 printf("\n\t Name of Degree: ");
 scanf(" %[^\n]s",q.deg);
 printf("\n\t Specialization: ");
 scanf(" %[^\n]s",q.spec);
 printf("\n\t Year of Passing: ");
 label:
 q.yy = readint();
 if(q.yy<2000 || q.yy>2016)
 {
  printf("\n\t WRONG INPUT ");
  printf("\n\t PLEASE ENTER YEAR AGAIN: ");
  goto label;
 }
 printf("\n\t NAME OF COLLEGE: ");
 scanf(" %[^\n]s",q.noc);
 printf("\n\t NAME OF UNIVERSITY: ");
 scanf(" %[^\n]s",q.nou);
 printf("\n\t %% OF MARKS/GRADES : ");
 scanf(" %[^\n]s",q.grades);

 if(!t)
 {
  fp=fopen(add,"ab+");
  fwrite(&q,sizeof(q),1,fp);
  fclose(fp); 
 }
 else
 {
  n++;
  if(!dnw) fseek(fp,-size,SEEK_CUR);
  fwrite(&q,sizeof(q),1,fp);
  fclose(fp);
 }
}

int check_fid(char FID[])
{
 char ch;
 if(((FID[0]!='n' &&  FID[0]!='N') || (FID[1]!='i' && FID[1]!='I') || (FID[2]!='T' && FID[2]!='t') || (FID[3]!='t' && FID[3]!='T') || (FID[4]!='E' && FID[4]!='e')) || 
    ((isdigit(FID[6]==0) || isdigit(FID[7])==0 || isdigit(FID[8])==0 || isdigit(FID[9])==0)) || (strlen(FID)<10 || strlen(FID)>10))
 {
  printf("\n\t INVALID FID");
  printf("\n\t PLEASE ENTER AGAIN: ");
  getch();
  return 0;
 }
 sprintf(add,"f_details/%s",strupr(FID));
 
 if(access(add,F_OK))
 {
  printf("\n\t ENTERED FID DOESNOT EXIST IN OUR DATABASE");
  printf("\n\t DO YOU WANT TO CREATE ONE? (Y/N) ");
  scanf(" %c",&ch);
  if(ch=='y' || ch=='Y')
  { new_faculty_entry(); return 1; }
  else
  { printf("\n\t THANK YOU!"); getch(); return 2; }
 }
 return 1;
}


void workshop_details()
{
 char FID[20]="",temp[20]="";
 FILE *fp;
 int flag;

 clrscr();
 gotoxy(72,10);
 set_head("WORKSHOP ATTENDED");
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  printf("\n\t WORKSHOP TITLE: ");
  scanf(" %[^\n]s",w.workshop_title);
  printf("\n\t DURATION: ");
  l:
  w.duration = readint();
  if(w.duration<=0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l;
  }
  printf("\n\t FROM: ");
  readdate(&w.start_d,&w.start_m,&w.start_y,1);
  printf("\n\t TO: ");
  readdate(&w.end_d,&w.end_m,&w.end_y,1);
  printf("\n\t WORKSHOP VENUE: ");
  scanf(" %[^\n]s",w.venue);
  printf("\n\n\n\t ORGANIZER: ");
  scanf(" %[^\n]s",w.organizer);  
  sprintf(temp,"workshop/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&w,sizeof(w),1,fp);
  fclose(fp); 
 }
}

void paper_conf()
{ 
 char FID[20]="",add[20]="";
 FILE *fp,*fr;
 int flag,i,n;
 char temp[20]="";

 clrscr();
 gotoxy(72,10);
 set_head("PAPER PUBLICATION DETAILS IN INTERNATIONAL CONFERENCES");
 printf("\n\n\n\n\t Enter Faculty Identification Number(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  sprintf(temp,"f_details/%s",FID);
  fr=fopen(temp,"r");
  fread(&p,sizeof(p),1,fr);

  printf("\n\t TITLE: ");
  scanf(" %[^\n]s",cp.title);
  printf("\n\t AUTHOR(s) : %s\n",p.Name);
  strcpy(cp.authors[0],p.Name);
  fclose(fr);
  printf("\n\t NUMBER OF CO-AUTHOR(s): ");
  l:
  n = readint();
  if(n<0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l;
  }
  for(i=0;i<n;i++)
  {
   printf("\n\t NAME OF CO-AUTHOR %d: ",i+1);
   scanf(" %[^\n]s",cp.authors[i+1]);
  }
  cp.authors[i+1][0]='\0';
  printf("\n\t DETAILS OF CONFERENCE: ");
  scanf(" %[^\n]s",cp.doc);
  printf("\n\t DATE OF RECIEVING PUBLICATION CERTIFICATE"); 
  getch();
  readdate(&cp.dd,&cp.mm,&cp.yy,1);
  sprintf(temp,"conf_paper/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&cp,sizeof(cp),1,fp);
  fclose(fp); 
 }
}


void paper_journal()
{ 
 char FID[20]="",add[20]="";
 FILE *fp,*fr;
 int flag,i,n;
 char temp[20]="";

 clrscr();
 gotoxy(72,10);
 set_head("PAPER PUBLICATION DETAILS IN JOURNALS");
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  sprintf(temp,"f_details/%s",FID);
  fr=fopen(temp,"r");
  fread(&p,sizeof(p),1,fr);

  printf("\n\t TITLE: ");
  scanf(" %[^\n]s",jp.title);
  printf("\n\t AUTHOR(s): %s\n",p.Name);
  strcpy(jp.authors[0],p.Name);
  fclose(fr);
  printf("\n\t NUMBER OF CO-AUTHOR(s): ");
  l:
  n = readint();
  if(n<0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l;
  }
  for(i=0;i<n;i++)
  {
   printf("\n\t NAME OF CO-AUTHOR %d: ",i+1);
   scanf(" %[^\n]s",jp.authors[i+1]);
  }
  jp.authors[i+1][0]='\0'; 
  printf("\n\t DETAILS OF JOURNAL: ");
  scanf(" %[^\n]s",jp.doc);
  printf("\n\t DATE OF RECIEVING PUBLICATION CERTIFICATE"); 
  getch();
  readdate(&jp.dd,&jp.mm,&jp.yy,1);
  sprintf(temp,"journal_paper/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&jp,sizeof(jp),1,fp);
  fclose(fp); 
 }
}



void patent()
{ 
 char FID[20]="",add[20]="";
 FILE *fp,*fr;
 int flag,i,n,pc=0;
 char temp[20]="";

 clrscr();
 gotoxy(72,10);
 set_head("PATENT DETAILS");
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  sprintf(temp,"f_details/%s",FID);
  fr=fopen(temp,"r");
  fread(&p,sizeof(p),1,fr);
  
  printf("\n\t INVENTOR: %s\n",p.Name);
  strcpy(pat.inventors[0],p.Name);
  fclose(fr);
  printf("\n\t NUMBER OF CO-INVENTORS(s): ");
  l:
  n = readint();
  if(n<0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l;
  }
  for(i=0;i<n;i++)
  {
   printf("\n\t NAME OF CO-INVENTOR %d: ",i+1);
   scanf(" %[^\n]s",pat.inventors[i+1]);
  }
  pat.inventors[i+1][0]='\0'; 

  printf("\n\t COUNTRY OF FILING: ");
  scanf(" %[^\n]s",pat.cof);
  printf("\n\t PATENT/APPLICATION NUMBER: ");
  scanf(" %[^\n]s",pat.app_no);
  printf("\n\t TITLE: ");
  scanf(" %[^\n]s",pat.title);
  printf("\n\t STATUS: ");
  scanf(" %[^\n]s",pat.status);
  strcpy(pat.status,strupr(pat.status));
  if(strstr(pat.status,"PUBLISHED"))
  {
   printf("\n\t ON?? \n"); 
   getch();
   pc=1;
   readdate(&pat.dd,&pat.mm,&pat.yy,1);
  }
   
  l1:
  printf("\n\t YEAR OF FILING: "); 
  pat.yof=readint();
  if((pat.yof>cur_yy) || (pat.yof<1500))
  {
   printf("\n\t SOMETHING'S NOT RIGHT!! ");
   goto l1;
  }
  
  if(pc==1)
   if(pat.yof>pat.yy)
   {
    printf("\n\t SOMETHING'S NOT RIGHT!! ");
    goto l;
   }

  sprintf(temp,"patent/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&pat,sizeof(pat),1,fp);
  fclose(fp); 
 }
}


void world_interaction()
{ 
 char FID[20]="";
 FILE *fp;
 int flag,i,n;
 char temp[40]="";

 clrscr();
 gotoxy(72,10);
 set_head("INTERACTION WITH OUTSIDE WORLD DETAILS");
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  printf("\n\t DATE: "); 
  getch();
  readdate(&ow.dd,&ow.mm,&ow.yy,1);
  
  printf("\n\t DURATION: ");
  ow.duration = readint();

  printf("\n\t DETAILS: ");
  scanf(" %[^#]s",ow.details);
  
  sprintf(temp,"outside_world_interaction/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&ow,sizeof(ow),1,fp);
  fclose(fp); 
 }
}


void author_det()
{ 
 char FID[20]="",add[20]="";
 FILE *fp,*fr;
 int flag,i,n;
 char temp[20]="";

 clrscr();
 gotoxy(72,10);
 set_head("BOOK AUTHORING DETAILS");
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 flag=check_fid(FID);
 if(!flag)
  goto label;
 if(flag==1)
 {
  sprintf(temp,"f_details/%s",FID);
  fr=fopen(temp,"r");
  fread(&p,sizeof(p),1,fr);

  printf("\n\t TITLE OF THE BOOK: ");
  scanf(" %[^\n]s",ad.title);

  printf("\n\t PRINCIPAL AUTHOR: %s\n",p.Name);
  strcpy(ad.authors[0],p.Name);
  fclose(fr);

  printf("\n\t NUMBER OF CO-AUTHOR(s): ");
  l:
  n = readint();
  if(n<0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l;
  }
  for(i=0;i<n;i++)
  {
   printf("\n\t NAME OF CO-AUTHOR %d: ",i+1);
   scanf(" %[^\n]s",ad.authors[i+1]);
  }
  ad.authors[i+1][0]='\0'; 

  printf("\n\t TOTAL NUMBER OF PAGES IN THE BOOK: ");
  l1:
  ad.pages=readint();
  if(ad.pages<=0)
  {
   printf("\n\t SOMETHING'S NOT RIGHT");
   goto l1;
  }

  printf("\n\t DATE OF COMMENCEMENT OF THE BOOK WRITING "); 
  getch();
  readdate(&ad.dd,&ad.mm,&ad.yy,1);
  
  printf("\n\t DATE OF COMPLETION OF THE BOOK WRITING"); 
  getch();
  readdate(&ad.cdd,&ad.cmm,&ad.cyy,1);
 
  sprintf(temp,"author/%s",FID);
  fp=fopen(temp,"ab+");
  fwrite(&ad,sizeof(ad),1,fp);
  fclose(fp); 
 }
}

void show()
{
 FILE *fp;
 DIR *d;
 struct dirent *dir;
 int l,c=13,check=0;
 char temp[30],FID[20],flag=0;

 d = opendir("f_details/");
 if(d)
 {
  clrscr();
  gotoxy(5,9);
  for(l=0;l<79;l++) printf("=");
  gotoxy(15,10);
  printf(" FID \t\t\t\t\t\t NAME ");
  gotoxy(5,11);
  for(l=0;l<79;l++)  printf("=");

  while ((dir = readdir(d)) != NULL)
  {
   if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
   { 
    sprintf(temp,"f_details/%s",dir->d_name);
    fp=fopen(temp,"r");
    fread(&p,sizeof(p),1,fp);
    gotoxy(13,c);
    printf("%s \t\t\t\t\t\t %s",p.FID,p.Name);
    c++;
    fclose(fp);
    flag=1;
   }
  }
  if(flag==0)
  {
   gotoxy(13,13);
   printf(" NO DATA TO DISPLAY! ");
   getch(); getch();
   return;
  }
 
  printf("\n\n\n\t ENTER FID FOR DETAILED INFORMATION: ");
  label: 
  scanf(" %[^\n]s",FID);
  check=check_fid(FID);
  if(!check)
   goto label;
  if(check==1)
  {
   for(;;)
   {
    int ch;
    clrscr();
    set_head("VIEWS");
    printf("\n\n\n\n\n\n\t 1. PERSONAL DETAILS \n\n\t 2. WORKSHOP ATTENDED \n\n\t 3. PAPER PUBLICATION DETAILS IN INTERNATIONAL CONFERENCES \n\n\t 4. PAPER PUBLICATION DETAILS IN JOURNALS ");
    printf("\n\n\t 5. PATENT DETAILS \n\n\t 6. BOOK AUTHORING DETAILS \n\n\t 7. INTERACTION WITH OUTSIDE WORLD \n\n\t 8. ALL \n\n\t 9. EXIT"); 
    printf("\n\n\t CHOICE: ");
    ch=readint();
    switch(ch)
    {
     case 8 : show_personal(FID); show_workshop(FID); show_conf_paper(FID); show_journal_paper(FID); show_patent(FID); show_author(FID); show_outside_world(FID); break; 
     case 1 : show_personal(FID); break;
     case 2 : show_workshop(FID); break; 
     case 3 : show_conf_paper(FID); break; 
     case 4 : show_journal_paper(FID); break; 
     case 5 : show_patent(FID); break; 
     case 7 : show_outside_world(FID); break; 
     case 6 : show_author(FID); break; 
     case 9 : return; 
     default: textcolor(RED); 
	      printf("\n\n\t INVALID INPUT");
              textcolor(WHITE); 
	      getchar();
	      break;
    }
   } 
  }
  else
  {
   textcolor(RED);
   printf("\n\t SORRY!! NO DATA TO DISPLAY ");
   textcolor(WHITE);
  }
 }
 closedir(d);
 getch();
}

void show_personal(char f[])
{
 FILE *fp;
 char path[20],temp[50]="";
 clrscr();
 set_head("PERSONAL DETAILS");
 sprintf(path,"f_details/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");
 if(fread(&p,sizeof(p),1,fp))
 {
  printf("\n\n\n\n\t %-38s%-10s%s","FACULTY IDENTIFICATION NUMBER(FID)",":",p.FID);
  printf("\n\n\n\t %-38s%-10s%s","NAME OF FACULTY",":",p.Name);
 
  printf("\n\n\t %-38s%-10s","Department",":");
  switch(p.Dep)
  {
   case 1: strcpy(temp,"AERONAUTICAL"); break;
   case 2: strcpy(temp,"CIVIL"); break;
   case 3: strcpy(temp,"CSE"); break;
   case 4: strcpy(temp,"ECE"); break;
   case 5: strcpy(temp,"EEE"); break;
   case 6: strcpy(temp,"ISE"); break;
   case 7: strcpy(temp,"MECH"); break;
  }
  printf("%s",temp);
  
  if(p.gen==1) printf("\n\n\t %-38s%-10s%s","SEX",":","MALE");
  else printf("\n\n\t %-38s%-10s%s","SEX",":","MALE");
  
  printf("\n\n\t %-38s%-10s","CATEGORY",":");
  switch(p.ctg)
  {
   case 1: strcpy(temp,"GENERAL"); break;
   case 2: strcpy(temp,"SC/ST"); break;
   case 3: strcpy(temp,"OBC"); break;
   case 4: strcpy(temp,"MINORITY"); break;
   case 5: strcpy(temp,"OTHERS"); break;
  }
  printf("%s",temp);
  
  printf("\n\n\t %-38s%-10s%d.%d.%d","DATE OF BIRTH",":",p.dd,p.mm,p.yy);
  printf("\n\n\t %-38s%-10s%d.%d.%d","DATE OF JOINING",":",p.jdd,p.jmm,p.jyy);
   
  printf("\n\n\t %-38s%-10s%d,%s","ADDRESS",":",p.hn,p.strt);
  printf("\n\t %*s%*sNear %s",-38,"",-10,"",p.lndmrk);
  printf("\n\t %*s%*s%s,%s",-38,"",-10,"",p.city,p.dst);
  printf("\n\t %*s%*s%s",-38,"",-10,"",p.state);
  getch();
  getch();
 
  fseek(fp,sizeof(p),SEEK_SET);
  int counter=1;
  clrscr();
  set_head("EDUCATIONAL QUALIFICATION");
  while(fread(&q,sizeof(q),1,fp))
  {
   if(counter==1) printf("\n\n\n\t UNDERGRADUATE \n");
   if(counter==2) printf("\n\n\n\t POSTRGRADUATE \n");
   if(counter==3) printf("\n\n\n\t PHD \n");
   printf("\n\n\t %-38s%-10s%s","NAME OF DEGREE",":",q.deg);
   printf("\n\n\t %-38s%-10s%s","SPECIALIZATION",":",q.spec);
   printf("\n\n\t %-38s%-10s%d","YEAR OF PASSING",":",q.yy);
   printf("\n\n\t %-38s%-10s%s","NAME OF COLLEGE",":",q.noc);
   printf("\n\n\t %-38s%-10s%s","NAME OF UNIVERSITY",":",q.nou);
   printf("\n\n\t %-38s%-10s%s","%% OF MARKS/GRADES",":",q.grades);
   counter++;
  }
  if(counter==1) { textcolor(RED); printf("\n\t SORRY!! NO DATA TO DISPLAY "); textcolor(WHITE); }
  fclose(fp);
  getch();
 }
 else
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
  fclose(fp);
 }
}


void show_workshop(char f[])
{
 FILE *fp;
 char path[20],temp[50]="";
 int flag=0;
 clrscr();
 set_head("WORKSOP ATTENDED DETAILS");
 sprintf(path,"workshop/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) { perror(path); exit(0); }
 printf("\n\n\n");
 while(fread(&w,sizeof(w),1,fp))
 {
  flag=1;
  printf("\n\n\n\n");  
  printf("\n\t %-38s%-15s%s","WORKSHOP TITLE",":",w.workshop_title);
  printf("\n\t %-38s%-15s%d","DURATION",":",w.duration);
  printf("\n\t %-38s%-15s%d.%d.%d","FROM",":",w.start_d,w.start_m,w.start_y);
  printf("\n\t %-38s%-15s%d.%d.%d","TO",":",w.end_d,w.end_m,w.end_y);
  printf("\n\t %-38s%-15s%s","WORKSHOP TITLE",":",w.venue);
  printf("\n\t %-38s%-15s%s","ORGANIZER",":",w.organizer);
  printf("\n\n");
 }
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}
 
void show_conf_paper(char f[])
{
 FILE *fp;
 char path[20]="";
 int i,flag=0;
 clrscr();
 set_head("PAPER PUBLICATION DETAILS IN INTERNATIONAL CONFERENCES");
 sprintf(path,"conf_paper/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");
 while(fread(&cp,sizeof(cp),1,fp))
 {
  i=1;
  flag=1;
  printf("\n\n\n\n");
  printf("\n\t %-50s%-15s%s","TITLE",":",cp.title);
  printf("\n\t %-50s%-15s%s","AUTHOR",":",cp.authors[0]);
  printf("\n\t %-50s%-15s","NAME OF CO-AUTHOR(s)",":");
 
  while(cp.authors[i][0]!='\0')
   if(i==1) printf("%d. %s",i++,cp.authors[i]); else printf("\n\t %*s%*s%d. %s",-50,"",-15,"",i++,cp.authors[i]);

  printf("\n\t %-50s%-15s%s","DETAILS OF CONFERENCE",":",cp.doc);
  printf("\n\t %-50s%-15s%d.%d.%d","DATE OF RECIEVING PUBLICATION CERTIFICATE",":",cp.dd,cp.mm,cp.yy);
  printf("\n\n");
 }
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}
 

void show_journal_paper(char f[])
{
 FILE *fp;
 char path[20];
 int i,flag=0;
 clrscr();
 set_head("PAPER PUBLICATION DETAILS IN JOURNALS");
 sprintf(path,"journal_paper/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");
 while(fread(&jp,sizeof(jp),1,fp))
 {
  i=1;
  flag=1;
  printf("\n\n\n\n");
  printf("\n\t %-50s%-15s%s","TITLE",":",jp.title);
  printf("\n\t %-50s%-15s%s","AUTHOR",":",jp.authors[0]);
  printf("\n\t %-50s%-15s","NAME OF CO-AUTHOR(s)",":");
 
  while(jp.authors[i][0]!='\0')
   if(i==1) printf("%d. %s",i++,jp.authors[i]); else printf("\n\t %*s%*s%d. %s",-50,"",-15,"",i++,jp.authors[i]);

  printf("\n\t %-50s%-15s%s","DETAILS OF CONFERENCE",":",jp.doc);
  printf("\n\t %-50s%-15s%d.%d.%d","DATE OF RECIEVING PUBLICATION CERTIFICATE",":",jp.dd,jp.mm,jp.yy);
  printf("\n\n");
 } 
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}


//////////////////////////////////////////////////////////*******************wh***************************//////////////////////////////////////////
void show_patent(char f[])
{
 FILE *fp;
 char path[20];
 int i,flag=0;
 clrscr();
 set_head("PATENT DETAILS");
 sprintf(path,"patent/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");
 while(fread(&pat,sizeof(pat),1,fp))
 {
  i=1;
  flag=1;
  printf("\n\n\n\n");
  printf("\n\t %-40s%-10s%s","INVENTOR",":",pat.inventors[0]);
  printf("\n\t %-40s%-10s","NAME OF CO-INVENTOR(s)",":");

  while(pat.inventors[i][0]!='\0')
   if(i==1) printf("%d. %s",i++,pat.inventors[i]); else printf("\n\t %*s%*s%d. %s",-40,"",-10,"",i++,pat.inventors[i]);

  printf("\n\t %-40s%-10s%s","COUNTRY OF FILING",":",pat.cof);
  printf("\n\t %-40s%-10s%s","PATENT/APPLICATION NUMBER",":",pat.app_no);
  printf("\n\t %-40s%-10s%s","TITLE",":",pat.title);
  printf("\n\t %-40s%-10s%s","STATUS",":",pat.status);

  if(strstr(pat.status,"PUBLISHED"))
   printf("\n\t %-40s%-10s%d.%d.%d","ON",":",pat.dd,pat.mm,pat.yy);
   
  printf("\n\t %-40s%-10s%d","YEAR OF FILING",":",pat.yof);
  printf("\n\n");
 } 
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}

void show_outside_world(char f[])
{
 FILE *fp;
 char path[40];
 int flag=0;
 clrscr();
 set_head("INTERACTION WITH OUTSIDE WORLD DETAILS");
 sprintf(path,"outside_world_interaction/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");
 while(fread(&ow,sizeof(ow),1,fp))
 {
  flag=1;
  printf("\n\n\n\n");
  printf("\n\t %-35s%-10s%d.%d.%d","DATE",":",ow.dd,ow.mm,ow.yy);
  printf("\n\t %-35s%-10s%d","DURATION",":",ow.duration);
  printf("\n\t %-35s%-10s%s","DETAILS",":",ow.details);
 }
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}

void show_author(char f[])
{
 FILE *fp;
 char path[20];
 int i,flag=0;
 clrscr();
 set_head("BOOK AUTHORING DETAILS");
 sprintf(path,"author/%s",strupr(f));
 fp=fopen(path,"r"); 
 if(fp==NULL) {perror(path); exit(0); }
 printf("\n\n\n");

 while(fread(&ad,sizeof(ad),1,fp))
 {
  flag=1;
  i=1;
  printf("\n\n\n\n");
  printf("\n\t %-50s%-10s%s","TITLE OF THE BOOK",":",ad.title);
  printf("\n\t %-50s%-10s%s","PRINCIPAL AUTHOR",":",ad.authors[0]);
  printf("\n\t %-50s%-10s","NAME OF CO-AUTHOR(s)",":");
  
  while(ad.authors[i][0]!='\0')
   if(i==1) printf("%d. %s",i++,ad.authors[i]); else printf("\n\t %*s%*s%d. %s",-50,"",-10,"",i++,ad.authors[i]);

  printf("\n\t %-50s%-10s%d","TOTAL NUMBER OF PAGES IN THE BOOK",":",ad.pages);
  printf("\n\t %-50s%-10s%d.%d.%d","DATE OF COMMENCEMENT OF THE BOOK WRITING",":",ad.dd,ad.mm,ad.yy);
  printf("\n\t %-50s%-10s%d.%d.%d","DATE OF COMPLETION OF THE BOOK WRITING",":",ad.cdd,ad.cmm,ad.cyy);
  printf("\n\n");
 }
 if(flag==0)
 {
  textcolor(RED);
  printf("\n\t SORRY!! NO DATA TO DISPLAY ");
  textcolor(WHITE);
 }
 fclose(fp);
 getch();
}

int ask()
{
 char ch; 
 l:
 printf("UPDATE (Y/N): ");
 scanf(" %c",&ch);
 if(isdigit(ch))
 {
  printf("\n\t PLEASE ENTER A CHARACTER ");
  goto l;
 }
 ch=toupper(ch);
 return ch=='Y'?1:0;
}

void modify_pd()
{
 FILE *fp;
 char ch,FID[10],name[20],eqch;
 int Dep,flag=0;
 int size = sizeof(p);

 clrscr();
 gotoxy(72,10);
 set_head("MODIFY PERSONAL DETAILS");
 
 printf("\n\n\n\n\t ENTER FACULTY IDENTIFICATION NUMBER(FID): ");
 label:
 fflush(stdin);
 scanf(" %[^\n]s",FID);
 if(((FID[0]!='n' &&  FID[0]!='N') || (FID[1]!='i' && FID[1]!='I') || (FID[2]!='T' && FID[2]!='t') || (FID[3]!='t' && FID[3]!='T') || (FID[4]!='E' && FID[4]!='e')) || 
    ((isdigit(FID[6]==0) || isdigit(FID[7])==0 || isdigit(FID[8])==0 || isdigit(FID[9])==0)) || (strlen(FID)<10 || strlen(FID)>10))
 {
  textcolor(RED); printf("\n\t INVALID FID"); textcolor(WHITE);
  printf("\n\t PLEASE ENTER AGAIN: ");
  goto label;
 }
 sprintf(add,"f_details/%s",strupr(FID));
 if((fp=fopen(add,"r+"))==NULL)
 {
  textcolor(RED); printf("\n\t ENTERED FID DOESNOT EXIST IN OUR DATABASE !!"); textcolor(WHITE);
  getch(); getch();
  return;
 }
 show_personal(FID); 
 clrscr();
 gotoxy(72,10);
 set_head("MODIFY PERSONAL DETAILS");
 fread(&p,sizeof(p),1,fp); 
 printf("\n\n\n\t NAME OF FACULTY?? "); 
 if(ask())
 {
  printf("\n\t ENTER THE NAME OF FACULTY : ");
  scanf(" %[^\n]s",name);
  strcpy(p.Name,name);
 }
 
 
 printf("\n\t DEPARTMENT?? ");
 if(ask())
 {
  printf("\n\t 1. AERONAUTICAL \n\t 2. CIVIL \n\t 3. CSE \n\t 4. ECE \n\t 5. EEE \n\t 6. ISE \n\t 7. MECH ");
  printf("\n\t ENTER THE DEPARTMENT: ");
  deplabel:
  Dep = readint();
  if(Dep!=1 && Dep!=2 && Dep!=3 && Dep!=4 && Dep!=5 && Dep!=6 && Dep!=7)
  {
   printf("\n\t WRONG INPUT! ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto deplabel;
  }
  p.Dep=Dep;
 }


 printf("\n\t GENDER?? ");
 if(ask())
 {
  printf("\n\t GENDER (1. MALE OR 2. FEMALE): ");
  glabe:
  p.gen = readint();
  if(p.gen!=1 && p.gen!=2)
  {
   printf("\n\t WRONG INPUT ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto glabe;
  }
 }
  
 printf("\n\t CATEGORY?? ");
 if(ask())
 {
  printf("\n\t 1. GENERAL \n\t 2. SC/ST \n\t 3. OBC \n\t 4. MINORITY \n\t 5. OTHERS ");
  printf("\n\t ENTER CATEGORY: ");
  clabel:
  p.ctg = readint();
  if(p.ctg!=1 && p.ctg!=2 && p.ctg!=3 && p.ctg!=4 && p.ctg!=5)
  {
   printf("\n\t WRONG INPUT ");
   printf("\n\t PLEASE ENTER AGAIN: ");
   goto clabel;
  }
 }
  
 printf("\n\t DATE OF BIRTH?? ");
 if(ask())
 {
  printf("\n\t ENTER DATE OF BIRTH: ");
  getch();
  readdate(&p.dd,&p.mm,&p.yy,0);
 }

 printf("\n\t DATE OF JOINING?? ");
 if(ask())
 {
  printf("\n\t ENTER DATE OF JOINING: ");
  getch();
  readdate(&p.jdd,&p.jmm,&p.jyy,1);
 }
  
 printf("\n\t ADDRESS?? ");
 if(ask())
 {
  printf("\n\t ENTER ADDRESS ");
  printf("\n\t HOUSE NUMBER/PLOT NUMBER: ");
  p.hn = readint();
  printf("\n\t STREET: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.strt);
  printf("\n\t LANDMARK: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.lndmrk);
  printf("\n\t CITY: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.city);
  printf("\n\t DISTRICT: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.dst);
  printf("\n\t STATE: ");
  fflush(stdin);
  scanf(" %[^\n]s",p.state);
 }
   
 fseek(fp,-size,SEEK_CUR);
 fwrite(&p,sizeof(p),1,fp);
 fclose(fp);
 printf("\n\t MODIFIED! ");
 getch(); getch();


 clrscr();
 gotoxy(72,10);
 set_head("EDUCATIONAL QUALIFICATION");
 printf("\n\n\n\t UNDERGRADUATE?? ");
 if(ask())
  qual_details(1);
 printf("\n\n\n\t POSTRGRADUATE?? ");
 if(ask())
 {
  qual_details(1);
  printf("\n\n\n\t PhD?? ");
  if(ask())
   qual_details(1);
  } 
 
 printf("\n\t MODIFIED! ");
 getch(); getch();

}
 
void menu()
{
 int ch;
 char *s="4. FACULTY PAPER PUBLICATION DETAILS IN INTERNATIONAL CONFERENCES";
 int Remaining = col - strlen(s);
 int Left = Remaining / 2;

 for(;;)
 {
  clrscr();
  set_head("MENU");
  gotoxy(0,xmid-10);
  printf("%*s%s",Left,"","1. NEW FACULTY ENTRY");
  gotoxy(0,xmid-8);
  printf("%*s%s",Left,"","2. UPDATION OF FACULTY DETAILS");
  gotoxy(0,xmid-6);
  printf("%*s%s",Left,"","3. FACULTY WORKSHOP ATTENDED DETAILS");
  gotoxy(0,xmid-4);
  printf("%*s%s",Left,"",s);
  gotoxy(0,xmid-2);
  printf("%*s%s",Left,"","5. FACULTY PAPER PUBLICATION DETAILS IN JOURNALS");
  gotoxy(0,xmid);
  printf("%*s%s",Left,"","6. FACULTY PATENT DETAILS");
  gotoxy(0,xmid+2);
  printf("%*s%s",Left,"","7. FACULTY BOOK AUTHORING DETAILS");
  gotoxy(0,xmid+4);
  printf("%*s%s",Left,"","8. FACULTY INTERACTION WITH OUTSIDE WORLD DETAILS");
  gotoxy(0,xmid+6);
  printf("%*s%s",Left,"","9. VIEW");
  gotoxy(0,xmid+8);
  printf("%*s%s",Left,"","10. EXIT");
  gotoxy(0,xmid+10);
  printf("%*s%s",Left,"","ENTER YOUR CHOICE: ");
  ch = readint();
  switch(ch)
  {
   case 1: new_faculty_entry();
           break;
   case 2: modify_pd();
           break;
   case 3: workshop_details();
           break;
   case 4: paper_conf();
	   break;
   case 5: paper_journal();
	   break;
   case 6: patent();
	   break;
   case 7: author_det();
  	   break; 
   case 8: world_interaction();
	   break;
   case 9: show();
	   break;
   case 10: gotoxy(0,xmid+12);
            printf("%*s%s",Left,"","THANK YOU!! ");
            getchar();
            exit(0);
   default: gotoxy(0,xmid+12);
	    textcolor(RED); 
	    printf("%*s%s",Left,"","INVALID INPUT");
            textcolor(WHITE); 
	    getchar();
	    break;
   }
 }
}

void login()
{
 system("clear");
 set_head("LOGIN");
 FILE *fp;
 char u[20]="";
 char p[20]="";
 int flag=0;
 gotoxy(0,xmid-1);
 print("LOGIN ID: ");
 scanf(" %[^\n]s",u);
 gotoxy(0,xmid+1);
 getchar();
 print("PASSWORD: ");
 getpassword(p);

 fp = fopen("users.txt","r");
 if(fp == NULL)
 {
  printf(" FILE DOESNOT EXIST");
  return;
 }
 while(fread(&c,sizeof(c),1,fp))
 {
  if(((strcmp(u,c.username)==0)) && (strcmp(p,c.password)==0))
  {
   flag=1;
   break;
  }
 }
 if(flag==1)
 {
  gotoxy(0,xmid+3);
  print("LOGIN SUCCESSFULL");
  getchar() ;
  menu();
 }
 else
 {
  gotoxy(0,xmid+3);
  print("LOGIN UNSUCCESSFULL");
  getchar();
 }
 fclose(fp);
}
 
int check(char usr[])
{
 FILE *fg=fopen("users.txt","r");
 if(fg==NULL)
 return 0;
 while(fread(&c,sizeof(c),1,fg))
 {
  if(strcmp(usr,c.username)==0)
  return 1;
 }
 fclose(fg);
 return 0;
}

int o=0;
void signup()
{
  clrscr();
  set_head("SIGN UP!");
  FILE *fp;
  char pass[20],pass1[20],user[20];
  fp = fopen("users.txt","ab+");
  if(fp == 0)
  fp = fopen("users.txt","wb+");
  gotoxy(0,xmid-7);
  print("ENTER USERNAME: ");
  label:
  scanf(" %[^\n]s",user);
  if(check(user)==1)
  {
   printf("\n");
   print("USERNAME ALREADY TAKEN \n");
   printf("\n");
   print("PLEASE ENTER AGAIN: ");
   goto label;
  }
  strcpy(c.username,user);
  renter:
  printf("\n");
  print("ENTER NEW PASSWORD: ");
  if(o==0){ getch(); o++;}
  getpassword(pass);
  printf("\n");
  print("RE-ENTER PASSWORD: ");
  getpassword(pass1);
  if(strcmp(pass,pass1)==0)
   {
    strcpy(c.password,pass);
    printf("\n");
    print("SIGN UP SUCCESSFULL");
    getch();
    fwrite((char*)&c,sizeof(c),1,fp);
   }
  else
  {
   printf("\n");
   print("PASSWORD DID NOT MATCH\n");
   goto renter;
  }
  fclose(fp);
}

int main()
{
 findterminal(); 
 time_t t = time(NULL);
 struct tm tm = *localtime(&t);
 cur_yy=tm.tm_year+1900;
 cur_mm=tm.tm_mon+1 ;
 cur_dd=tm.tm_mday;

 int ch;
 for(;;)
 {
  clrscr();
  set_head("LOG IN / SIGN UP!");
  gotoxy(0,xmid-3);
  print("1. LOGIN");
  gotoxy(0,xmid-1);
  print("2. SIGN UP");
  gotoxy(0,xmid+1);
  print("3. EXIT");
  gotoxy(0,xmid+3);
  print("ENTER YOUR CHOICE: ");
  ch = readint();
  switch(ch)
  {
   case 1: login();
 	   break;
   case 2: signup();
	   break;
   case 3: gotoxy(0,xmid+5);
	   backup();
           print("THANK YOU!! ");
           getchar(); getch();
	   exit(0);
   default: gotoxy(0,xmid+5);
	    textcolor(RED); 
	    print("INVALID INPUT");
            textcolor(WHITE); 
	    getchar();
	    break;
  }
 }
 return 0;  
}
