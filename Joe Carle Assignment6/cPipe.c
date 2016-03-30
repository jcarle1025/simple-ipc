#include "capPipe.h"

int a1[2];
int a2[2];
int b1[2];
int b2[2];

int main(){
	FILE *fp;
	pid_t pid1, pid2, pid3;

	if (pipe(a1) < 0)
		fprintf(stderr, "pipe error \n");
	if (pipe(a2) < 0)
		fprintf(stderr, "pipe error \n");
	if (pipe(b1) < 0)
		fprintf(stderr, "pipe error \n");
	if (pipe(b2) < 0)
		fprintf(stderr, "pipe error \n");

	fp = fopen("mobydick.txt", "r");
	if (fp == NULL){
		perror("Error opening file");
		return (-1);
	}
	
	//All pipes are made and file is open. Ready to fork()
	//PROCESS 1
	pid1 = fork();
	if (pid1 == 0){
		char * line, *sigA;
		printf("P1 STARTED\n");
		line = (char*)malloc(sizeof(char));
		line = returnLine(fp);
		
		while(line!=NULL){
			//printf("\tLine is: %s\n", line);
			write(a1[1], line, strlen(line)+1);
			sleep(1);
	
			sigA = (char*)malloc(sizeof(char));
			read(a2[0], sigA, MAX); 
			//if(strcmp(sigA, "done") == 0)
				//printf("\tP2 received\n");
			
			free(sigA);
			free(line);
			line = (char*)malloc(sizeof(char));
			line = returnLine(fp);
		}
		
		write(a1[1], "FIN", MAX);
		printf("P1 FINISHED\n");
		exit(0);
	}
	
	//PROCESS 2
	pid2 = fork();
	if(pid2 == 0){
		printf("P2 STARTED\n");
		char *line2, *sig;
		sig = (char*)malloc(sizeof(char));
		line2 = (char*)malloc(sizeof(char));

		while(1){
			read(a1[0], line2, MAX);
			if(strcmp(line2, "FIN") == 0)
				break;
			write(a2[1], "done", SIGMAX);
			//sleep(1);
		
			changeCaps(line2);
		
			write(b1[1], line2, MAX);
			sleep(1);
			
			read(b2[0], sig, SIGMAX);
			//if(strcmp(sig, "done") == 0)
				//printf("\tP3 recieved\n");
		}
		write(b1[1], "FIN", MAX);
		printf("P2 FINISHED\n");
		exit(0);
	}
	
	//PROCESS 3
	pid3 = fork();
	if (pid3 == 0){
		printf("P3 STARTED\n");
		FILE *pf;
		pf = fopen("mobydicknew.txt", "w");
		char *line3;
		line3 = (char*)malloc(sizeof(char));
		
		while(1){
			line3 = realloc(line3, sizeof(char)*MAX);
			
			read(b1[0], line3, MAX);
			if(strcmp(line3, "FIN") == 0)
				break;
			
			char l3[strlen(line3)+1];
			int i;
			for(i = 0; i<strlen(line3); i++){
				l3[i] = *(line3+i);		
			}
			l3[i] = '\n';
			
			//printf("P3 is writing...%s\n", line3);
			fwrite(l3, sizeof(char), sizeof(l3), pf);
			
			write(b2[1], "done", SIGMAX);
			//sleep(1);
		}
		fclose(pf);
		printf("P3 FINISHED\n");
		exit(0);
	}
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);

	return 0;
}	
