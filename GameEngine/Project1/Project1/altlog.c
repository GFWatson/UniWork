#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "altlog.h"



void activateLogging() {
	//isLogging = true;
#ifdef _DEBUG
	isLogging = true;
#else
	isLogging = false;
#endif
	root = malloc(sizeof(struct indivLog));
	strcpy_s(root->message, 4096, "THIS IS THE ROOT! IF YOU'RE READING THIS SOMETHING HAS GONE WRONG!");
	root->tag = MIS;
	root->next = NULL;
	curr = root;
}

void deactivateLogging() {
	isLogging = false;
	//delete list
	if (root->next != NULL) {
		struct indivLog *nextLog;
		curr = root->next;
		nextLog = curr;
		while (curr != NULL) {
			nextLog = curr->next;
			free(curr);
			curr = nextLog;
		}
		root->next = NULL;
		curr = root;
	}
	//delete root and current nodes
	curr = root;
	free(root);
}


void writeLog(enum tagEnum t, char * format, ...) {

	if (isLogging) {
		//make log
		struct indivLog *l;
		l = malloc(sizeof(struct indivLog));
		//check there is space in memory
		if (l == NULL) {
			printf("Unable to log! No memory left!");
		}
		else {
			//fill in details
			l->tag = t;
			l->next = NULL;
			
			va_list aList;
			va_start(aList, format);
			vsnprintf(l->message, 4095, format, aList);
			va_end(aList);
			
			//add to end of list

			while (curr->next != NULL) {
				curr = curr->next;
			}
			curr->next = l;

		}
	}

}

void noLog(enum tagEnum t, char * format, ...) {

}

void printLogs(bool printDebug, bool printError, bool printPerf, bool printMis, bool printToCons) {
	//set current to root and traverse through list
	//will always miss root node
	//find all in each tag set wanted and print

	//console printing
	if (printToCons && isLogging) {
		curr = root;

		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		printf("\nCurrent date and time: %s", asctime(timeinfo));

		if (printDebug) {
			//print debug heading
			printf("\n========================\nDEBUG\n========================\n\n");
			//find and print logs
			while (curr->next != NULL) {
				curr = curr->next;
				printf(curr->message);
				printf("\n");
			}
		}

		curr = root;
		if (printError) {
			//print error heading
			printf("\n========================\nERROR\n========================\n\n");
			//find and print logs
			while (curr->next != NULL) {
				curr = curr->next;
				printf(curr->message);
				printf("\n");
			}
		}

		curr = root;
		if (printPerf) {
			//print performance heading
			printf("\n========================\nPERFORMANCE\n========================\n\n");
			//find and print logs
			while (curr->next != NULL) {
				curr = curr->next;
				printf(curr->message);
				printf("\n");
			}
		}

		curr = root;
		if (printMis) {
			//print miscellaneous heading
			printf("\n========================\nMISCELLANEOUS\n========================\n\n");
			//find and print logs
			while (curr->next != NULL) {
				curr = curr->next;
				printf(curr->message);
				printf("\n");
			}
		}

	}
	else if(isLogging) //text file printing
	{
		errno_t err;
		if ((err = fopen_s(&aFile, "..\\altlog.txt", "w")) == 0) {
			curr = root;

			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			fprintf(aFile, "\nCurrent date and time: %s", asctime(timeinfo));

			if (printDebug) {
				//print debug heading
				fprintf(aFile, "\n========================\nDEBUG\n========================\n\n");
				//find and print logs
				while (curr->next != NULL) {
					curr = curr->next;
					fprintf(aFile, curr->message);
					fprintf(aFile, "\n");
				}
			}

			curr = root;
			if (printError) {
				//print error heading
				fprintf(aFile, "\n========================\nERROR\n========================\n\n");
				//find and print logs
				while (curr->next != NULL) {
					curr = curr->next;
					fprintf(aFile, "PRINTING LOG\n");
					fprintf(aFile, curr->message);
					fprintf(aFile, "\n");
				}
			}

			curr = root;
			if (printPerf) {
				//print performance heading
				fprintf(aFile, "\n========================\nPERFORMANCE\n========================\n\n");
				//find and print logs
				while (curr->next != NULL) {
					curr = curr->next;
					fprintf(aFile, curr->message);
					fprintf(aFile, "\n");
				}
			}

			curr = root;
			if (printMis) {
				//print miscellaneous heading
				fprintf(aFile, "\n========================\nMISCELLANEOUS\n========================\n\n");
				//find and print logs
				while (curr->next != NULL) {
					curr = curr->next;
					fprintf(aFile, curr->message);
					fprintf(aFile, "\n");
				}
			}
		}
		fclose(aFile);
	}

	//delete list
	if (root->next != NULL) {
		struct indivLog *nextLog;
		curr = root->next;
		nextLog = curr;
		while (curr != NULL) {
			nextLog = curr->next;
			free(curr);
			curr = nextLog;
		}
		root->next = NULL;
		curr = root;
	}

}
