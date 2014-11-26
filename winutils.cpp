#include "winutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <fcntl.h>

#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#define MAX_INTERFACES 100

int addAppToRun(char *appName, char *runPath)
{
    HKEY hRoot = HKEY_LOCAL_MACHINE;
    char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    HKEY hKey;

    DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
    LONG lRet = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    if(lRet != ERROR_SUCCESS)  
    {
        printf("failed to find !\n");
        return -1;
    }

    lRet = RegSetValueEx(hKey, appName, 0, REG_SZ, (BYTE *)runPath, strlen(runPath));
    RegCloseKey(hKey);
    if(lRet != ERROR_SUCCESS)
    {
        printf("failed to reg !\n");
        return -1;
    }  
    return 0;
}

/*
 * 1 the application is in run
 * 0 the application is not in run
 * -1 we encounter an issue while getting HOME directory
 */
int isAppInRun(char *appName)
{
    HKEY hRoot = HKEY_LOCAL_MACHINE;
    char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    char runPath[1024];
    HKEY hKey;
    int count = 0;

    (void) memset(runPath, 0, sizeof (runPath));

    DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
    LONG lRet = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    if(lRet != ERROR_SUCCESS)  
    {
        printf("failed to find !\n");
        return -1;
    }

    lRet = RegQueryValueEx(hKey, appName, 0, REG_SZ, (LPBYTE)runPath, &count);
    RegCloseKey(hKey);
    if(lRet != ERROR_SUCCESS)
    {
        printf("failed to reg !\n");
        return -1;
    }
    if (count != 0) {
        return 1;
    } else {
        return 0;
    }
}

int delAppFromRun(char *runName)
{
    HKEY hRoot = HKEY_LOCAL_MACHINE;
    char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    char runPath[1024];
    HKEY hKey;
    int count = 0;

    (void) memset(runPath, 0, sizeof (runPath));

    DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
    LONG lRet = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    if(lRet != ERROR_SUCCESS)  
    {
        printf("failed to find !\n");
        return -1;
    }

    lRet = RegDeleteKey(hKey, (LPTSTR)appName);
    RegCloseKey(hKey);
    if(lRet != ERROR_SUCCESS)
    {
        printf("failed to reg !\n");
        return -1;
    }
    return 0;
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

bool is64bit()
{
    bool res = false;

    /* 
     * When this application is compiled as a 32-bit app,
     * and run on a native 64-bit system, Windows will run
     * this application under WOW64.  WOW64 is the Windows-
     * on-Windows subsystem that lets native 32-bit applications
     * run in 64-bit land.  This calls the kernel32.dll
     * API to see if this process is running under WOW64.
     * If it is running under WOW64, then that clearly means
     * this 32-bit application is running on a 64-bit OS,
     * and IsWow64Process will return true.
     */
    IW64PFP IW64P = (IW64PFP)GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
    if(IW64P != NULL){
        IW64P(GetCurrentProcess(), &res);
    }
    return res;
}

int downNetworkInterfaces()
{
    char cmd[32];

    if (is64bit()) {
        strcpy(cmd, "devcon_64.exe disable =net");
    } else {
        strcpy(cmd, "devcon_32.exe disable =net");
    }

    return system(cmd);
}

int getIPv6Enabled ()
{
    return 0;
}

int updateIPv6State()
{
    return 0;
}

int pingAddress(char *dstIp) {
    int res = 0;
    HANDLE MyHandle;
    char Randomdata[20] = "Stuff to send away!";
    DWORD mainint,lenofbuffer;
    ICMP_ECHO_REPLY reply; //ICMP_ECHO_REPLY data structure
    IPAddr address;
    lenofbuffer = strlen(Randomdata) + sizeof(ICMP_ECHO_REPLY) + 8; //Make buffer large enough to contain data sent and the size of the ICMP_ECHO_REPLY data structure
    char* bufferpointer = new char[lenofbuffer];
    MyHandle = IcmpCreateFile(); //Create Handle
    address = inet_addr(dstIp); // convert IP address into IPAddr object
    //Perform the operation!, timeout is 300ms
    res = IcmpSendEcho(MyHandle, address, Randomdata, strlen(Randomdata), NULL, bufferpointer, lenofbuffer, 300);
    memcpy(&reply, bufferpointer, sizeof(reply)); //Copy the returned information to an ICMP_ECHO_REPLY
    IcmpCloseHandle(MyHandle);
    if(res == 0){ //If the status of the ICMP_ECHO_REPLY structures stored in bufferpointer is more than 0
        return -1;
    }
    return reply.RoundTripTime;
}
