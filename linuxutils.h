#ifndef LINUXUTILS_H
#define LINUXUTILS_H

int checkFile(char *filename);
int addAppToRun(char *appName, char *runPath);
int isAppInRun(char *appName);
int delAppFromRun(char *appName);
int getActiveInterfacesCount();
int downNetworkInterfaces();
int dnsAccessible(int timeout);
int getIPv6Enabled ();
int updateIPv6State();

#endif
