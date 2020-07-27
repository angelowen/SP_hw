/*執行方法：
  myShell
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
///color///
#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"
#define RED_BOLD "\x1b[;31;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

/*
   全域變數，放解析過後的使用者指令（字串陣列）
 */
char* argVect[256];
/*
   parseString：將使用者傳進的命令轉換成字串陣列
   str：使用者傳進的命令
   cmd：回傳執行檔
 */

int idx;
void parseString(char* str, char** cmd) {

	char* retPtr;
	idx = 0;
	//printf("%s\n", str);
	retPtr=strtok(str, " \n");
	while(retPtr != NULL) {
		//printf("token =%s\n", retPtr);
		//if(strlen(retPtr)==0) continue;
		argVect[idx++] = retPtr;

		if (idx==1)
			*cmd = retPtr;
		retPtr=strtok(NULL, " \n");
	}
	argVect[idx]=NULL;
}

int main (int argc, char** argv , char *env[]) {
	char cmdLine[4096];
	char hostName[256];
	char cwd[256];
	char* exeName;
	int pid, pos, wstatus;
	char *const envp[] = { NULL };
	const char path[10][50] = {"/usr/local/sbin/" , "/usr/local/bin/" , "/usr/sbin/" , "/usr/bin/" , "/sbin/" , "/bin/" , "/usr/games/" , "/usr/local/games/" , "/snap/bin/"};
	char tmp[100];
	int flag,x;
	char *ptr,*qtr;
	char arg[1024][1024];
	while(1) {
		char* showPath;
		char* loginName = getlogin();
		int homeLen = 0;
		gethostname(hostName, 256);
		/*
		   底下程式碼製造要顯示的路徑字串，
		   會參考"home"將"home"路徑取代為~
		 */
		getcwd(cwd, 256);
		pos=strspn(getenv("HOME"), cwd);
		homeLen = strlen(getenv("HOME"));
		//printf("cwd=%s, home=%s, pos=%d, prompt=%s", cwd, getenv("HOME"), pos, &cwd[pos]);
		if(pos>=homeLen) {
			cwd[pos-1]='~';
			showPath=&cwd[pos-1];
		}
		else
			showPath=cwd;
		/*
		   底下程式碼負責印出提示符號
		 */
		printf(LIGHT_GREEN"%s@%s:", loginName, hostName);
		printf(BLU_BOLD"%s>> ", showPath);
		printf(NONE);
		/*
		   接收使用者命令，除了cd, exit以外，其他指令呼叫對應的執行檔
		 */
		fgets(cmdLine, 4096, stdin);
		if (strlen(cmdLine)>1)
		{
			parseString(cmdLine, &exeName);
			//printf("%s\n",exeName);
			//for(int i=0;argVect[i]!=NULL;i++)
			//printf("%s\n",argVect[i]);
			//printf("%s\n",longcmd);
		}
		else
			continue;
		if (strcmp(exeName, "exit") == 0)
			break;
		if (strcmp(exeName, "cd") == 0) {
			if (strcmp(argVect[1], "~")==0)
				chdir(getenv("HOME"));
			else
				chdir(argVect[1]);
			continue;
		}
		if( strcmp(exeName,"echo") == 0)
		{
			int len = 0 , arglen = 0 , flag = 0;
			char tmp[50],var[50];
			for(int k=1; argVect[k]!=NULL; k++)
			{	
				arglen = strlen(argVect[k]);
				for(int i=1; i<arglen; i++)
					tmp[i-1] = argVect[k][i];
				for(int i=0; env[i]!=NULL; i++)
				{
					qtr=env[i];
					ptr=strstr(env[i],"=");
					for(x=0;x<ptr-qtr;x++){
						var[x]=env[i][x];
					}
					var[x]='\0';
					if( strcmp(var, tmp )==0 )
					{
						ptr++;
						printf("%s\n",ptr);						
						flag = 1;
						break;
					}
				}
			}
			if(flag)
				continue;
		}
		pid = fork();
		if (pid == 0) {
			/*
			   產生一個child執行使用者的指令
			 */
			for(int i=0;i<9;i++)
			{
				flag = 0;
				for(int j=0;j<50;j++) tmp[j] = '\0';
				strcpy(tmp , path[i]);
				strcat(tmp , exeName);
				tmp[strlen(tmp)] = '\0';


			/*	char e[1024]="execle(tmp,argVect[";
				e[strlen(e)]='0'+idx;
				e[strlen(e)]=']';
				char e1[1024]=",NULL,NULL)";
				for(k=1;k<idx;k++){
					
					strcat(e,"");
				}
			//	if(execle(tmp,argVect[0],NULL,NULL)==-1)
			//		continue;
			//	else flag=1;*/
				if(idx==1)
					if( execle(tmp ,argVect[0] , NULL , NULL) == -1 )
						continue;
					else
						flag = 1;
				else if(idx==2)
					if( execle(tmp , argVect[0] , argVect[1] , NULL , NULL) == -1 )
						continue;
					else
						flag = 1;
				else if(idx==3)
					if( execle(tmp , argVect[0] , argVect[1] , argVect[2], NULL , NULL) == -1 )
						continue;
					else
						flag = 1;
				else if(idx==4)
					if( execle(tmp , argVect[0] , argVect[1] , argVect[2] , argVect[3] , NULL , NULL) == -1)
						continue;
					else
						flag = 1;

				if(flag) break;
			}
			if(flag==0)
				printf("error\n");
		} else {
			/*
			   parent(myShell)等待child完成命令
			   完成命令後，parent將child的結束回傳值印出來
			 */
			wait(&wstatus);
			printf(RED "return value of " YELLOW "%s" RED " is " YELLOW "%d\n", 
					exeName, WEXITSTATUS(wstatus));
			printf(NONE);
		}
	}
}
