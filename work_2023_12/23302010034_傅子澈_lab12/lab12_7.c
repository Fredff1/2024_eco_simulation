#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_url(const char *url, char *protocol, char *domain, char *port) {
    int start=0;
    int len=strlen(url);
    int pos=0;
    int copy_pos=0;
    while(url[pos]!=':'){
        protocol[copy_pos++]=url[pos++];
    }
    pos+=3;
    protocol[copy_pos]='\0';
    //printf("%d\n",pos);
    copy_pos=0;
    //printf("%d\n",pos);
    while(url[pos]!=':'&&url[pos]!='\0'){
        domain[copy_pos++]=url[pos++];
    }
    domain[copy_pos]='\0';
    //printf("%s\n%s\n\n", protocol, domain);
    if(url[pos]=='\0'){
        if(strcmp(protocol,"http")==0){
            strcpy(port,"80");
        }else if(strcmp(protocol,"https")==0){
            strcpy(port,"443");
        }
        return;
    }
    copy_pos=0;
    pos+=1;
    while(url[pos]!='\0'){
        port[copy_pos++]=url[pos++];
    }
    port[copy_pos]='\0';
    //printf("%s\n%s\n\n", protocol, domain);
}

int main() {
    char protocol[20];
    char domain[50];
    char port[10];
    parse_url("https://cn.bing.com", protocol, domain, port);
    if (strcmp(protocol, "https") != 0 || strcmp(domain, "cn.bing.com") != 0 || strcmp(port, "443") != 0) {
        fputs("test case 1 parse error", stderr);
        exit(1);
    }
    parse_url("http://cn.bing.com", protocol, domain, port);
    if (strcmp(protocol, "http") != 0 || strcmp(domain, "cn.bing.com") != 0 || strcmp(port, "80") != 0) {
        fputs("test case 2 parse error", stderr);
        exit(1);
    }
    parse_url("http://cn.bing.com:233", protocol, domain, port);
    if (strcmp(protocol, "http") != 0 || strcmp(domain, "cn.bing.com") != 0 || strcmp(port, "233") != 0) {
        fputs("test case 3 parse error", stderr);
        exit(1);
    }
    puts("success");
}