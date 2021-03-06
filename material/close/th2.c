#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_param {
    int num_increments;
    int *variable_to_increment;
};

void *increment(void *d)
{
    struct thread_param *param = (struct thread_param *)d;
    for(int j = 0; j < param->num_increments; j++) {
        (*param->variable_to_increment)++;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <num increments 1> <num increments 2>\n",
                argv[0]);
        exit(1);
    }
    pthread_t thread1;
    pthread_t thread2;

    int our_value = 0;
    int ret;
    void *res;

    struct thread_param param1;
    struct thread_param param2;

    param1.num_increments = atoi(argv[1]);
    param2.num_increments = atoi(argv[2]);

    param1.variable_to_increment = &our_value;
    param2.variable_to_increment = &our_value;

    ret = pthread_create(&thread1, NULL, increment, (void *)&param1);
    if(ret) {
        fprintf(stderr, "Could not create thread 1\n");
        exit(1);
    }
    ret = pthread_create(&thread2, NULL, increment, (void *)&param2);
    if(ret) {
        fprintf(stderr, "Could not create thread 2\n");
        exit(1);
    }

    ret = pthread_join(thread1, &res);
    if(ret) {
        fprintf(stderr, "Could not join thread 1\n");
        exit(1);
    }
    ret = pthread_join(thread2, &res);
    if(ret) {
        fprintf(stderr, "Could not join thread 2\n");
        exit(1);
    }

    printf("Final value: %d\n", our_value);
    return 0;
}
