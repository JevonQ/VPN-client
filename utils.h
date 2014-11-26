#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include "libs/tinyxml/tinyxml.h"
#include "settings.h"
#include <stdio.h>
#if defined(Q_OS_WIN)
#include <winutils.h>
#else
#include <linuxutils.h>
#endif
using namespace std;

/*
 * We should provide the following functions to use:
 * 1 parser XML file of server list
 * 2 read and save settings of client
 */
typedef struct server_s
{
    char address[20];
    char location[64];
    float load;
    char username[32];
    char password[32];
    char caption[32];
    int ping;
    int res;
} Server;

int getXmlNodeCount(char *xmlFile);
int getServerList(char *xmlFile, Server list[]);
int getCountryList(Server *serverList, int num, Server *countryList);
void encrypt(const char *i, char *result);
void decrypt(const char *i, char *result);
int getSettings(char *settingsFile, settings *s);
int saveSettings(char *settingsFile, settings s);
int getTemplateFile(char *templateFile, char *link);
void setTemplateFile(char *templateFile, int mode);
int getAccountInfo(char *file, char *username, char *password, int *flag);
void saveAccountInfo(char *file, char *username, char *password, int flag);
int getServerListFile(char *serverListFile, char *link);
bool verifyAccount(char *xmlFile);
int replaceStr(char *p_result,char* p_source,char* p_seach,char *p_repstr);

#endif //end of UTILS_H
