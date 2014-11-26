#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int downloadFile(char *templateFile, char *link)
{
    CURL *curl;
    FILE * fd = NULL;
    fd = fopen(templateFile, "wb");
    curl = curl_easy_init ();
    curl_easy_setopt (curl, CURLOPT_URL, link);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, fd);
    curl_easy_perform (curl);   /* ignores error */
    fclose(fd);
    curl_easy_cleanup (curl);
    return 0;
}

int getServerListFile(char *serverListFile, char *link)
{
    return downloadFile(serverListFile, link);
}

int getTemplateFile(char *templateFile, char *link)
{
    return downloadFile(templateFile, link);
}

int main()
{
    int res = 0;
    char tFile[] = "/Users/lei/.safejumper/linux-template.tpl";
    char tLink[] = "https://proxy.sh/ovpn-linux.tpl";
    char sFile[] = "/Users/lei/.safejumper/serverlist.xml";
    char sLink[] = "https://proxy.sh/access.php?u=ukjynydk&p=Kaz94m07qB&hub=true";
    // test case 1: download template file
    if ((res = getTemplateFile(tFile, tLink)) != 0) {
        printf("getTemplateFile function return is %d\n", res);
    }

    // test case 2: download server list xml file
    if ((res = getServerListFile(sFile, sLink)) != 0) {
        printf("getServerListFile function return is %d\n", res);
    }
}
