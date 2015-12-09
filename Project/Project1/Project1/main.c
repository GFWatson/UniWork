#include "altlog.h"

int main(int argc, char *argv[]) {
	activateLogging();
	writeLog(DEBUG, "%s, %s, %f", "test", "test2", 7.27);
	writeLog(DEBUG, "%d", 7327);
	printLogs(1, 0, 0, 0, 1);
	deactivateLogging();

}