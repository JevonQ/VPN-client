#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#if defined(Q_OS_WIN)
#include <windows.h>
#endif

int getXmlNodeCount(char *xmlFile)
{
    TiXmlDocument doc(xmlFile);
    bool loadOkay = doc.LoadFile();
    int count = 0;
    TiXmlNode* node = 0;
    TiXmlElement* itemElement = 0;

    if ( !loadOkay )
    {
        printf( "Could not load test file %s. Error='%s'. Exiting.\n", xmlFile, doc.ErrorDesc() );
        return -1;
    }

    node = doc.RootElement();
    itemElement = node->FirstChildElement("server_list");
    for( node = itemElement->FirstChild("server");
         node;
         node = node->NextSibling( "server" ) )
    {
        count++;
    }
    return count;
}

int getServerList(char *xmlFile, Server list[])
{
    TiXmlDocument doc(xmlFile);
    bool loadOkay = doc.LoadFile();
    TiXmlNode* node = 0;
    TiXmlNode *cred = 0;
    TiXmlElement* itemElement = 0;
    int num = 0;
    char username[32];
    char password[32];

    if ( !loadOkay )
    {
        printf( "Could not load test file %s. Error='%s'. Exiting.\n", xmlFile, doc.ErrorDesc() );
        return 1;
    }

    node = doc.RootElement();
    cred = node->FirstChild("credentials");
    strcpy(username, cred->FirstChildElement("username")->GetText());
    strcpy(password, cred->FirstChildElement("password")->GetText());

    itemElement = node->FirstChildElement("server_list");
    for( node = itemElement->FirstChild("server");
         node;
         node = node->NextSibling( "server" ) )
    {
        strcpy(list[num].address, node->FirstChildElement("address")->GetText());
        strcpy(list[num].location, node->FirstChildElement("location")->GetText());
        list[num].load = atof(node->FirstChildElement("server_load")->GetText());
        strcpy(list[num].username, username);
        strcpy(list[num].password, password);
        //printf("%d - address: %s, location: %s, load: %f\n", num, list[num].address, list[num].location, list[num].load);
        num ++;
    }

    printf("The first server location is %s\n", list[0].location);
    return 0;
}

/*
 * return the number of country if success
 */
int getCountryList(Server serverList[], int num, Server countryList[])
{
    int count = 0;
    int i = 0;
    char *s = NULL;
    char country[64] = {0};
    char temp[32] = {0};

    for (; i < num; i ++) {
        (void) memset(country, 0, sizeof (country));
        (void) memset(temp, 0, sizeof (temp));
        (void) strcpy(temp, serverList[i].location);
        s = strtok(temp, " ");
        while (s != NULL) {
            if (((strlen(s) ==1 || strlen(s) ==2) ? !(atoi(s) >= 0 && atoi(s) <= 99): strcmp(s, "Hub"))) {
                sprintf(country, "%s ", s);
            }
            s = strtok(NULL," ");
        }
        country[strlen(country)-1] = '\0';
        if (strcmp(country, countryList[count-1<0 ? 0 :count-1].location)) {
            strcpy(countryList[count].location, country);
            strcpy(countryList[count].address, serverList[i].address);
            strcpy(countryList[count].username, serverList[i].username);
            strcpy(countryList[count].password, serverList[i].password);
            strcpy(countryList[count].caption, serverList[i].caption);
            countryList[count].res = i;
            count ++;
        }
        serverList[i].res = (count -1 < 0) ? 0 : (count -1);
    }
    return (count);
}

#define C1 52845
#define C2 22719
#define C3 1213

void encrypt(const char *in, char *result)
{
    int size = strlen(in);
    char input[100] = {'\0'};
    char str[2] = {'1', '2'};
    int i,j, k=0;
    int key = C3;

    (void) memset(result, '\0', sizeof (result));
    strcpy(result, in);
    strcpy(input, in);
    printf("the input string length is %d\n", size);
    for(i = 0; i < size; i ++)
    {
       result[i] = input[i]^(key>>8);
       key = ((unsigned char)result[i]+key)*C1+C2;
    }
    strcpy(input, result);
    (void) memset(result, '\0', sizeof (result));

    for(i=0; i< (int)strlen(input); i++)
    {
       j = (unsigned char)input[i];
       str[0] = 65+j/26;
       str[1] = 65+j%26;
       result[k++] = str[0];
       result[k++] = str[1];
    }
    result[k] = '\0';
}


void decrypt(const char *in, char *result)
{
    int size = strlen(in);
    char input[100] = {'\0'};
    char str[1] = {'1'};
    int i,j, k = 0;
    int key = C3;

    (void) memset(result, '\0', sizeof (result));
    strcpy(input, in);
    for (i=0; i < size/2; i++)
    {
       j = ((unsigned char)input[2*i]-65)*26;
       j += (unsigned char)input[2*i+1]-65;
       str[0] = j;
       result[k++] = str[0];
    }
    result[k] = '\0';
    memset(input, '\0', sizeof (input));
    strcpy(input, result);
    for (i=0; i< (int) strlen(input); i++)
    {
       result[i] = (unsigned char)input[i]^(key>>8);
       key = ((unsigned char)input[i]+key)*C1+C2;
    }
}

int getSettings(char *settingsFile, settings *s)
{
    TiXmlDocument doc(settingsFile);
    bool loadOkay = doc.LoadFile();
    TiXmlNode* node = 0;
    TiXmlElement* itemElement = 0;
    char itemName[11][32] = {"primaryDns", "secondaryDns", "activatePort",
        "launchOnLogin", "connectWhenLaunches", "reConnectWhenDrops",
        "wifiAutoConnect", "displayFullServerList", "killInterfacesWhenEndVPN",
        "disableIPv6", "dns"};
    char result[100] = {'\0'};

    if ( !loadOkay )
    {
        printf( "Could not load settings file %s. Error='%s'. Exiting.\n",
                settingsFile, doc.ErrorDesc() );
        return 1;
    }

    node = doc.RootElement();
    itemElement = node->FirstChildElement(itemName[0]);
    decrypt(itemElement->GetText(), result);
    strcpy(s->dns1, result);
    itemElement = node->FirstChildElement(itemName[1]);
    decrypt(itemElement->GetText(), result);
    strcpy(s->dns2, result);
    itemElement = node->FirstChildElement(itemName[2]);
    decrypt(itemElement->GetText(), result);
    s->activatePort = atoi(result);

    itemElement = node->FirstChildElement(itemName[3]);
    decrypt(itemElement->GetText(), result);
    s->launchOnLogin = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[4]);
    decrypt(itemElement->GetText(), result);
    s->connectWhenLaunches = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[5]);
    decrypt(itemElement->GetText(), result);
    s->reConnectWhenDrops = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[6]);
    decrypt(itemElement->GetText(), result);
    s->wifiAutoConnect = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[7]);
    decrypt(itemElement->GetText(), result);
    s->displayFullServerList = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[8]);
    decrypt(itemElement->GetText(), result);
    s->killInterfacesWhenEndVPN = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[9]);
    decrypt(itemElement->GetText(), result);
    s->disableIPv6 = atoi(result) ? true: false;
    itemElement = node->FirstChildElement(itemName[10]);
    decrypt(itemElement->GetText(), result);
    s->dns = atoi(result) ? true: false;

    return 0;
}

int saveSettings(char *settingsFile, settings s)
{
    char result[100];
    TiXmlDocument test(settingsFile);
    if (test.LoadFile()) {
        char cmd[100];
        (void) memset(cmd, '\0', sizeof (cmd));
        sprintf(cmd, "rm -f %s", settingsFile);
        system(cmd);
    }
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
    doc.LinkEndChild( decl );  

    TiXmlElement * root = new TiXmlElement( "settings" );  
    doc.LinkEndChild( root );  

    TiXmlElement* primaryDns = new TiXmlElement("primaryDns");  
    encrypt(s.dns1, result);
    primaryDns->LinkEndChild( new TiXmlText(result));
    root->LinkEndChild(primaryDns);
     
    TiXmlElement * secondaryDns = new TiXmlElement("secondaryDns");
    encrypt(s.dns2, result);
    secondaryDns->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(secondaryDns);
    
 
    TiXmlElement * activatePort = new TiXmlElement("activatePort");
    char activatePortString[6];
    (void) memset(activatePortString, '\0', sizeof (activatePortString));
    sprintf(activatePortString, "%d", s.activatePort);
    encrypt(activatePortString, result);
    activatePort->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(activatePort);
     
    TiXmlElement* launchOnLogin = new TiXmlElement("launchOnLogin");
    encrypt(s.launchOnLogin ? "1" : "0", result);
    launchOnLogin->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(launchOnLogin);

    TiXmlElement* connectWhenLaunches = new TiXmlElement("connectWhenLaunches");  
    encrypt(s.connectWhenLaunches ? "1" : "0", result);
    connectWhenLaunches->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(connectWhenLaunches);

    TiXmlElement* reConnectWhenDrops = new TiXmlElement("reConnectWhenDrops");  
    encrypt(s.reConnectWhenDrops ? "1" : "0", result);
    reConnectWhenDrops->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(reConnectWhenDrops);

    TiXmlNode* wifiAutoConnect = new TiXmlElement("wifiAutoConnect");  
    encrypt(s.wifiAutoConnect? "1" : "0", result);
    wifiAutoConnect->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(wifiAutoConnect);

    TiXmlElement* displayFullServerList = new TiXmlElement("displayFullServerList");  
    encrypt(s.displayFullServerList? "1" : "0", result);
    displayFullServerList->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(displayFullServerList);

    TiXmlElement* killInterfacesWhenEndVPN = new TiXmlElement("killInterfacesWhenEndVPN");  
    encrypt(s.killInterfacesWhenEndVPN? "1" : "0", result);
    killInterfacesWhenEndVPN->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(killInterfacesWhenEndVPN);

    TiXmlElement* disableIPv6 = new TiXmlElement("disableIPv6");  
    encrypt(s.disableIPv6? "1" : "0", result);
    disableIPv6->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(disableIPv6);

    TiXmlElement* dns = new TiXmlElement("dns");  
    encrypt(s.dns? "1" : "0", result);
    dns->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(dns);
    
    doc.SaveFile(settingsFile);  
    return 0;
}

int downloadFile(char *templateFile, char *link)
{
    int res = 0;
#if defined(Q_OS_WIN)
    char cmd[1024] = {0};
    sprintf(cmd, "bin\\wget.exe --no-check-certificate \"%s\" -O \"%s\" -q", link, templateFile);
     //res = system(cmd);
    res = WinExec(cmd, SW_HIDE);
    Sleep(10 * 1000);
#else
    CURL *curl;
    FILE * fd = NULL;
    fd = fopen(templateFile, "wb");
    curl = curl_easy_init ();
    curl_easy_setopt (curl, CURLOPT_URL, link);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 50L);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, fd);
    res = curl_easy_perform (curl);   /* ignores error */
    fclose(fd);
    curl_easy_cleanup (curl);
#endif
    return res;
}

int getServerListFile(char *serverListFile, char *link)
{
    return downloadFile(serverListFile, link);
}

int getTemplateFile(char *templateFile, char *link)
{
    return downloadFile(templateFile, link);
}

void setTemplateFile(char *templateFile, int mode)
{
    (void) mode;
    FILE *file = fopen(templateFile, "wb");
    if (file == 0) {
        printf("Can not create Template File, %s\n", templateFile);
        return;
    }
    fputs("client\n", file);
    fputs("dev tun0\n", file);
    fputs("proto %%server-proto%%\n", file);
    fputs("remote-random\n", file);
    fputs("remote %%server-ip%% %%server-port%%\n", file);
    fputs("auth-user-pass\n", file);
    fputs("resolv-retry infinite\n", file);
    fputs("nobind\n", file);
    fputs("persist-key\n", file);
    fputs("persist-tun\n", file);
    fputs("verb 3\n", file);
    fputs("route-method exe\n", file);
    fputs("comp-lzo\n", file);
    fputs("route-delay 2\n", file);
    fputs("allow-pull-fqdn\n", file);
    fputs("ca ca-file-default\n", file);
    fclose(file);
}

int getAccountInfo(char *file, char *username, char *password, int *flag)
{
    TiXmlDocument doc(file);
    bool loadOkay = doc.LoadFile();
    TiXmlNode* node = 0;
    TiXmlElement* itemElement = 0;
    char itemName[3][32] = {"username", "password", "flag"};
    char result[100] = {'\0'};

    if ( !loadOkay )
    {
        printf( "Could not load test file %s. Error='%s'. Exiting.\n",
                file, doc.ErrorDesc() );
        return 1;
    }

    node = doc.RootElement();
    itemElement = node->FirstChildElement(itemName[0]);
    decrypt(itemElement->GetText(), result);
    strcpy(username, result);
    itemElement = node->FirstChildElement(itemName[1]);
    decrypt(itemElement->GetText(), result);
    strcpy(password, result);
    itemElement = node->FirstChildElement(itemName[2]);
    decrypt(itemElement->GetText(), result);
    *flag = atoi(result);

    return 0;
}

void saveAccountInfo(char *file, char *username, char *password, int flag)
{
    char result[100];
    TiXmlDocument test(file);
    if (test.LoadFile()) {
        remove(file);
    }
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
    doc.LinkEndChild( decl );  

    TiXmlElement * root = new TiXmlElement( "account" );  
    doc.LinkEndChild( root );  

    TiXmlElement* usr = new TiXmlElement("username");  
    encrypt(username, result);
    usr->LinkEndChild( new TiXmlText(result));
    root->LinkEndChild(usr);
     
    TiXmlElement * pwd = new TiXmlElement("password");
    encrypt(password, result);
    pwd->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(pwd);
    
    TiXmlElement* f = new TiXmlElement("flag");
    encrypt(flag ? "1" : "0", result);
    f->LinkEndChild(new TiXmlText(result));
    root->LinkEndChild(f);
    
    doc.SaveFile(file);  
}

bool verifyAccount(char *xmlFile)
{
    if (getXmlNodeCount(xmlFile) > 0) {
        return (true);
    } else {
        return (false);
    }
}

int replaceStr(char *p_result,char* p_source,char* p_seach,char *p_repstr)
{  
    int c = 0;
    int repstr_leng = 0;
    int searchstr_leng = 0;

    char *p1;
    char *presult = p_result;
    char *psource = p_source;
    char *prep = p_repstr;
    char *pseach = p_seach;
    int nLen = 0;

    repstr_leng = strlen(prep);
    searchstr_leng = strlen(pseach);

    do{
        p1 = strstr(psource,p_seach);

        if (p1 == 0)
        {
            strcpy(presult,psource);
            return c;
        }
        c++;

        nLen = p1 - psource;
        memcpy(presult, psource, nLen);

        memcpy(presult + nLen,p_repstr,repstr_leng);

        psource = p1 + searchstr_leng;
        presult = presult + nLen + repstr_leng;
    }while(p1);

    return c;
}
