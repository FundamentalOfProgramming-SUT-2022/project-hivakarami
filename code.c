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
int dir, x, y, size_num;


void creatfile()
{
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
    
    path[stfile] = '\0';
    //printf("fuck%s-\n", path);    
    goto_path(path);
  

    path[stfile] = '/';
    path += stfile+1;
    FILE *fp;
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

    FILE *fp;
    fp = fopen(path, "r");
    
    char string[10000] = {"\0"};

    get_all_char_pos(string, fp, x, y);

    for(int i = 0; i < strlen(str) - 1; i++)
    {
        if(str[i] != '\"')
            string[strlen(string)] = str[i];
    }

    get_all_char(string, fp);

    fclose (fp);
    fp = fopen(path, "w");
    //printf("%s", string);
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

    fp = fopen(path, "w");
    //printf("%s", string);
    fputs(string, fp);
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
    fp = fopen("clipboard.txt", "w");
    fputs(string, fp);
    fclose (fp);

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
    insert();
   
    fclose (fp);

}



/*
void find(char * token)
{
    return;
}

*/




int _file_(char** token)
{
    if((*token)[0] != 'f' || (*token)[1] != 'i' || (*token)[2] != 'l' || (*token)[3] != 'e')
        return 0;
    (*token) += 5;
    if((*token)[strlen((*token))-1] == ' ')
        (*token)[strlen((*token))-1] = '\0';
    fix((*token));
    (*token)++;
    path = (*token);
    //printf("path : %s- \n", path);
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


void query()
{
    str = path = NULL;
    dir = x = y = size_num = -1;
    char* token = strtok(q, "-");
    type = token;
    type[strlen(type)-1] = '\0';
    token = strtok(NULL, "-");
 
    while(token != NULL)
    {
        //printf("%s-\n", token);
        if(_file_(&token))
            goto hell;
        else if(_str_(&token))
            goto hell;
        else if(_borf_(&token))
            goto hell;
        else if(_pos_(&token))
            goto hell;
        else if(_size_(&token))
            goto hell;  

        hell:
        token = strtok(NULL, "-");
    }

    //printf("/////////////////// \ntype : %s\nstr : %s\npath : %s-\nx : %d\ny : %d\ndir : %d\nsizre : %d\n/////////////////// \n",type, str, path, x, y, dir, size_num);


}



int main()
{
    printf("%s\n", getcwd(CWD, 100));
    scanf("%[^\n]", q);


    while(strcmp(q, "exit"))   
    {
       
        query();
    
        
        if(strcmp(type, "createfile") == 0)             // createfile -file /root/dir1/dir2/file3.txt
        {
            creatfile();
        }
        else if(strcmp(type, "insertstr") == 0)         // insertstr -file /root/dir1/dir2/file.txt -str "xxxxxxx" -pos 2:5
        {
            insert();
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
        else if(strcmp(type, "cut") == 0)
        {
            cut();
        }
        else if(strcmp(type, "pastestr") == 0)          // pastestr -file /root/dir1/dir2/a.txt -pos 1:0
        {
            paste();
        }
        else if(strcmp(type, "find") == 0)
        {
           // find();
        }
          
        char temp;
        scanf("%c",&temp);
        scanf("%[^\n]", q);
    }

    return 0;
}

