/*
 * filesysmain.c
 *
 *  Created on: Apr 23, 2018
 *      Author: peter
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

void do_ls(char[]);
void dostat(char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int , char [] );
void oops(char *, char *);

#define COPYMODE        0644
#define BUFFERSIZE      4096

bool longver = false;
bool sortbytimemod = false;
bool sortBySize = false;

int main(int argc, char *argv[]){
	//check arg length using passed variable named argc. The tokenization
	// of these arguments should be stored in the 2-d array
	// checking changes
	int opt, index, count =0;
	
	while((opt = getopt_long(argc,argv,"lts")) != -1){
		switch(opt){
		case 'l': 
		longver = true;
        count++;
		break;
		case 't': 
        sortbytimemod = true;
        count++;
		break;
		case 's': 
        sortBySize = true;
        count++;
		break;
		default:
			printf("Usage %s enter cp or ls first \n", argv[0]);
			return 1;
		}

	}
	
	for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
	
    //debugging
	//int opt1 = optind;
	//printf("%d, value of longver: %d, index = %d\n", opt1, longver, index);
	
	
	if((strcmp(argv[optind], "ls")) == 0){	
		if ( ((argc - count) == optind)  || (argc == 2))
				do_ls( "." );
			else
				do_ls(argv[(argc -1)]);
		}
	else if((strcmp(argv[optind], "cp")) == 0){
		if(argc != 4){
            fprintf( stderr, "usage: myFS cp source destination\n");
            exit(1);           
        }
        int     in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE];
        
        /* open files	*/
        int i;
        for(i = 0; i < argc; i++){
        	printf("%s\n", argv[i]);
        }

        if ( (in_fd=open(argv[2], O_RDONLY)) == -1 )
                oops("Cannot open ", argv[1]);

        if ( (out_fd=creat( argv[3], COPYMODE)) == -1 )
                oops( "Cannot creat", argv[2]);
	
						/* copy files	*/

        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", argv[3]);
        if ( n_chars == -1 )
			oops("Read error from ", argv[2]);

						/* close files	*/

        if ( close(in_fd) == -1 || close(out_fd) == -1 )
            oops("Error closing files","");
    }
	return 0;
}
//error function
void oops(char *s1, char *s2)
{
        fprintf(stderr,"Error: %s ", s1);
        perror(s2);
        exit(1);
}


/*
 * 
 * this is the start of the ls function
 * 
 * 
 * 
 * */

void do_ls( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

    //this if statement does two things. first it opens the directory passed into do_ls
    //then it error catches
	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls2: cannot open %s\n", dirname);
	else
	{
        //direntp is a structure which holds info about file
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
			if (strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0)
				continue;
                
            if(longver == true)
                dostat( direntp->d_name );
            else{
                printf("%s\n", direntp->d_name);
            }
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
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
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

#include	<pwd.h>

char *uid_to_name( uid_t uid )
/*
 *	returns pointer to username associated with uid, uses getpw()
 */
{
	struct	passwd *getpwuid(), *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}

#include	<grp.h>

char *gid_to_name( gid_t gid )
/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
{
	struct group *getgrgid(), *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

