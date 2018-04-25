/*
 * filesysmain.c
 *
 *  Created on: Apr 23, 2018
 *      Author: peter
 */


#include <stdio.h>

int main(int argc, char *argv[]){
	//check arg length using passed variable named argc. The tokenization
	// of these arguments should be stored in the 2-d array

	if(argc < 2){
		printf("Usage: %s [commands]", argv[0]);
	}else{
		if(argv[1] == "ls"){
			//check for arguments and flags
		}
		else if(argv[1] == "cp"){
			//check for arguments and flags
		}
	}

}
