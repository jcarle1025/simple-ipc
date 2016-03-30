#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#define MAX 80
#define SIGMAX 5

void changeCaps();
int testEOL(char a);
int testEOF(char a);
char * returnLine(FILE *fp);

int testEOL(char a){
	if(a == '\n') {return 1;}
	else {return 0;}
}

int testEOF(char a){
	if (a == EOF){return 1;}
	else {return 0;}
}

char * returnLine(FILE *fp){
	char c, *line;
	int i = 0;
	line = NULL;
	line = realloc(line, sizeof(char));
	
	c = fgetc(fp);
	
	if (testEOF(c)) {return NULL;}
	
	while (!testEOL(c)){
		*(line+i) = c;
		i++;
		line = realloc(line, sizeof(char)*(i+1));
		c = fgetc(fp);
	}
	*(line+i) = '\0';
	return line;
}

void changeCaps(char *line){
	int i;
	for(i = 0; i<strlen(line); i++){
		if(*(line+i) == toupper(*(line+i)))
			*(line+i) = tolower(*(line+i));
		else if(*(line+i) == tolower(*(line+i)))
			*(line+i) = toupper(*(line+i));
	}

	//printf("\tNew Line is: %s\n", line);
}
