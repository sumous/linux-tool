# include <stdio.h>
# include <unistd.h>
# include <dirent.h>
# include <string.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdlib.h>


# define boolean char
# define TRUE 1
# define FALSE 0

# define is_num(a) ((a>='0' && a<='9') ? TRUE : FALSE )

/* judge if d_name is process name */
boolean is_process_name(char * d_name)
{
	while(*d_name != '\0') {
		if (! is_num(*d_name)) return FALSE;
		d_name++;
	}
     
	return TRUE;
}

/* search the pid that is r/w file */
void pids_rw_file(char *file_name_s, char pids[][10], int * pid_l)
{
	char  file_name[PATH_MAX];
	realpath(file_name_s, file_name);
   
	char file_name_buf[PATH_MAX];
	char fd_path_buf[PATH_MAX];
	char file_path_buf[PATH_MAX];

	DIR * proc = opendir("/proc");
	/* change the directory to /proc */
	chdir("/proc");

	struct dirent * ptr;
	while( (ptr = readdir(proc)) && (ptr != NULL)) {
		
		if (! is_process_name(ptr->d_name)) continue;
		
		char * fd_path = fd_path_buf;
		strcpy(fd_path, ptr->d_name);
		strcat(fd_path, "/fd");
	   
		/* no excutable */
		if (access(fd_path, X_OK) != 0) {
			printf("cannot open directory /proc/%s: Permission denied\n", fd_path);
			continue;
		}
		
		/* traversal the process fd path */
		DIR * process = opendir(fd_path);
		struct dirent * p;
		while ( (p = readdir(process)) && (p != NULL) ) {
			char * file_path = file_path_buf;
			strcpy(file_path, fd_path);
			strcat(file_path, "/");
			strcat(file_path, p->d_name);

			char * linkname = file_name_buf;
			
		        ssize_t r = readlink(file_path, linkname, PATH_MAX);
			linkname[r] = '\0';
		        if (strcmp(linkname, file_name) == 0) {
				strcpy((char *)pids, ptr->d_name);
				pids += 1;
				(*pid_l) ++;
				break;
			}
		}

		
	}
}

int main(int argc, char * argv[])
{	
	if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
	}

	char pids[100][10];
	int pid_l = 0;
	pids_rw_file(argv[1], pids, & pid_l);
	int i;
	printf("%d process are r/w %s\n", pid_l, argv[1]);
	for (i = 0; i < pid_l; i++)
		printf("%s\n", pids[i]);
	return 0;
}
