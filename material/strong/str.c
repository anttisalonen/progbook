#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
# define MAX_WORDS 20

    char *str = "this is a string.\n";
    char str_buf[256];
    memset(str_buf, 0x00, sizeof(str_buf));
    strncpy(str_buf, str, 255);
    int indices[MAX_WORDS];
    int next_index = 1;
    memset(indices, -1, sizeof(indices) / sizeof(int));
    indices[0] = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        if(str_buf[i] == '\0') {
            break;
        }
        if(str_buf[i] == ' ') {
            indices[next_index++] = i + 1;
            str_buf[i] = '\0';
            assert(next_index < MAX_WORDS);
        }
    }

    const int num_words = next_index;

    /* printing out each word */
    for(int i = 0; i < num_words; i++) {
        printf("%s\n", &str_buf[indices[i]]);
    }
}
