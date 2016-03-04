#pragma once

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

//enum for tagging
enum tagEnum { DEBUG, ERROR, PERF, MIS };

//struct for individual logs
//also form their own linked list
struct indivLog {
	char message[4096];
	enum tagEnum tag;
	struct indivLog *next;
};

//root and current nodes
struct indivLog *root;
struct indivLog *curr;

//bool for whether it should be logging or not
bool isLogging;

//turn on/off logging
void activateLogging();
void deactivateLogging();

//log function
//creates indivLog and adds to log list
//only logs if logging is active
void writeLog(enum tagEnum t, char * format, ...);

//file to log into
FILE * aFile;

//print func
//prints logs, clumped into tags
//only prints which tags have been chosen
//can print to text file or console
//will delete current list after printing
//will overwrite anything currently in file
void printLogs(bool printDebug, bool printError, bool printPerf, bool printMis, bool printToCons);
