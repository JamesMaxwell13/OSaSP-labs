#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_FILENAME "buffer.bin"
#define SORTED_FILENAME "result.bin"
#define N 8192

typedef struct index_s {
    double time_mark;       
    uint64_t recno;         
} index_s;

typedef struct index_hdr_s {
    uint64_t records;       
    index_s idx[];          
} index_hdr_s;

void print_data(char *filename, long num)
{
    index_s index;
    index_hdr_s header;
    size_t read_size;
    long rec_count = 0;
    FILE *f = fopen(filename, "rb");

    if (num == 0 && (read_size = fread(&header, sizeof(index_hdr_s), 1, f)) > 0)
    {
        printf("\nAmount of records: %lu\n", header.records);
        printf("\n");
    }

    while ((read_size = fread(&index, sizeof(index_s), 1, f)) > 0)
    {
        printf("Record #%lu \tTimemark: %f\n", index.recno, index.time_mark);
        ++rec_count;
    }
    printf("\n");

    if(num == 1)
        printf("Sorted records: %lu\n", rec_count);

    fclose(f);
}


double generate_num();

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Generation error!\n");
        fprintf(stdout, "USAGE: ./generator <file size> <file name>\n");
        return EXIT_FAILURE;
    }

    int recs = (int) strtol(argv[1], NULL, 10);
    if (recs % 256 != 0)
    {
        fprintf(stderr, "File size must be a multiple of 256!\n");
        return EXIT_FAILURE;
    }

    index_hdr_s head;
    head.records = recs;
    char *filename = argv[2];
    FILE *f = fopen(filename, "wb");

    fwrite(&head, sizeof(index_hdr_s), 1, f);

    srandom(time(NULL));
    for (int i = 0; i < recs; ++i)
    {
        index_s record;
        record.time_mark = generate_num();
        record.recno = i + 1;
        fwrite(&record, sizeof(struct index_s), 1, f);
    }
    fclose(f);

    print_data(filename, 0);
    printf("Data was generated and saved into %s\n\n", filename);

    return 0;
}

double generate_num()
{
    long maxnum = 15020;
    time_t curtime = time(NULL);
    time_t yesterday = curtime / (24 * 60 * 60) - 1;
    long rand_int = maxnum + random() % (yesterday - maxnum + 1);

    double rand_double = (double) random() / ((double) RAND_MAX + 1 / 2.0f) + 0.5;

    double finalnum = (double) rand_int + rand_double;

    return finalnum;
}
