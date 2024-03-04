#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *longest_str(const char *strs[], int cnt) {
    int pos = 0;
    int max = strlen(strs[0]);
    for (int i = 1; i < cnt; i++) {
        if (strlen(strs[i]) > max) {
            max = strlen(strs[i]);
            pos = i;
        }
    }
    return strs[pos];
}

int main() {
    const char *strings[] = {"Hello", "OpenAI", "Assistant", "GPT-3.5"};
    int count = sizeof(strings) / sizeof(strings[0]);
    const char *longest = longest_str(strings, count);
    printf("longest: %s\n", longest);
}