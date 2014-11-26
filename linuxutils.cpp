#include "linuxutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/times.h>
#include <fcntl.h>

#include <net/if.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_INTERFACES 100

int checkFile(char *cmdname)
{
    int res = 0;
    char whichcmd[100] = {0};
	sprintf(whichcmd, "/usr/bin/which %s", cmdname);
    res = system(whichcmd);
    return res;
}

int addAppToRun(char *appName, char *runPath)
{
    int res = 0;
    char *homedir = getenv("HOME");
    if (homedir == NULL) {
        printf("The application can not get the HOME directory!\n");
        return 1;
    }

    char autoStartFile[50];
    memset(autoStartFile, '\0', sizeof (autoStartFile));
    char createConfigDirCmd[50];
    memset(createConfigDirCmd, '\0', sizeof (createConfigDirCmd));
    sprintf(createConfigDirCmd, "mkdir -p %s/.config/autostart", homedir);
    res = system(createConfigDirCmd);
    if (res != 0 ) {
        printf("Can not create autostart directory!\n");
        return res;
    }

    sprintf(autoStartFile, "%s/.config/autostart/%s.desktop", homedir, appName);
    FILE *autoStartStream = fopen(autoStartFile, "wb");
    fputs("[Desktop Entry]\n", autoStartStream);
    fputs("[Desktop Entry]\n", autoStartStream);
    fputs("Type=Application\n", autoStartStream);
    fputs("Exec=", autoStartStream);
    fputs(runPath, autoStartStream);
    fputs("Hidden=false\n", autoStartStream);
    fputs("NoDisplay=false\n", autoStartStream);
    fputs("X-GNOME-Autostart-enabled=true\n", autoStartStream);
    fputs("Name=", autoStartStream);
    fputs(appName, autoStartStream);
    fputs("\nComment=", autoStartStream);
    fputs(appName, autoStartStream);
    fputs(" startup on login\n", autoStartStream);
    fclose(autoStartStream);

    return res;
}

/*
 * 1 the application is in run
 * 0 the application is not in run
 * -1 we encounter an issue while getting HOME directory
 */
int isAppInRun(char *runName)
{
    char *homedir = getenv("HOME");
    FILE *file = NULL;
    if (homedir == NULL) {
        printf("The application can not get the HOME directory!\n");
        return -1;
    }

    char autoStartFile[50];
    (void) memset(autoStartFile, '\0', sizeof (autoStartFile));
    (void) sprintf(autoStartFile, "%s/.config/autostart/%s.desktop", homedir, runName);
    if ((file = fopen(autoStartFile, "r")) != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

int delAppFromRun(char *runName)
{
    int res = 0;
    FILE *file = NULL;
    char *homedir = getenv("HOME");
    if (homedir == NULL) {
        printf("The application can not get the HOME directory!\n");
        return -1;
    }

    char autoStartFile[50];
    (void) memset(autoStartFile, '\0', sizeof (autoStartFile));
    (void) sprintf(autoStartFile, "%s/.config/autostart/%s.desktop", homedir, runName);
    if ((file = fopen(autoStartFile, "r")) != NULL) {
        fclose(file);
        res = remove(autoStartFile);
    }

    return res;
}

int getActiveInterfacesCount()
{
    int sock;
    struct ifconf ifconf;
    struct ifreq ifreq[MAX_INTERFACES];
    struct ifreq ifr;

    int interfaces;
    int i;

    printf("Active network interfaces:\n");
    printf("--------------------------\n");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        printf("Error creating socket");

    ifconf.ifc_buf = (char *) ifreq;
    ifconf.ifc_len = sizeof ifreq;

    if (ioctl(sock, SIOCGIFCONF, &ifconf) == -1) {
        close(sock);
        printf("Error ioctl");
        return -1;
    }

    interfaces = ifconf.ifc_len / sizeof(ifreq[0]);

    for (i=0; i < interfaces; i++) {
        memset( &ifr, 0, sizeof ifr);

        memcpy( &ifr.ifr_name, &ifreq[i].ifr_name, IF_NAMESIZE );
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            close(sock);
            printf("Error ioctl(2)");
            return -1;
        }

        printf("%s, flags(%d)", ifr.ifr_name, ifr.ifr_flags);

        if ((ifr.ifr_flags & IFF_LOOPBACK) == IFF_LOOPBACK)
            printf(" LOOPBACK");

        if ((ifr.ifr_flags & IFF_UP) == IFF_UP)
            printf(" up");
        else
          printf(" down");
        
        printf("\n");
    }

    close(sock);
    return interfaces;
}

int downNetworkInterfaces()
{
    int sock;
    struct ifconf ifconf;
    struct ifreq ifreq[MAX_INTERFACES];
    struct ifreq ifr;

    int interfaces;
    int i;

    printf("Disabling interfaces...");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Error creating socket");
        return -1;
    }

    ifconf.ifc_buf = (char *) ifreq;
    ifconf.ifc_len = sizeof ifreq;

    if (ioctl(sock, SIOCGIFCONF, &ifconf) == -1) {
        close(sock);
        printf("Error ioctl");
        return -1;
    }

    interfaces = ifconf.ifc_len / sizeof(ifreq[0]);

    for (i=0; i < interfaces; i++) {
        memset( &ifr, 0, sizeof ifr);

        memcpy( &ifr.ifr_name, &ifreq[i].ifr_name, IF_NAMESIZE );
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            perror("Error ioctl(2)");
            return -2;
        }

        if ((ifr.ifr_flags & IFF_LOOPBACK) == IFF_LOOPBACK)
            continue;  // don't disable loopbacks

        if ((ifr.ifr_flags & IFF_UP) == IFF_UP) {
            ifr.ifr_flags = ifr.ifr_flags & ~IFF_UP;
            if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
                perror("Error ioctl(3)");
                return -2;
            }
        }
    }

    printf("Done.\n");
    close(sock);
    return 0;
}

int dnsAccessible(int timeout)
{
    (void) timeout;
    char digcmd[32] = "dig NS +aaonly com.";
    char result[1024];
    FILE *ptr = NULL;

    if((ptr=popen(digcmd, "r"))!=NULL) 
    {   
        while(fgets(result, 1024, ptr)!=NULL)   
        {
            if (!strcmp(";; ANSWER SECTION:", result))
                return 1;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else  
    {   
        printf("popen %s error\n", digcmd);   
        return -1;
    }   

    return 1;
}

int getIPv6Enabled ()
{
    return 0;
}

int updateIPv6State()
{
    return 0;
}

int pingAddress(char * addr)
{
    char pingcmd[64] = {0};
    char result[1024];
    FILE *ptr = NULL;
    int time = 0;

    sprintf(pingcmd, "ping %s -c 1", addr);
    if((ptr=popen(pingcmd, "r"))!=NULL) 
    {   
        while(fgets(result, 1024, ptr)!=NULL)   
        {
            if (strstr(result, "time=") != NULL) {
                sscanf(result, "time=%d ms", time);
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", digcmd);   
        return -1;
    }
    return time;
}
