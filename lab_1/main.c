#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>


typedef struct cmd_opt{
    bool symlink;
    bool dir;
    bool file;
    bool sort;
    bool empty;
} options; 

typedef struct filesys_record{ 
    char path[PATH_MAX + 1];
    int type;
} record;

int compare_strings(const void* item1, const void* item2) {
    const record *item_1 = (const record *)item1;
    const record *item_2 = (const record *)item2;
    return strcoll((const char *)item_1->path, (const char *)item_2->path);
}

record* get_record(const char* path, int type) {
    record* item = (record*)malloc(sizeof(record));
    strcpy(item->path, path);
    item->type = type;
    return item;
}

void print_record(record item) {
    switch(item.type) {
        case 0: printf("[link]"); break;
        case 1: printf("[dir ]"); break;
        case 2: printf("[file]"); break;
        default: break;
    }
    printf(" %s\n", item.path);
}

void directory_walk(char *directory, options opt) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char path_name[PATH_MAX + 1];
    dir = opendir(directory);

    if( dir == NULL ) {
    	printf( "Error opening %s: %s", directory, strerror(errno));
    	return;
    }

    int size_out = 1;
    record* rec_list = (record*)malloc(size_out*sizeof(record));
    int i = 0;

    entry = readdir(dir);
	while(entry != NULL) {
    	if((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
        	entry = readdir(dir);
        	continue;
    	}

	    strncpy(path_name, directory, PATH_MAX);
        strncat(path_name, "/", PATH_MAX);
        strncat(path_name, entry->d_name, PATH_MAX);

        struct stat entry_info;
        if(lstat(path_name, &entry_info) == 0) {  
            if(S_ISDIR(entry_info.st_mode)) {
			    if(opt.dir) {
                    rec_list[i] = *get_record(path_name, 1);
                    i++;
                }
                directory_walk(path_name, opt);
            } 
            else 
                if(S_ISREG(entry_info.st_mode) && (opt.file)) {
                    rec_list[i] = *get_record(path_name, 2);
                    i++;
                } 
                else 
                    if(S_ISLNK(entry_info.st_mode) && opt.symlink) {
                        char target_name[PATH_MAX + 1];
                        if(readlink(path_name, target_name, PATH_MAX) != -1) {
                            rec_list[i] = *get_record(path_name, 0);
                            i++;
                        } 
                        else {
                            printf("%s -> (invalid symbolic link!)\n", path_name);
                            continue;
                        }
                }   
        } else {
            printf("Error starting %s: %s\n", path_name, strerror(errno));
            continue;
        }
        if(i == size_out) {        
            size_out++;
            rec_list = (record*)realloc(rec_list, size_out*sizeof(record));
        }
        entry = readdir(dir);
	}

    size_out--;
    rec_list = (record*)realloc(rec_list, size_out*sizeof(record));

    if(opt.sort == 1) {
        qsort(rec_list, size_out, sizeof(record), compare_strings);
    }

    for(i = 0; i < size_out; i++) {
        print_record(rec_list[i]);
    }
    free(rec_list);

	closedir(dir);
}

options parse_options(int argc, char *argv[], options opt) {
    int flags;
    while((flags = getopt(argc, argv, "ldfs?")) != -1) {
        switch(flags) {
            case 'l': opt.symlink = 1; break;
            case 'd': opt.dir = 1; break;
            case 'f': opt.file = 1; break;
            case 's': opt.sort = 1; break;
            case '?': 
                printf("Usage: -d -- directories, -f -- files, -l -- links, -s -- sort\n");
                exit(EXIT_SUCCESS);
            default: continue;
        }
    } 
    if(!opt.symlink && !opt.dir && !opt.file) {
        opt.symlink = 1;
        opt.dir = 1;
        opt.file = 1;
        opt.empty = 1;
    }
    return opt;
}

int main(int argc, char *argv[]) {
    char directory[PATH_MAX];
    options opt = {0, 0, 0, 0, 0};
    opt = parse_options(argc, argv, opt);
    switch(argc) {
        case 1: {
            strcpy(directory, ".");
            break;
        }
        case 2: {
            if(!opt.empty) {
                strcpy(directory, ".");
            }
            else {
                strncpy(directory, argv[1], PATH_MAX);
            }
            break;
        }
        case 3: {
            strncpy(directory, argv[2], PATH_MAX);
            break;
        }
        default: return 1;
    }
    directory_walk(directory, opt);
    return 0;
}

// -l – только символические ссылки (-type l)
// -d – только каталоги (-type d)
// -f -- только файлы (-type f)
// -s — сортировать выход в соответствии с LC_COLLATE
// Опции могут быть указаны как перед каталогом, так и после.
// Опции могут быть указаны как раздельно, так и вместе (-l -d, -ld ).
// Если опции ldf опущены, выводятся каталоги, файлы и ссылки.
