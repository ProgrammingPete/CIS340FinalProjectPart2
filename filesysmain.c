/*
 * filesysmain.c
 *
 *  Created on: Apr 23, 2018
 *      Author: peter
 */


#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include	<string.h>

void do_ls(char[]);
void dostat(char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int , char [] );

int main(int argc, char *argv[]){
	//check arg length using passed variable named argc. The tokenization
	// of these arguments should be stored in the 2-d array
	// checking changes
	int opt, index;
	int longver = 0;
	bool sortbytimemod = false;
	bool sortBySize = false;
	
	while((opt = getopt(argc,argv,"lts")) != -1){
		switch(opt){
		case 'l': 
		longver = 1;
		break;
		case 't': sortbytimemod = true;
		break;
		case 's': sortBySize = true;
		break;
		default:
			printf("Usage %d enter cp or ls first \n", argv[0]);
			return 1;
		}

	}
	
	for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
	
	//int opt1 = optind;
	//printf("%d", opt1);
	
	int i =0;
	char str[100];
	
	if((strcmp(argv[1], "ls")) == 0){	
			do_ls(".");
		}
	else if((strcmp(argv[1], "cp")) == 0){
		//printf("hello");
	}
}


void do_ls( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls2: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
			if (strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0)
				continue;
			dostat( direntp->d_name );
		}
		closedir(dir_ptr);
	}
}


void dostat( char *filename )
{
	struct stat info;

	if ( stat(filename, &info) == -1 )		/* cannot stat	 */
		perror( filename );			/* say why	 */
	else					/* else show info	 */
		show_file_info( filename, &info );
}


void show_file_info( char *filename, struct stat *info_p )
/*
 * display the info about 'filename'.  The info is stored in struct at *info_p
 */
{
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void	mode_to_letters();
        char    modestr[11];

 mode_to_letters( info_p->st_mode, modestr );
	//these correspond to colums
	printf( "%s"    , modestr );		
	printf( "%4d "  , (int) info_p->st_nlink);
//	printf( "%-8s " , uid_to_name(info_p->st_uid) );
//	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );

}

void mode_to_letters( int mode, char str[] )
{
    strcpy( str, "----------" );           /* default=no perms */

    if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
    if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
    if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */

    if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

