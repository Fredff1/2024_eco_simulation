#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

void parse_url(const char *url, char *protocol, char *domain, char *port) {
    int pos = 0, copy_pos = 0;

    // 读取协议
    while (url[pos] != ':') {
        protocol[copy_pos++] = url[pos++];
    }
    protocol[copy_pos] = '\0'; // 添加字符串结束符

    pos += 3; // 跳过 "://"
    copy_pos = 0;

    // 读取域名
    while (url[pos] != ':' && url[pos] != '\0') {
        domain[copy_pos++] = url[pos++];
    }
    domain[copy_pos] = '\0'; // 添加字符串结束符

    if (url[pos] == '\0') {
        port[0] = '\0'; // 设置端口为空字符串
        return;
    }

    pos++; // 跳过冒号
    copy_pos = 0;

    // 读取端口
    while (url[pos] != '\0') {
        port[copy_pos++] = url[pos++];
    }
    port[copy_pos] = '\0'; // 添加字符串结束符
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
