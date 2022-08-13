#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void current_dir(char* directory) {
    printf("%s\n", directory);
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

void ls_path(char* directory, char* tok2) {
    DIR *dir = opendir(directory);
    struct dirent *entry;
    
    if (tok2 != NULL && strncmp("-", tok2, 1) != 0) {
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
    // printf("bool a: %d\n", a);
    bool l = strchr(tok2,'l') != NULL;
    // printf("bool l: %d\n", l);
    bool r = strchr(tok2,'r') != NULL;
    // printf("bool r: %d\n", r);
    
    // DIR *dir = opendir(directory);
    // struct dirent *entry = readdir(dir);
    char* copy = (char*)malloc(500 * sizeof(char));

    if (strncmp("-", tok2, 1) == 0 && r == 0) {
        while ((entry = readdir(dir)) != NULL) {
            strcpy(copy, directory);
            strcat(copy, "/");
            strcat(copy, entry->d_name);
            if (a != 0) {
                if (l != 0) {
                    ls_l(entry, copy);
                } else {
                    printf("%s\n",entry->d_name);
                }
            } else if (l != 0) {
                if (strncmp(".", entry->d_name, 1) != 0) {
                    ls_l(entry, copy);
                }
            
            }
        }
    } else if (strncmp("-", tok2, 1) == 0 && r != 0) {
        int level = 0;
        printf("! direc: %s\n", directory);
        ls_r(directory, level, a, l);
    }
    
    
}

void ls(char* directory) {
    // // if (tok2 != NULL) {
    // //     change_dir(directory, tok2);
    // // } else 
    // if (tok2 == NULL) {
        DIR *dir = opendir(directory);
        struct dirent *entry = readdir(dir);
        printf("In ls dir: %s\n", directory);

        // char* copy = (char*)malloc(500 * sizeof(char));
        while (entry != NULL) {
            if (strncmp(".", entry->d_name, 1) != 0) {
                printf("%s\n", entry->d_name);
            }
            entry = readdir(dir);
        }
    // }
}

// void ls_a(struct dirent *entry) {
//     printf("%s\n", entry->d_name);
// }

void ls_l(struct dirent *entry, char* copy) {

    struct stat path_stat;
    stat(copy, &path_stat);
    int size = path_stat.st_size;
    int mode = path_stat.st_mode;
    // printf("\nsize: %d mode: %o\n", size, mode);

    // bit manipulation
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

void print_spaces(int level) {
    for (int i = 0; i < 4 * level; i++) {
        printf(" ");
    }
}

void ls_r(char* directory, int level, bool a, bool l) {
    char* copy = (char*)malloc(500 * sizeof(char));
    DIR *dir = opendir(directory);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        strcpy(copy, directory);
        strcat(copy, "/");
        strcat(copy, entry->d_name);

        // printf("inside ls_r copy: %s\n", copy);
        struct stat path_stat;
        stat(copy, &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;

        // printf("size: %d mode: %o\n", size, mode);
        // printf("check mode: %d\n", S_ISDIR(mode));

        if (a != 0 && l == 0) {
            if (S_ISDIR(mode) == 0 || strncmp(".", entry->d_name, 1) == 0) {
                print_spaces(level);
                printf("%s\n", entry->d_name);
            } else {
                // if (strncmp(".", entry->d_name, 1) != 0) {
                print_spaces(level);
                printf("%s\n", entry->d_name);
                ls_r(copy, level+1, a, l);
            }
        } else if (a == 0 && l != 0) {
            if (S_ISDIR(mode) == 0) {
                print_spaces(level);
                ls_l(entry, copy);
            } else if (strncmp(".", entry->d_name, 1) != 0) {
                print_spaces(level);
                ls_l(entry, copy);
                ls_r(copy, level+1, a, l);
            }
        } else if (a != 0 && l != 0) {
            if (S_ISDIR(mode) == 0 || strncmp(".", entry->d_name, 1) == 0) {
                print_spaces(level);
                ls_l(entry, copy);
            } else {
                print_spaces(level);
                ls_l(entry, copy);
                ls_r(copy, level+1, a, l);
            }
        } else {
            if (S_ISDIR(mode) == 0) {
                print_spaces(level);
                printf("%s\n", entry->d_name);
            } else if (strncmp(".", entry->d_name, 1) != 0) {
                print_spaces(level);
                printf("%s\n", entry->d_name);
                ls_r(copy, level+1, a, l);
            }
        }
    }
}

int main() {
    // print the current directory
    char* directory = (char*)malloc(500 * sizeof(char));
    char* copy = (char*)malloc(500 * sizeof(char));
    getcwd(directory, 499);
    printf("\nCurrent working dir: %s\n", directory);

    // prompt the user to input a command
    char input[200];
    while (strcmp(input, "logout") != 0) {
        printf("$ "); 
        gets(input);

        char* tok1 = strtok(input, " ");
        char* tok2 = strtok(NULL, " ");
        printf("command: %s\n", tok1); 
        printf("path: %s\n\n", tok2);
        
        if (strcmp("pwd", tok1) == 0) {
            printf("pwd!!!: ");
            current_dir(directory);

        } else if (strcmp("cd", tok1) == 0 && tok2 != NULL) { // cd
            change_dir(directory, tok2);
        } else if (strcmp("ls", tok1) == 0) {
            // ls_path(directory, tok2);
            if (tok2 == NULL) {
                printf("null!\n");
                ls(directory);
                printf("null finished\n");
            } else {
                ls_path(directory, tok2);
            }
            // } else if (strcmp("-a", tok2) == 0) {
            //     ls_a(directory, copy);
            // } else if (strcmp("-l", tok2) == 0) {
            //     ls_l(directory, copy);

            // if (strcmp("-r", tok2) == 0) {
            //     printf("ls -r!\n");
            //     int level = 0;
            //     ls_r(directory, level);
            // }

            // } else {
            //     ls(directory, tok2);
            // }
        }
        
    }
}