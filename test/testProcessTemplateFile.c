#include <stdio.h>
#include <string.h>

void processTemplateFile(char *templateFile, char *cafile, char *server_proto, char *server_ip, char *server_port)
{
    char content[64][128];
    FILE *tFILE = NULL;
    FILE *cFILE = NULL;
    int i = 0;
    int count = 0;
    int ca = 0;
    char *point = NULL;
    (void) memset(content, '\0', sizeof (content));
    cFILE = fopen(cafile, "w");
    tFILE = fopen(templateFile, "r");
    while (fgets(content[count], 128, tFILE)) {
        if ((point = strstr(content[count], "%server-proto%")) != NULL) {
            sprintf(content[count], "proto %s\n", server_proto);
        } else if ((point = strstr(content[count], "%server-ip%")) != NULL) {
            sprintf(content[count], "remote %s %s\n", server_ip, server_port);
        } else if ((point = strstr(content[count], "<ca>")) != NULL) {
            ca ++;
            sprintf(content[count], "ca %s\n", cafile);
        } else if ((point = strstr(content[count], "</ca>")) != NULL) {
            break;
        }

        if (ca > 1) {
            fputs(content[count], cFILE);
            ca ++;
        }
        count ++;
    }
    fclose(tFILE);
    fclose(cFILE);

    tFILE = fopen(templateFile, "w");
    for (; i < count - ca + 2; i ++) {
        fputs(content[i], tFILE);
    }

    fclose(tFILE);
}

int main()
{
    processTemplateFile("/Users/lei/.safejumper/linux.tpl", "/Users/lei/.safejumper/ca-file", "tcp", "127.0.0.1", "443");
}
