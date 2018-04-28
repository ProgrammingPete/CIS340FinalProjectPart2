/*
 * filesysmain.c
 *
 *  Created on: Apr 23, 2018
 *      Author: peter
 */


#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
	//check arg length using passed variable named argc. The tokenization
	// of these arguments should be stored in the 2-d array
	// checking changes
	int opt;
	bool longver = false;
	bool sortbytimemod = false;
	bool sortBySize = false;
	while((opt = getopt (argc,argv,"lts")) != -1){
		switch(opt){
		case 'l': longver = true;
		break;
		case 't': sortbytimemod = true;
		break;
		case 's': sortBySize = true;
		break;
		default:
			printf("Usage %: enter cp or ls first \n", argv[0]);
		}

	}
	
	//int opt1 = optind;
	//printf("%d", opt1);
	
	int i;
	char str[100];
	
	if((strcmp(argv[1], "ls")) == 0){
		while(argv[i][0] != '/0'){
			if(argv[i][0] ==  134){
				strcpy(argv[i], str);
			}
		}
	}else if((strcmp(argv[1], "ls")) == 0){
		printf("hello");
	}
}
