#ifndef WINUTILS_H
#define WINUTILS_H

int checkFile(char *filename);
int addAppToRun(char *appName, char *runPath);
int isAppInRun(char *appName);
int delAppFromRun(char *appName);
int downNetworkInterfaces();
int getIPv6Enabled ();
int updateIPv6State();
int pingAddress(char *ip);

#endif
