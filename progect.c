
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include<unistd.h>



typedef long long int ll;
typedef double db;

#define f first
#define s second
#define all(v) (v).begin(),(v).end()
#define print(x) printf("%d \n", x);
#define printll(x) printf("%lld \n", x);
#define get(n) scanf("%d", &n);
#define N 36
#define mod 1000000007	// 1e9 + 7


char CWD[100];



void invalid_command()
{
    printf("invalid command\n");
}
bool exists(char *path)
{
    FILE *file;
    if ((file = fopen(path, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
bool valid(char * path)
{
    return(path != NULL && strlen(path) >= 5 && path[0] == 'r' && path[1] == 'o' && path[2] == 'o' && path[3] == 't' && path[4] == '/');
}
void fix(char* token)
{
    if(token[0] == '"')
    {
        //printf("fix\n");
        
        for(int i = 0; i < strlen(token)-1; i++)
            token[i] = token[i+1];
        token[strlen(token)-2] = '\0';
       
    }
}
void goto_root()
{
    getcwd(CWD, 100);
    int sz = strlen(CWD);
    while(CWD[sz-1] != 't' || CWD[sz-2] != 'o' || CWD[sz-3] != 'o' || CWD[sz-4] != 'r' || CWD[sz-5] != '/')
    {
        chdir("..");
        getcwd(CWD, 100);
        sz = strlen(CWD);
    }
    chdir("..");
}

void goto_path(char * path) // staet with : /root
{
    //printf("%s-\n", path);
    path = strtok(path, "/");
  //  path = strtok(NULL, "/");
    while(path != NULL) 
    {
        //char *st = token;
        //printf("%s-\n", path);
        mkdir(path, S_IRWXU);
        chdir(path);
        path = strtok(NULL, "/");
        
    }   
}
void get_all_char(char* string, FILE *fp)
{
   
    char c;
    while((c = getc(fp)) != EOF)
    {
        //printf("%c !!!! \n", c);
        string[strlen(string)] = c;
    }
}
void get_all_char_pos(char* string, FILE *fp, int x, int y)
{
    char c;
    for(int i = 1; i < x; i++)
    {
        while((c = getc(fp)) != '\n') 
        {
            string[strlen(string)] = c;
        }
        string[strlen(string)] = c;
    }
    for(int j = 0; j < y; j++)
    {
        c = getc(fp);
        string[strlen(string)] = c;
      //  printf("%c\n", c);
    }

}

char q[100];
char *str, *path, *type;
int dir, x, y, size_num, at;
bool C, L, count, byword, all;

void copy_file()
{
    FILE *fp, *fpcp;
    fp = fopen(path, "r");
    path[strlen(path)] = '*';
    fpcp = fopen(path, "w");
    char stt[10000] = {"\0"};
    get_all_char(stt, fp);
    //printf("%s\n", stt);
    fputs(stt, fpcp);
    fputs(stt, fp);
    path[strlen(path)-1] = '\0';
   
    fclose (fpcp);
    fclose(fp);
}


void Put(char* st, char* path)
{
   if(path[0] == '/')
     path++;
   FILE *fp;
   fp = fopen(path, "w");
   fputs(st, fp);
   fclose (fp);
   return;
}

void creatfile(char *path)
{
    //return;
    if(!valid(path) || exists(path))
    {
        invalid_command();
        return;
    }
    
    int stfile = 0;
    for(int i = strlen(path)-1; i >= 0; i--)
    {
        if(path[i] == '/')
        {
            stfile = i;
            break;
        }
    }
    
    printf("%d %s-\n", stfile, path);
    path[stfile] = '\0';
    print(-1);
    goto_path(path);
  

    path[stfile] = '/';
    path += stfile+1;
    FILE *fp;
    fp = fopen(path, "w+");
    fclose (fp);

    path[strlen(path)] = '*';
    fp = fopen(path, "w+");
    fclose (fp);

    goto_root();
   
}



void insert()
{

    if(!valid(path) || !exists(path) || x == -1 || y == -1 || str == NULL)
    {
        invalid_command();
        return;
    }
   
    copy_file(path);

    FILE *fp;
    fp = fopen(path, "r");

    char string[10000] = {"\0"};

    get_all_char_pos(string, fp, x, y);
    for(int i = 0; i < strlen(str); i++)
    {
        //if(str[i] != '\"')
        string[strlen(string)] = str[i];
    }

    get_all_char(string, fp);

    fclose (fp);
    fp = fopen(path, "w");
    fputs(string, fp);
    fclose (fp);
}

void cat()
{
   
    //printf("%s\n", path);
    if(!valid(path) || !exists(path))
    {
        invalid_command();
        return;
    }

    FILE *fp;
    fp = fopen(path, "r");
    char string[10000] = {"\0"};
    get_all_char(string, fp);
    printf("%s", string);
    fclose (fp);
    
}

void removestr()
{
  
    //printf("%s-\n", path);
    if(!valid(path) || !exists(path) || x == -1 || y == -1 || size_num == -1 || dir == -1)
    {
        invalid_command();
        return;
    }
    FILE *fp;
    fp = fopen(path, "r");

    char string[10000] = {"\0"};
    get_all_char_pos(string, fp, x, y);
    if(dir)
    {
        if(strlen(string) < size_num)
        {
            invalid_command();
            return;
        }
        for(int i = 0; i < size_num; i++)
          string[strlen(string)-1] = '\0';
        
    }
    else
    {
        char c;
        for(int i = 0; i < size_num; i++)
        {
            c = getc(fp);
            if(c == EOF)
            {
                invalid_command();
                return;
            }
        }

    }

    get_all_char(string, fp);
    fclose (fp);

    copy_file();

    fp = fopen(path, "w");
    //printf("%s", string);
    fputs(string, fp);
    fclose (fp);


}

void Put_in_clipboard(char* st)
{
    FILE *fp;
    fp = fopen("clipboard.txt", "w");
    fputs(st, fp);
    fclose (fp);
}

void copy()
{
   // printf("%s %d %d %d %d \n", path, x, y, size_num, dir);
    if(!valid(path) || !exists(path) || x == -1 || y == -1 || size_num == -1 || dir == -1)
    {
        invalid_command();
        return;
    }

    FILE *fp;
    fp = fopen(path, "r");

    char tmp[10000] = {"\0"}, string[10000] = {"\0"};
    get_all_char_pos(tmp, fp, x, y);
    

    if(dir)
    {
        if(strlen(tmp) < size_num)
        {
            invalid_command();
            return;
        }
        int k = size_num;
        for(int i = 0; i < size_num; i++)
        {
          string[i] = tmp[strlen(tmp)-k];
          k--;
        }
    }
    else
    {
       
        for(int i = 0; i < size_num; i++)
        {
            string[i] = getc(fp);
            if(string[i] == EOF)
            {
                invalid_command();
                return;
            }
        }

    }
    //printf("%s", string);
    fclose (fp);
    Put_in_clipboard(string);

}


void cut()
{
    copy();
    removestr();
}

void paste()
{
    if(!valid(path) || !exists(path))
    {
        invalid_command();
        return;
    }
    FILE *fp;
    fp = fopen("clipboard.txt", "r");
    char string[10000] = {"\0"};
    //printf("%s\n", str);
    get_all_char(string, fp);
    str = string;
    fclose (fp);
    insert();

}

bool match(char* st1, char* st2)
{
    if(strlen(st2) > strlen(st1))
        return 0;
    for(int i = 0; i < strlen(st2); i++)
    {
        if(st1[i] != st2[i])
            return 0;
    }
    return 1;
}

bool match2(char* a, char* b, int len)
{
    if(strlen(b) > len)
        return 0;

    if(b[0] == '*')
    {
        b++;
        int j = 0;
        for(int i = len-strlen(b); i < len; i++)
        {
            if(b[j++] != a[i])
                return 0;

        }
        return 1;
    }
    if(b[strlen(b)-1] == '*' && b[strlen(b)-2] != '\\')
    {
        for(int i = 0; i < strlen(b)-1; i++)
        {
            if(a[i] != b[i])
                return 0;
        }
        return 1;
    }
    if(strlen(b) != len)
        return 0;
    
    //for(int i = 0; i < len; i++)
      //  printf("%c", a[i]);
    //printf("\n");
    
    for(int i = 0; i < len; i++)
    {
        if(b[i] != a[i])
            return 0;
    }    
    
    return 1;

}

void find()     // count (at n) byword all              //find -str hiva -file /root/a.txt -all -byword
{
    if(!valid(path) || !exists(path) || str == NULL)
    {
        invalid_command();
        return;
    }

    FILE *fp;
    fp = fopen(path, "r");
    char string[10000] = {"\0"};
    get_all_char(string, fp);
    fclose (fp);

    printf("%s\n\n\n", string);

    if(count + (at != 0) + all > 1)
    {
        invalid_command();
        return;
    }
    if(!count && !all)
        at = 1;

    int cnt = 0, cnt_vaj = 0, ind_vaj = 0;
    
    for(int i = 0; i < strlen(string); i++)
    {
        while(string[i] == ' ' || string[i] == '\n')
            continue;
        //printf("start of word : %d\n", i);

        int j = i; 
        bool fl = 0;   
        ind_vaj++;    
        while(string[j] != '\n' && string[j] != ' ' && j < strlen(string))
        {
            //*
            for(int k = i; k <= j; k++)
            {
                if(match2(string+k, str, j-k+1))
                {
                    fl = 1;
                    cnt++;
                    if(!byword && cnt == at)
                    {
                        print(k);
                        return;
                    }

                    if(!byword && all)
                    {
                        //printf("%d %d\n", k, j);
                        if(cnt > 1)
                            printf(", ");
                        printf("%d", k);
                    }
                }

            }
            //*/
            //printf("%d %d %c\n", (int)(strlen(string)), j, string[j]);
            j++;
        }
        cnt_vaj += fl;
        if(byword && cnt_vaj == at)
        {
            print(ind_vaj);
            return;
        }
        if(byword && all && fl)
        {
            if(cnt_vaj > 1)
                printf(", ");
            printf("%d", ind_vaj);
        }

        i = j;
    }

    if(byword)
    {
        if(count)
            print(cnt_vaj);
        if(at != 0 && cnt_vaj < at)
            print(-1);
    }
    else
    {
        if(count)
            print(cnt);
        if(at != 0 && cnt < at)
            print(-1);
    }

 
    if(all)
        printf("\n");

    return;
}

void grep()
{
    if(str == NULL)
    {
        invalid_command();
        return;
    }
    
    char copy_path[100] = {"\0"};
    for(int i = 0; i < strlen(path); i++)
        copy_path[i] = path[i];

    char *token = strtok(path, " ");
    while(token != NULL)
    {
        token++;
        //printf("%s-\n", token);
        if(!valid(token) || !exists(token))
        {
            invalid_command();
            return;
        }
        token = strtok(NULL, " ");
    }

    
    path = copy_path;
    //printf("%s-\n", path);
    int cnt = 0;
    char* token2 = strtok(path, " ");
    while(token2 != NULL)
    {
        token2++;

        //printf("%s--\n", token2);
        
        FILE *fp;
        fp = fopen(token2, "r");
        char string[10000] = {"\0"};
        get_all_char(string, fp);
        fclose (fp);
       // printf("%s--\n", string);

        int st = 0;
        for(int i = 0; i < strlen(string); i++)
        {
            if(string[i] == '\n')
            {
                st = i+1;
                continue;
            }
            if(match(string+i, str))
            {
                cnt++;
                if(!C)
                    printf("%s", token2);
                if(!C && !L)
                    printf(": ");        
                while(string[st] != '\n')
                {
                    if(!C && !L)
                        printf("%c", string[st]);
                    st++;
                }
                if(!C)
                    printf("\n");
                i = st;
            }
        }

        token2 = strtok(NULL, " ");
    }
    if(C)
        print(cnt);

    /*

    */



}


void undo()
{
    if(!valid(path) || !exists(path))
    {
        invalid_command();
        return;
    }

    FILE *fp, *fpcp;
    
    path[strlen(path)] = '*';
    fpcp = fopen(path, "r");
    char stt[10000] = {"\0"};
    get_all_char(stt, fpcp);
    path[strlen(path)-1] = '\0';

    fclose (fpcp);

    copy_file();
   
    fp = fopen(path, "w");
    fputs(stt, fp);
    fclose (fp);



}

int get_cnt_line(char *st)
{
    int ans = 0;
    for(int i = 0; i < strlen(st); i++)
        ans += (st[i] == '\n');
    return ans;    
}


void compare()
{
    char p1[10000] = {"\0"}, p2[10000] = {"\0"};
    int ii = 0;
    while(path[ii] != ' ')
    {
        p1[strlen(p1)] = path[ii];
        //printf("%c\n", path[ii]);
        ii++;
    }
    path += 2;
    while(ii < strlen(path))
    {
        p2[strlen(p2)] = path[ii];
        ii++;
    }

    if(!valid(p1) || !exists(p1) || !valid(p2) || !exists(p2))
    {
        invalid_command();
        return;
    }

    char st1[10000] = {"\0"}, st2[10000] = {"\0"};
    FILE *fp;
    fp = fopen(p1, "r");
    get_all_char(st1, fp);
    fclose (fp);

    fp = fopen(p2, "r");
    get_all_char(st2, fp);
    fclose (fp);
    printf("%s-\n%s-\n", st1, st2);
    //return; 
    int i = 0, j = 0, l = 1; 
    while(l < strlen(st1) && i < strlen(st1) && j < strlen(st2))
    {
        bool fl = true;
        int t_i = i, t_j = j;
        while(fl && st1[t_i] != '\n' && st2[t_j] != '\n')
        {
            if(st1[t_i] != st2[t_j])
                fl = false;     

            t_i++;
            t_j++;
        }    

        if(!fl)
            printf("============ #%d ===========\n", l);

        while(i < strlen(st1) && st1[i] != '\n')
        {
            if(!fl)
                printf("%c", st1[i]);
            i++;
        }
        i++;
        if(!fl)
            printf("\n");

        while(j < strlen(st2) && st2[j] != '\n')
        {
            if(!fl)
                printf("%c", st2[j]);
            j++;
        }
        j++;
        if(!fl)
            printf("\n");
        l++;
       
    }
    //printf("%d %d\n", i, j);
    if(i != strlen(st1))
    {
        printf("<<<<<<<<<<< #%d - #%d <<<<<<<<<<<\n", l, get_cnt_line(st1));
        while(i < strlen(st1))
        {
            while(i < strlen(st1) && st1[i] != '\n')
                printf("%c", st1[i++]);
            l++;
            i++;
        }
        printf("\n");

    }


    if(j != strlen(st2))
    {
        printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>n", l, get_cnt_line(st2));
        while(j < strlen(st2))
        {
            printf("============ #%d ===========\n", l);
            while(j < strlen(st2) && st2[j] != '\n')
                printf("%c", st2[j++]);
            l++;
            j++;
        }
        printf("\n");
    }
}

void closing_pairs()
{
    if(!valid(path) || !exists(path))
    {
        invalid_command();
        return;
    }
    char st2[10000] = {"\0"}, st1[1000] = {"\0"};

    FILE *fp;
    fp = fopen(path, "r");
    get_all_char(st1, fp);
    fclose (fp);

    int i = 0, cnt_tab = 0;
    while(i < strlen(st1))
    {
        //printf("%d\n%s", i, st2);
        bool fl_char = 0;
        while(st1[i] == ' ' || st1[i] == '\t')  
            i++;
        if(st1[i] == '\n')
            goto hell;
        // insert tabs
        for(int t = 0; t < cnt_tab; t++)
            st2[strlen(st2)] = '\t';

        while(i < strlen(st1) && st1[i] != '\n')
        {
            if(st1[i] == '{')
            {
                while(fl_char && st2[strlen(st2)] == ' ' || st2[strlen(st2)] == '\t')
                    st2[strlen(st2)] = '\0';
                if(fl_char)        
                    st2[strlen(st2)] = ' ';   
                st2[strlen(st2)] = '{';
                st2[strlen(st2)] = '\n';   
                cnt_tab++;    
                goto hell;
            }    
            else if(st1[i] == '}')
            {
                st2[strlen(st2)] = '\n';
                cnt_tab--;
                // insert tabs
                for(int t = 0; t < cnt_tab; t++)
                    st2[strlen(st2)] = '\t';

                st2[strlen(st2)] = '}';
                st2[strlen(st2)] = '\n';
                goto hell;   
            }
            else if(st1[i] != ' ' && st1[i] != '\t')
                fl_char = 1;

            st2[strlen(st2)] = st1[i];
            i++;
        }
        hell:
        i++;
    }
    //printf("%s", st2);

    copy_file();
    fp = fopen(path, "w");
    //printf("%s", string);
    fputs(st2, fp);
    fclose (fp);
}

int _file_(char** token)
{
    if((*token)[0] != 'f' || (*token)[1] != 'i' || (*token)[2] != 'l' || (*token)[3] != 'e')
        return 0;
    (*token) += 5;
    //printf("path : %s- \n", (*token));
    if((*token)[strlen((*token))-1] == ' ')
        (*token)[strlen((*token))-1] = '\0';
    fix((*token));
    (*token)++;
    path = (*token);
    return 1;
}

int _pos_(char** token)
{
    //printf("%s\n", (*token));
    if((*token)[0] != 'p' || (*token)[1] != 'o' || (*token)[2] != 's')
        return 0;
    (*token) += 4;
    x = (int)((*token)[0] - '0');
    y = (int)((*token)[2] - '0');
    return 1;

}

int _size_(char** token)
{
    if((*token)[0] != 's' || (*token)[1] != 'i' || (*token)[2] != 'z' || (*token)[3] != 'e')
        return 0;
    (*token) += 5;
    size_num = (int)((*token)[0] - '0');
    return 1;

}

int _borf_(char** token)
{
    if((*token)[0] == 'b')
        dir = 1;
    else if((*token)[0] == 'f')
        dir = 0;
    else
        return 0;
    return 1;    
}

int _str_(char** token)
{
    if((*token)[0] != 's' || (*token)[1] != 't' || (*token)[2] != 'r')
        return 0;
    (*token)[strlen((*token))-1] = '\0';
    (*token) += 4;
    fix((*token));    
    str = (*token);
    return 1;
}

int _C_(char** token)
{
    if((*token)[0] != 'C')
        return 0;
    return C = 1;
}

int _L_(char** token)
{
    if((*token)[0] != 'l')
        return 0;
    return L = 1;
}

int _find_(char** token)
{
    if((*token)[0] == 'c' && (*token)[1] == 'o' && (*token)[2] == 'u' && (*token)[3] == 'n' &&  (*token)[4] == 't')
    {
        count = 1;
        return 1;
    }

    if((*token)[0] == 'b' && (*token)[1] == 'y' && (*token)[2] == 'w' && (*token)[3] == 'o' && (*token)[4] == 'r' && (*token)[5] == 'd')
    {
        byword = 1;
        return 1;
    }
    if((*token)[0] == 'a' && (*token)[1] == 'l' && (*token)[2] == 'l')
    {
        all = 1;
        return 1;
    }
    if((*token)[0] != 'a' || (*token)[1] != 't' || (*token)[2] != ' ')
    {
        return 0;
    }
    //printf("fuck %s\n", (*token));
    //printf("%c\n", (*token)[0]);
    (*token) += 3;
    at = (int)((*token)[0] - '0');

    return 0;
}


void query()
{
    str = path = NULL;
    dir = x = y = size_num = -1;
    count = at = all = byword = C = L = 0;
    char* token = strtok(q, " ");
    type = token;
    token++;
    token = strtok(NULL, "-");
 
    while(token != NULL)
    {
        //printf("tolen : %s-\n", token);
        if(_file_(&token))
            goto hell;
        else if(_find_(&token))
            goto hell;            
        else if(_str_(&token))
            goto hell;
        else if(_size_(&token))
            goto hell;
        else if(_pos_(&token))
            goto hell;
        else if(_borf_(&token))
            goto hell;  
        else if(_C_(&token))
            goto hell;
        else if(_L_(&token))
            goto hell;

        hell:
        token = strtok(NULL, "-");
    }

    printf("/////////////////// \ntype : %s\nstr : %s-\npath : %s-\nx : %d\ny : %d\ndir : %d\nsizre : %d\nC : %d\nL : %d\n",type, str, path, x, y, dir, size_num, C, L);
    printf("conut : %d\nat : %d\nbyword : %d\nall : %d\n////////////////////////////////\n", count, at, byword, all);

    if(strcmp(type, "createfile") == 0)             // createfile -file /root/a.txt
    {
        creatfile(path);
    }
    else if(strcmp(type, "insertstr") == 0)         // insertstr -file /root/a.txt -str "hhhhhhhhhhhhhhhhhhhh" -pos 1:0
    {
        insert(path, str);
    }
    else if(strcmp(type, "cat") == 0)               // cat -file /root/dir1/dir2/file.txt
    {
        cat();
    }   
    else if(strcmp(type, "removetstr") == 0)        // removetstr -file /root/dir1/dir2/file.txt -pos 2:5 -b -size 3
    {
        removestr(); 
    }
    else if(strcmp(type, "copy") == 0)              // copy -file /root/dir1/dir2/file.txt -pos 2:5 -b -size 3
    {
        copy();
    }
    else if(strcmp(type, "cut") == 0)               // find -str hiva -file /root/a.txt -all
    {
        cut();
    }
    else if(strcmp(type, "pastestr") == 0)          // pastestr -file /root/dir1/dir2/a.txt -pos 1:0
    {
        paste();
    }
    else if(strcmp(type, "find") == 0)              // find -str hiva -file /root/a.txt -all
    {
        find();
    }
    else if(strcmp(type, "grep") == 0)              //  grep -str "hiva" -files /root/b.txt /root/a.txt /root/c.txt
    {
        grep();
    }
    else if(strcmp(type, "undo") == 0) 
    {
        undo();
    }  
    else if(strcmp(type, "compare") == 0)           // compare -file /root/a.txt /root/b.txt
    {
        compare();
    }
    else if(strcmp(type, "auto-indent") == 0)       // auto-indent -file /root/a.txt
    {
        closing_pairs();
    }
}



int main()
{
    //creatfile("root/a.txtaaaalfk");
    //return 0;
    //printf("%s\n", getcwd(CWD, 100));
    scanf("%[^\n]", q);


    while(strcmp(q, "exit"))   
    {
       
        query();
    
        char temp;
        scanf("%c",&temp);
        scanf("%[^\n]", q);
    }

    return 0;
}
