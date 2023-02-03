#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<unistd.h>
#include<ncurses.h>
#include "progect.h"


typedef long long int ll;
typedef double db;

#define f first
#define s second
#define all(v) (v).begin(),(v).end()
#define print(x) printf("%d \n", x);
#define printll(x) printf("%lld \n", x);
#define get(n) scanf("%d", &n);
#define N 10005
#define Dis 3

int st_selectx, st_selecty;
char slct[N];


int max(int x, int y)
{
   if(x > y)
      return x;
   return y;   
}

int min(int x, int y)
{
   if(x < y)
      return x;
   return y;   
}
void swap(int *x, int *y)
{
   int tmp = *x;
   *x = *y;
   *y = tmp;
}

struct Editor
{
   int x, y, line, save, st_line;
   char mode;      // i -> insert    m | n -> noemal    v->visual   x->exit
   char* file_name; 
   char Buffer[N][N];
   char Bar[N];
};
struct Editor ed;

void curses_init()
{
   initscr();                  
   noecho();                       
   cbreak();                       
   keypad(stdscr, true);     
   start_color();      
}

void navigation(int input)
{
   if(input == 'k' || input == KEY_UP)         // up
   {
      if(ed.y == 0)
         ed.st_line = max(ed.st_line-1, 0);
      ed.y = max(0, ed.y-1);
   }
   else if(input == 'j' || input == KEY_DOWN)   //down
   {

      if(ed.y == 9)
         ed.st_line++;
      
      ed.y = min(9, ed.y+1);
      //ed.y = ed.y + 1;
      //ed.st_line = max(ed.st_line, ed.x-10);
      //printf("%d", strlen(ed.Buffer[y])+Dis-1);
   }
   else if(input == 'h' || input == KEY_LEFT)   // left
   {
      ed.x--;
   }
   else if(input == 'l' || input == KEY_RIGHT)   // righy
   {
      ed.x++;
   }
   
   int y = ed.y + ed.st_line;
   ed.x = min(ed.x, strlen(ed.Buffer[y])+Dis);
   ed.x = max(Dis, ed.x);
   //clrtoeol();
   
}

void Put_st_in_ed(char* st)
{
   for(int i = 0; i < ed.line; i++)
      memset(ed.Buffer[i], '\0', sizeof ed.Buffer[i]);
   ed.line = 0;   
   //return;
   for(int i = 0; i < strlen(st); i++)
   {
      int cnt = 0;
      while(i < strlen(st) && st[i] != '\n')
      {
         ed.Buffer[ed.line][cnt++] = st[i];
         i++;
      }
      
      ed.line++;

   }

}

void init(char *fn)
{
   ed.file_name = fn;
   ed.mode = 'n';
   ed.save = 1;
   ed.x = Dis;
   ed.Bar[0] = '\0';
   ed.st_line = ed.y = ed.line = 0;
   if(fn == NULL)
   {
      return;
   }   

   fn++;
   printf("%s\n", fn);
   FILE *fp;
   fp = fopen(fn, "r");

   char st[N] = {"\0"};
   get_all_char(st, fp);
   Put_st_in_ed(st);
   fclose (fp);
}

bool is_select()
{
   int y, x;
   getyx(stdscr, y, x);
   if(ed.mode != 'v')
      return 0;
   //int y, x;
   //getyx(stdscr, y, x);
   int mny = ed.y;
   int mxy = st_selecty;
   int mnx = ed.x;
   int mxx = st_selectx;
   if(mny > mxy)
   {
      swap(&mny, &mxy);
      swap(&mnx, &mxx);
   }
   if(mny < y && y < mxy)
      return 1;

   if(mny == mxy)
   {
      if(mnx > mxx)
         swap(&mnx, &mxx);
      return (y == mny && x >= mnx && x <= mxx);
   }   
   if(y == mny && x >= mnx)   
      return 1;
   if(y == mxy && x <= mxx)
      return 1;   
   return 0;      
}

void printBuff()
{
   init_pair(1, COLOR_GREEN, COLOR_YELLOW);
   memset(slct, '\0', sizeof slct);
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {
      //if(select())
      move(i-ed.st_line, 0);
      printw("%2d ", i);//, ed.Buffer[i]);
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
       
         if(is_select())
         {
            attron(COLOR_PAIR(1));
            addch(ed.Buffer[i][j]);
            slct[strlen(slct)] = ed.Buffer[i][j];
            if(j == strlen(ed.Buffer[i]) - 1)
               slct[strlen(slct)] = '\n';
            //printf("%d %d %d    ", y, st_selecty, ed.y);
            attroff(COLOR_PAIR(1));
         }    
         else
         {
            printw("%c", ed.Buffer[i][j]);
            //printw("%d", (ed.y >= y && y <= 2));
            //printw("%d", y);
         }
         
      }
      printw("\n");   
   }
   int y, x;
   getmaxyx(stdscr, y, x);
   //printw("%d %d", y, x);

   WINDOW * Mode = newwin(3, x-1, y-5, 0), *Bar = newwin(2, x-1, y-2, 0);          
   refresh();
   box(Mode, 0, 0);
   wattron(Mode, A_REVERSE);
   if(ed.mode == 'n' || ed.mode == 'm')
      mvwprintw(Mode, 1, 1, "NORMAL");
   else if(ed.mode == 'i')
      mvwprintw(Mode, 1, 1, "INSERT");
   else if(ed.mode == 'v')
      mvwprintw(Mode, 1, 1, "VISUAL");  
   wattroff(Mode, A_REVERSE);   
   wprintw(Mode, " %s",  ed.file_name);
   if(!ed.save)
   {
      wprintw(Mode, " +");
   }
   wrefresh(Mode);

   //box(Bar, 0, 0);
   //wattron(Bar, A_REVERSE);
   wmove(Bar, 1, 1);
   if(ed.Bar != NULL)
      wprintw(Bar, "%s", ed.Bar);
   wrefresh(Bar);
   //wattroff(Bar, A_REVERSE);
   
   if(ed.mode != 'm')
   {
      move(ed.y, ed.x);
   }
   //init_color(COLOR_RED, 20, 0, 0);
}

void insert_input(int input)
{
   ed.save = 0;
   char st[N] = {"\0"};
   int y = ed.y + ed.st_line;
   for(int i = 0; i < ed.x-Dis; i++)
   {
      st[i] = ed.Buffer[y][i];
   }
   st[ed.x-Dis] = input;
   for(int i = ed.x-Dis; i < strlen(ed.Buffer[y]); i++)
   {
      st[i+1] = ed.Buffer[y][i];
   }
   //mvprintw(10, 10, "%s\n", st);

   for(int i = 0; i < strlen(st); i++)
   {
      ed.Buffer[y][i] = st[i];
   }
   ed.x++;
   ed.line = max(ed.line, y);
}

void remove_slct()
{
   ed.save = 0;
   //if(mny < y && y < mxy)
     // return 1;
   char st[N] = {"\0"};  
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {
      bool fl = false;
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
         move(i-ed.st_line, j+Dis-1);
         if(!is_select())
         {
            st[strlen(st)] = ed.Buffer[i][j];
            fl |= (j == strlen(ed.Buffer[i])-1);
         }
      }
      if(fl)
         st[strlen(st)] = '\n';
   }
   //printf("%s", st);
   Put_st_in_ed(st);
   ed.y = 0;
   ed.x = Dis;
}

void past_clipboard()
{
   ed.save = 0;
   char st[N] = {"\0"}; 
   
   FILE *fp;
   fp = fopen("clipboard.txt", "r");
   get_all_char(st, fp);
   fclose (fp);
   
   for(int i = 0; i < strlen(st); i++)
      insert_input(st[i]);

   //ed.y = 0;
   //ed.x = Dis;

}

void get_all_ed(char* st)
{
   for(int i = 0; i < ed.line; i++)
   {
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
         st[strlen(st)] = ed.Buffer[i][j];
      st[strlen(st)] = '\n';   
   }
   return;
}



void process(char *q)
{
   if(q[0] == '/')
   {
      q++;
      //find_expression(q);
      return;
   }
   q++;
   if(strcmp(q, "save") == 0)
   {
      if(ed.file_name == NULL)
      {
         memset(ed.Bar, '\0', sizeof ed.Bar); 
         char error[] = "No filename";
         for(int i = 0; i < 11; i++)
            ed.Bar[i] = error[i];
         return;   
      }
      char st[N] = {'\0'};
      get_all_ed(st);
      //printf("%s", st);
      Put(st, ed.file_name);
      ed.save = 1;
      goto hell;
   }
   if(strlen(q) > 9 && q[0] == 's' && q[1] == 'a' && q[2] == 'v' && q[3] == 'e' && q[4] == 'a' && q[5] == 's')
   {
      char pt[N], st[N];
      for(int i = 0; i < strlen(q); i++)
         pt[i] = ed.Bar[i+9];
      creatfile(pt);
      get_all_ed(st);
      Put(st, pt);
      ed.save = 1;
      ed.file_name = pt;
      //mvprintw(10, 10, "%s\n", ed.Bar);   
      goto hell;  
   }
   if(strcmp(q, "open") == 0)
   {
      
      goto hell;
   }
   if(strcmp(q, "undo") == 0)
   {
      goto hell;
   }
   hell:
   memset(ed.Bar, '\0', sizeof ed.Bar); 
}


void handel(int input)
{
   if(ed.mode == 'v')
   {
      if(input == 'x')
      {
         ed.mode = 'x';
         return;
      }
      if(input == 'i')
      {
         ed.mode = 'i';
         return;
      }
      if(input == 27)
      {
         ed.mode = 'n';
         return;
      }
      if(input == 'd')
      {
         Put_in_clipboard(slct);
         remove_slct(ed.file_name);
         ed.mode = 'n';
         return;      
      }  
      if(input == 'y')
      {
         ed.mode = 'n';
         Put_in_clipboard(slct);
         return;
      }
      navigation(input);   
      return;
   }

   if(ed.mode == 'n')
   {
      if(input == 'x')
      {
         ed.mode = 'x';
         return;
      }
      if(input == 'i')
      {
         ed.mode = 'i';
         return;
      }
      if(input == 'v')
      {
         ed.mode = 'v';
         st_selectx = ed.x;
         st_selecty = ed.y; 
         return;
      }
      if(input == ':' || input == '/')
      {
         ed.mode = 'm';
         memset(ed.Bar, '\0', sizeof ed.Bar);         
         ed.Bar[0] = input;
         return;
      }
      if(input == 'p')        // past
      {
         past_clipboard();
         return;
      }

      navigation(input);   
      return;
   }

   if(ed.mode == 'm')
   {
      if(input == 10)
      {
         ed.mode = 'n';
         process(ed.Bar);
         //memset(ed.Bar, '\0', sizeof ed.Bar); 
         return;
      }
     
      ed.Bar[strlen(ed.Bar)] = input;

      return;
   }

   // insert
   //print(input);
   if(input == 27)
   {
      ed.mode = 'n';
      return;
   }

   insert_input(input);
}

int main(int argc, char* argv[])
{

   if(argc > 1)
   {          
      init(argv[1]);

   }
   else
   {
      init(NULL);
   }
   //("%s\n", ed.Buffer);
   curses_init();                

   while(ed.mode != 'x')
   {
      printBuff();
      int input = getch();
      handel(input);
   }

   refresh();                      
   endwin();                      

   return 0;
}