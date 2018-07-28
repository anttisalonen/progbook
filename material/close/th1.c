#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_text(void *d)
{
    char char_to_print = *(char *)d;
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 50; i++) {
            printf("%c", char_to_print);
        }
        printf("\n");
    }
    return NULL;
}

int main(void)
{
    pthread_t x_thread;
    pthread_t y_thread;
    int ret;
    void *res;

    char x_param = 'x';
    char y_param = 'y';

    ret = pthread_create(&x_thread, NULL, print_text, (void *)&x_param);
    if(ret) {
        fprintf(stderr, "Could not create thread x\n");
        exit(1);
    }
    ret = pthread_create(&y_thread, NULL, print_text, (void *)&y_param);
    if(ret) {
        fprintf(stderr, "Could not create thread y\n");
        exit(1);
    }

    ret = pthread_join(x_thread, &res);
    if(ret) {
        fprintf(stderr, "Could not join thread x\n");
        exit(1);
    }
    ret = pthread_join(y_thread, &res);
    if(ret) {
        fprintf(stderr, "Could not join thread y\n");
        exit(1);
    }

    return 0;
}
