#define _GNU_SOURCE

#include "header.h"

bool flag = false;

RingBuffer *buf;

// функция для корректного завершения потоков
void cleanup()
{
    pthread_mutex_unlock(&mutex_consumer);
    pthread_mutex_unlock(&mutex_producer);
}

void *consumer_fun()
{
    struct message ms;
    while (!flag) {
        sleep(1);
        if (buf->is_empty == false) {
            pthread_mutex_lock(&mutex_consumer);
            bool ans = dequeue(buf, &ms);
            if (!(ms.hash && 11))
                printf("Message is corrupted\n");
            pthread_mutex_unlock(&mutex_consumer);
            if (ans) {
                print_message(ms);
                printf("GETS: %d\n", buf->count_errase);
            }
        }
        pthread_testcancel();
    }
    return NULL;
}

void *producer_fun()
{
    struct message ms;
    pthread_mutex_lock(&mutex_producer);
    while (!flag) {
        sleep(1);
        if (buf->is_empty == true) {
            ms = init_mes();
            bool ans = enqueue(buf, ms);
            if (!(ms.hash && 11))
                printf("Message is corrupted\n");
            if (ans)
                printf("SENDS: %d\n", buf->count_add);
            pthread_mutex_unlock(&mutex_producer);
        }
        pthread_testcancel();
    }
    pthread_mutex_unlock(&mutex_producer);

    return NULL;
}

int main()
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);       // установка флагов отмены

    pthread_cleanup_push(cleanup, NULL)           // установка обработчика отмены

    pthread_t *consumers = NULL;
    pthread_t *producers = NULL;
    int count_consumers = 0;
    int count_producers = 0;
    char option[5];

    buf = init(10);
    printf("[+] - create producer\n"
           "[-] - delete producer\n"
           "[a] - create consumer\n"
           "[d] - delete consumer\n"
           "[*] - increase buffer\n"
           "[/] - decrease buffer\n"
           "[l] - show threads\n"
           "[q] - quit\n");
    while (1) {
        strcpy(option, "\0");
        fgets(option, sizeof(option), stdin);
        if (strcmp(option, "+\n") == 0) {      //создать producer
            producers = create_thread(producers, &count_producers, producer_fun);
        } else if (strcmp(option, "-\n") == 0) {      //завершить producer
            if (count_producers) {
                pthread_cancel(producers[count_producers - 1]); // запрос отмены выполнения потока
                pthread_join(producers[count_producers - 1], NULL); //ожидание завершения потока
                count_producers--;
                producers = realloc(producers, count_producers * sizeof(pthread_t));
            }
        } else if (strcmp(option, "a\n") == 0) {         //создать consumer
            consumers = create_thread(consumers, &count_consumers, consumer_fun);
        } else if (strcmp(option, "d\n") == 0) {        //завершить consumer
            if (count_consumers) {
                pthread_cancel(consumers[count_consumers - 1]);   // запрос отмены выполнения потока
                pthread_join(consumers[count_consumers - 1], NULL);   //ожидание завершения потока
                count_consumers--;
                consumers = realloc(consumers, count_consumers * sizeof(pthread_t));
            }
        } else if (strcmp(option, "*\n") == 0) {      //увеличить размер буфера
            set_new_size(buf, (buf->size) + 1);
            printf("NEW SIZE: %d\n", buf->size - 1);
        } else if (strcmp(option, "/\n") == 0) {      //увеньшить размер
            set_new_size(buf, (buf->size) - 1);
            printf("NEW SIZE: %d\n", buf->size - 1);
        } else if (strcmp(option, "l\n") == 0) {
            printf("PRODUCERS: ");
            show_thread(producers, count_producers);
            printf("CONSUMERS: ");
            show_thread(consumers, count_consumers);
        } else if (strcmp(option, "q\n") == 0) {       //выход из программы
            flag = true;
            for (int i = 0; i < count_consumers; i++) {
                pthread_cancel(consumers[i]);
                pthread_join(consumers[i], NULL);   //ожидание конца
            }
            for (int i = 0; i < count_producers; i++) {
                pthread_cancel(producers[i]);
                pthread_join(producers[i], NULL);
            }
            free(consumers);
            free(producers);
            break;
        }
    }

    pthread_cleanup_pop(0);     // удаление обработчика отмены

    pthread_mutex_destroy(&mutex_consumer);
    pthread_mutex_destroy(&mutex_producer);


    destroy(buf);
    return 0;
}

