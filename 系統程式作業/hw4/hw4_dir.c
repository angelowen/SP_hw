/*
   listDir.c
   usage listDir .
 */

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <linux/limits.h>
#define len 1024
int level = 0;
void swap(int a,int b,char v[][1024]){
	char tmp[1024];
	strcpy(tmp,v[a]);
	strcpy(v[a],v[b]);
	strcpy(v[b],tmp);
}

void printName(char* type, char* name) {
	printf("%s", type);
	for (int i=0; i < level; i++)
		printf("  ");
	if (strcmp("d", type)==0)
		printf("+");
	else
		printf("|");
	printf("%s\n", name);
}

void listDir(char* pathName)
{
	level++;
	DIR* curDir = opendir(pathName);
	assert(curDir!=NULL);
	char* newPathName = (char*)malloc(PATH_MAX);
	struct dirent entry;
	struct dirent* result;
	int ret,cnt_file=0,cnt_folder=0,i,j;
	char file[len][len],folder[len][len];

	ret = readdir_r(curDir, &entry, &result);
	while(result != NULL) {
		if (strcmp(entry.d_name, ".") == 0 || strcmp(entry.d_name, "..") == 0) {
			ret = readdir_r(curDir, &entry, &result);
			assert(ret == 0);
			continue;
		}
		assert(ret == 0);

		if (entry.d_type == DT_REG){
					strcpy(file[cnt_file],entry.d_name);
			cnt_file++;

		}
		if (entry.d_type == DT_DIR) {
					strcpy(folder[cnt_folder],entry.d_name);
			cnt_folder++;
		}
		ret = readdir_r(curDir, &entry, &result);
		assert(ret == 0);        
	}
	for(i=0;i<cnt_file;i++){
		for(j=i+1;j<cnt_file;j++){
			if(strcmp(file[i],file[j])>0){
				swap(i,j,file);
			}

		}		
	}
	for(i=0;i<cnt_file;i++){
		printName("f", file[i]);
	}
	for(i=0;i<cnt_folder;i++){
		for(j=i+1;j<cnt_folder;j++){
			if(strcmp(folder[i],folder[j])>0){
				swap(i,j,folder);
			}

		}		
	}
	for(i=0;i<cnt_folder;i++){
		printName("d", folder[i]);
		sprintf(newPathName, "%s/%s", pathName, folder[i]);
		printf("%s\n", newPathName);
		listDir(newPathName);

	}

	closedir(curDir);
	level--;
}

int main(int argc, char** argv) {
	listDir(argv[1]);
}
