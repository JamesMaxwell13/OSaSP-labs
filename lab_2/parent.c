// Разработать две программы ‒ parent и child.
// Перед запуском программы parent в окружении создается переменная среды CHILD_PATH с
// именем каталога, где находится программа child.
// Родительский процесс (программа parent) после запуска получает переменные среды, сортирует их в LC_COLLATE=C и выводит в stdout. После этого входит в цикл обработки нажатий клавиатуры.
// Символ «+», используя fork(2) и execve(2) порождает дочерний процесс и запускает в нем
// очередной экземпляр программы child. Информацию о каталоге, где размещается child, получает из окружения, используя функцию getenv(). Имя программы (argv[0]) устанавливается как
// child_XX, где XX порядковый номер от 00 до 99. Номер инкрементируется родителем. ‒
// Символ «*» порождает дочерний процесс аналогично предыдущему случаю, однако информацию о расположении программы child получает, сканируя массив параметров среды,
// переданный в третьем параметре функции main().
// Символ «&» порождает дочерний процесс аналогично предыдущему случаю, однако информацию о расположении программы child получает, сканируя массив параметров среды, 
// указанный во внешней переменной extern char **environ, установленной хост-средой при запуске
// (см. IEEE Std 1003.1-2017).
// При запуске дочернего процесса ему передается сокращенное окружение, включающее набор переменных, указанных в файле, который передается родительскому процессу как параметр
// командной строки. Минимальный набор переменных должен включать SHELL, HOME, HOSTNAME,
// LOGNAME, LANG, TERM, USER, LC_COLLATE, PATH. Дочерний процесс открывает этот файл, считывает имена переменных, получает из окружения их значение и выводит в stdout.
// Дочерний процесс (программа child) выводит свое имя, pid, ppid, открывает файл с набором переменных, считывает их имена, получает из окружения, переданного ему при запуске, их
// значение способом, указанным при обработке нажатий, выводит в stdout и завершается.
// Символ «q» завершает выполнение родительского процесса.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <unistd.h>

extern char** environ;
const char child_path[] = "CHILD_PATH";

static int compare(const void* str1, const void* str2) {
    return strcmp(*(const char**) str1, *(const char**) str2);
}

void env_print(char *envp[]) {
    int env_size = 0;
    while(envp[env_size]) {
        env_size++;
    }

    qsort(envp, env_size, sizeof(char*), compare);

    printf("\nParent enviroment variables:\n");
    for(int i = 0; i < env_size; i++) {
        printf("%s\n", envp[i]);
    }
    printf("\n");
}

char* get_child_path(char** str) {
    while(*str) {
        if(strstr(*str, child_path))
            return *str + strlen(child_path) + 1;
        str++;
    }
    return NULL;
}

int main(int argc, char *argv[], char *envp[]) {
    env_print(envp);
    int child_count = 0;
    int command;

    while((command = getchar()) != EOF) {
        if(command == 'q') {
            exit(0);
        }

        char* child_process = NULL;
        switch((char)command) {
            case '+':
                child_process = getenv(child_path);
                break;
            case '*':
                child_process = get_child_path(envp);
                break;
            case '&':
                child_process = get_child_path(environ);
                break;
            default: continue;
        }

        //child_XX
        char child_name[9];
        strncpy(child_name, "child_", 7);
        child_name[6] = child_count / 10 + '0';
        child_name[7] = child_count % 10 + '0';
        child_count++;
        char* const args[] = {child_name, argv[1]};
        pid_t pid = fork();
        if(pid > 0) {
            int status;
            wait(&status);
        }
        else 
            if(pid == 0) {
                if(execve(child_process, args, envp) == -1) {
                    printf("Execve error\n");
                    exit(1);
                }
                else {
                    printf("Fork error\n");
                    exit(1);
                }
        }
    }
    return 0;
}