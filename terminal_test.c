#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void current_dir(char* directory) {
    getcwd(directory, 499);
}

void change_dir(char* directory, char* tok2) {
    char* path1 = &tok2[1];
    // absolute path
    if (strncmp("/", tok2, 1) == 0) {
        printf("absolute /\n");
        directory = tok2;
        printf("Current working dir: %s\n", directory);
    } else if (strncmp(":\\", path1, 2) == 0) {
        printf("absolute :\\\n");
        directory = tok2;
        printf("Current working dir: %s\n", directory);
    } else { // relative path
        strcat(directory, "/");
        strcat(directory, tok2);
        printf("Current working dir: %s\n", directory);
    }
}

void print_stats(struct dirent *entry, struct stat path_stat) {
    int size = path_stat.st_size;
    int mode = path_stat.st_mode;
    // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
    printf("\nsize: %d mode: %o\n", size, mode);

    for (int i = 8; i >= 0; i--) {
        int temp = (1 << i) & mode;
        if (temp != 0 && ((i+1) % 3 == 0)) {
            printf("r");
        } else if (temp != 0 && ((i+2) % 3 == 0)) {
            printf("w");
        } else if (temp != 0 && ((i+3) % 3 == 0)) {
            printf("x");
        } else {
            printf("-");
        }
    }
    printf(" %d %s", size, entry->d_name);
    printf("\n");
}

void ls(char* directory, char* tok2) {
    DIR *dir = opendir(directory);
    char* copy = (char*)malloc(500 * sizeof(char));
    struct dirent *entry;
    struct stat path_stat;

    strcpy(copy, directory);
    strcat(copy, "/");
    strcat(copy, entry->d_name);
    stat(copy, &path_stat);


    // ls with path
    if (tok2 != NULL) {
        char* copy = (char*)malloc(500 * sizeof(char));
        change_dir(directory, tok2);
        strcpy(copy, directory);
        dir = opendir(copy);
        printf("In ls dir with path: %s\n", directory);

        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(".", entry->d_name, 1) != 0) {
                printf("%s\n", entry->d_name);
            }
        }
    }
    bool a = (strchr(tok2,'a') != NULL);
    // printf("tok2: %s\n", tok2);
    printf("bool a: %d\n", a);
    bool l = strchr(tok2,'l') != NULL;
    bool r = strchr(tok2,'r');

    // printf("inside: %s\n", entry->d_name);
    while((entry = readdir(dir)) != NULL) {
        printf("inside: %s\n", entry->d_name);
        if (a != 0) {
            printf("a!\n");
            if (l != 0) {
                print_stats(entry, path_stat);
            } else {
                printf("%s\n",entry->d_name);
            }
        } else if (l != 0) {
            printf("l!\n");
        } else if (r != 0) {
            printf("r!\n");
        }
    }
    
}

void ls_null(char* directory) {
        DIR *dir = opendir(directory);
        struct dirent *entry = readdir(dir);
        printf("In ls dir: %s\n", directory);

        // char* copy = (char*)malloc(499 * sizeof(char));
        while (entry != NULL) {
            // strcpy(copy, directory);
            // strcat(copy, "/");
            // strcat(copy, entry->d_name);

            // printf("copy: %s\n", copy);
            // ls with no options
            if (strncmp(".", entry->d_name, 1) != 0) {
                printf("%s\n", entry->d_name);
            }
            entry = readdir(dir);
        }
}

void ls_a(char* directory) {
    DIR *dir = opendir(directory);
    struct dirent *entry = readdir(dir);
    printf("In ls -a dir: %s\n", directory);

    char* copy = (char*)malloc(499 * sizeof(char));
    while (entry != NULL) {
        strcpy(copy, directory);
        strcat(copy, "/");
        strcat(copy, entry->d_name);

        // printf("copy: %s\n", copy);
        printf("%s\n", entry->d_name);
        entry = readdir(dir);
    }
}

void ls_l(char* directory) {
    DIR *dir = opendir(directory);
    struct dirent *entry = readdir(dir);

    char* copy = (char*)malloc(499 * sizeof(char));
    while (entry != NULL) {
        strcpy(copy, directory);
        strcat(copy, "/");
        strcat(copy, entry->d_name);

        // printf("copy: %s\n", copy);
        if (strncmp(".", entry->d_name, 1) != 0) {
            printf("%s\n", entry->d_name);
        }

        struct stat path_stat;
        stat(copy, &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;
        // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
        printf("\nsize: %d mode: %o\n", size, mode);

        for (int i = 8; i >= 0; i--) {
            int temp = (1 << i) & mode;
            if (temp != 0 && ((i+1) % 3 == 0)) {
                printf("r");
            } else if (temp != 0 && ((i+2) % 3 == 0)) {
                printf("w");
            } else if (temp != 0 && ((i+3) % 3 == 0)) {
                printf("x");
            } else {
                printf("-");
            }
        }
        printf(" %d %s", size, entry->d_name);
        printf("\n");

        entry = readdir(dir);
    }
}

void ls_r(char* directory, int level) {
    char* copy = (char*)malloc(500 * sizeof(char));

    DIR *dir = opendir(directory);
    struct dirent *entry = readdir(dir);
    // strcpy(copy, directory);

    while (entry != NULL) {
        strcpy(copy, directory);
        strcat(copy, "/");
        strcat(copy, entry->d_name);

        printf("copy: %s\n", copy);
        struct stat path_stat;
        stat(copy, &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;

        // printf("size: %d mode: %o\n", size, mode);
        // printf("check mode: %d\n", S_ISDIR(mode));

        if (S_ISDIR(mode) == 0 || strncmp(".", entry->d_name, 1) == 0) {
            for (int i = 0; i < 4 * level; i++) {
                printf(" ");
            }
            printf("%s\n", entry->d_name);
        
        } else {
            // if (strncmp(".", entry->d_name, 1) != 0) {
                for (int i = 0; i < 4 * level; i++) {
                    printf(" ");
                }
                printf("%s\n", entry->d_name);
                ls_r(copy, level+1);
            // }
        }
        entry = readdir(dir);
    }
}

int main() {
    // printf the current directory
    char* directory = (char*)malloc(500 * sizeof(char));
    current_dir(directory);
    printf("\nCurrent working dir: %s\n", directory);

    // prompt the user to input a command
    char input[500];
    while (strcmp(input, "logout") != 0) {
        printf("$ "); 
        gets(input);

        char* tok1 = strtok(input, " ");
        char* tok2 = strtok(NULL, " ");
        printf("command: %s\n", tok1); 
        printf("path: %s\n\n", tok2);
        
        if (strcmp("pwd", tok1) == 0) {
            printf("%s\n", directory);

        } else if (strcmp("cd", tok1) == 0 && tok2 != NULL) { // cd
            change_dir(directory, tok2);

        } else if (strcmp("ls", tok1) == 0) {
            
            if (tok2 == NULL) {
                ls_null(directory);
            } 
            ls(directory, tok2);

            // else if (strcmp("-a", tok2) == 0) {
            //     ls_a(directory);
            // } else if (strcmp("-l", tok2) == 0) {
            //     ls_l(directory);
            // } else if (strcmp("-r", tok2) == 0) {
            //     int level = 0;
            //     ls_r(directory, level);
            // }

            // } else if (tok2 == NULL) {
            //     printf("null!\n");
            //     ls_null(directory);
            // }
            //  else if (tok2 != NULL && strncmp("-", tok2, 1) != 0) {
            //     printf("!!!\n");
            //     ls(directory, tok2);
            //     printf("null finished\n");
            // }
        }
        // } else if (strcmp("ls", tok1) == 0 && tok2 == NULL) {
        //         printf("null !!!\n");
        //         ls_null(directory);
        //     }
        

        // printf("print the original directory: \n");

        // char* copy = (char*)malloc(500 * sizeof(char));
        // DIR *dir = opendir(directory);
        // struct dirent *entry = readdir(dir);
        // while (entry != NULL) {

        //     strcpy(copy, directory);
        //     strcat(copy, "/");
        //     strcat(copy, entry->d_name);

        //     printf("copy: %s\n", copy);
        //     struct stat path_stat;
        //     stat(copy, &path_stat);
        //     int size = path_stat.st_size;
        //     int mode = path_stat.st_mode;
        //     // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
        //     printf("size: %d mode: %o\n", size, mode);
        //     printf("check mode: %d\n", S_ISDIR(mode));

        //     for (int i = 8; i >= 0; i--) {
        //         int temp = (1 << i) & mode;
        //         if (temp != 0 && ((i+1) % 3 == 0)) {
        //             printf("r");
        //         } else if (temp != 0 && ((i+2) % 3 == 0)) {
        //             printf("w");
        //         } else if (temp != 0 && ((i+3) % 3 == 0)) {
        //             printf("x");
        //         } else {
        //             printf("-");
        //         }
        //     }
        //     printf(" %d %s", size, entry->d_name);
        //     printf("\n");
        //     entry = readdir(dir);
        // }

        // with path (changed)
        // printf("\nprint the path the user entered (changed): \n");
        // DIR *dir2 = opendir(directory);
        // struct dirent *entry2 = readdir(dir2);

        // char* copy = (char*)malloc(499 * sizeof(char));
        // while (entry2 != NULL) {
        //     strcpy(copy, directory);
        //     strcat(copy, "/");
        //     strcat(copy, entry2->d_name);

        //     // strcat(directory, entry->d_name);
        //     printf("copy: %s\n", copy);
        //     // ls with no options
        //     if (strncmp(".", entry2->d_name, 1) != 0) {
        //         printf("%s\n", entry2->d_name);
        //     }

            // struct stat path_stat;
            // stat(copy, &path_stat);
            // int size = path_stat.st_size;
            // int mode = path_stat.st_mode;
            // // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
            // printf("\nsize: %d mode: %o\n", size, mode);

            // entry2 = readdir(dir2);
        // }

        // // bit manipulation
        
        struct stat path_stat;
        stat("C:\\Users\\SylviaYang\\Downloads\\Edmonds\\EdCC CS 131 Fall 2020/hw1Song", &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;

        printf("\nsize: %d mode: %o\n", size, mode);

        // for (int i = 8; i >= 0; i--) {
        //     int temp = (1 << i) & mode;
        //     printf("%d", temp);
        //     if (temp != 0 && ((i+1) % 3 == 0)) {
        //         printf("r");
        //     } else if (temp != 0 && ((i+2) % 3 == 0)) {
        //         printf("w");
        //     } else if (temp != 0 && ((i+3) % 3 == 0)) {
        //         printf("x");
        //     } else {
        //         printf("-");
        //     }
        //     printf("\n");
        // }
    }
}