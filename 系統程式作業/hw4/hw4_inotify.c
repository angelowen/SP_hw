#include <sys/inotify.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define Len 1024
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void printInotifyEvent(struct inotify_event* event);

char wd[1000][10000],path[Len][Len];
int cnt_p=0,level=0;

void listDir(char* pathName)
{
	level++;
	DIR* curDir = opendir(pathName);
	assert(curDir!=NULL);
	char* newPathName = (char*)malloc(PATH_MAX);
	struct dirent entry;
	struct dirent* result;
	int ret,cnt_file=0,cnt_folder=0,i,j;
	char file[Len][Len],folder[Len][Len];

	ret = readdir_r(curDir, &entry, &result);
	while(result != NULL) {
		if (strcmp(entry.d_name, ".") == 0 || strcmp(entry.d_name, "..") == 0) {
			ret = readdir_r(curDir, &entry, &result);
			assert(ret == 0);
			continue;
		}
		assert(ret == 0);
		if (entry.d_type == DT_DIR) {
			//printName("d", entry.d_name);
			sprintf(newPathName, "%s/%s", pathName, entry.d_name);
		//	printf("%s\n", newPathName);
			strcpy(path[cnt_p],newPathName);
			cnt_p++;

			listDir(newPathName);
		}
		ret = readdir_r(curDir, &entry, &result);
		assert(ret == 0);        
	}
/*	for(i=0;i<cnt_p;i++){
		printf("%s\n", path[i]);
	
	}*/

	closedir(curDir);
	level--;
}

int main(int argc, char **argv) {
	int fd, num, ret, i;
	char* p;
	char inotify_entity[BUF_LEN];

	listDir("./");
	
	fd = inotify_init();
	ret=inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
	strcpy(wd[ret], argv[1]);

	for (i=0; i< cnt_p; i++) {
		ret=inotify_add_watch(fd, path[i], IN_ALL_EVENTS);
		strcpy(wd[ret], path[i]);
	
	}
	while(1) {
		num = read(fd, inotify_entity, BUF_LEN);
		for (p = inotify_entity; p < inotify_entity + num; ) {
			printInotifyEvent((struct inotify_event *) p);
			p+=sizeof(struct inotify_event) + ((struct inotify_event *)p)->len;
		}
	}
}

void printInotifyEvent(struct inotify_event* event) {
	char buf[4096]="";
	sprintf(buf, "[%s] ", wd[event->wd]);
	strncat(buf, "{", 4096);
	if (event->mask & IN_ACCESS) 		strncat(buf, "ACCESS, ", 4096);
	if (event->mask & IN_ATTRIB) 		strncat(buf,"ATTRIB, ", 4096);
	if (event->mask & IN_CLOSE_WRITE) 	strncat(buf,"CLOSE_WRITE, ", 4096);
	if (event->mask & IN_CLOSE_NOWRITE) 	strncat(buf,"CLOSE_NOWRITE, ", 4096);
	if (event->mask & IN_CREATE) 		strncat(buf,"CREATE, ", 4096);
	if (event->mask & IN_DELETE) 		strncat(buf,"DELETE, ", 4096);
	if (event->mask & IN_DELETE_SELF) 	strncat(buf,"DELETE_SELF, ", 4096);
	if (event->mask & IN_MODIFY) 		strncat(buf,"MODIFY, ", 4096);
	if (event->mask & IN_MOVE_SELF)		strncat(buf,"MOVE_SELF, ", 4096);
	if (event->mask & IN_MOVED_FROM)	strncat(buf,"MOVED_FROM, ", 4096);
	if (event->mask & IN_MOVED_TO)		strncat(buf,"MOVED_TO, ", 4096);
	if (event->mask & IN_OPEN)			strncat(buf,"OPEN, ", 4096);
	if (event->mask & IN_IGNORED)		strncat(buf,"IGNORED, ", 4096);
	if (event->mask & IN_ISDIR)			strncat(buf,"ISDIR, ", 4096);
	if (event->mask & IN_Q_OVERFLOW)	strncat(buf,"Q_OVERFLOW, ", 4096);
	buf[strlen(buf)-2]='\0';
	strncat(buf, "}", 4096);
	sprintf(buf, "%s cookie=%d", buf, event->cookie);
	if (event->len>0)
    	sprintf(buf, "%s name = %s\n", buf, event->name);
	else
		sprintf(buf, "%s name = null\n", buf);
	printf("%s", buf);
}
