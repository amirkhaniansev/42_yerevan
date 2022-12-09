#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

typedef struct input
{
    int        start;
    int        end;
    long long* arr;
} input_t;

typedef struct output
{
    long long result;
} output_t;

void* execute(void* input)
{
    input_t* actual_input = input;
    if (actual_input == NULL)
        return NULL;

    output_t* output = malloc(sizeof(output_t));
    for (int i = actual_input->start; i < actual_input->end; i++)
    {
        output->result += actual_input->arr[i];
    }
    return output;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("%s\n", "Incorrect amount of arguments are provided!");
        return 1;
    }

    srand(time(NULL));

    int        count = atoi(argv[1]);
    long long* arr   = malloc(sizeof(long long) * count);
    for (int i = 0; i < count; i++)
    {
        arr[i] = rand() % 10;
    }

    int        thread_count = atoi(argv[2]);
    int        per_thread   = count / thread_count;
    pthread_t* threads      = malloc(sizeof(pthread_t) * thread_count);
    for (int i = 0; i < thread_count; i++)
    {
        input_t input;
        input.arr   = arr;
        input.start = i * per_thread;
        input.end   = input.start + per_thread;
        printf("Thread %d will calculate the range [%d:%d).\n", i, input.start, input.end);
        pthread_create(&threads[i], NULL, execute, &input);
    }

    long long sum = 0;
    for (int i = 0; i < thread_count; i++)
    {
        output_t* output;
        if (pthread_join(threads[i], (void**)(&output)) == 0)
        {
            printf("Thread %d ended with result %d.\n", i, output->result);
            sum += output->result;
        }
        free(output);
    }

    free(threads);
    free(arr);

    return 0;
}
