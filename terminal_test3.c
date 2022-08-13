#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>

void current_dir(char* directory) {
    printf("%s\n", directory);
}

void change_dir(char* directory, char* tok2) {
    char* path1 = &tok2[1];
    // absolute path
    if (strncmp("/", tok2, 1) == 0) {
        printf("absolute /");
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

int main() {
    // printf the current directory
    char* directory = (char*)malloc(500 * sizeof(char));
    getcwd(directory, 499);
    printf("\nCurrent working dir: %s\n", directory);

    // prompt the user to input a command
    char input[200];
    while (strcmp(input, "logout") != 0) {
        printf("$ "); 
        gets(input);
        // printf("input: %s\n", input);
        char* tok1 = strtok(input, " ");
        char* tok2 = strtok(NULL, " ");
        // char* path1 = &path[1];
        printf("command: %s\n", tok1); 
        printf("path: %s\n\n", tok2);
        
        if (strcmp("pwd", tok1) == 0) {
            printf("pwd!!!: ");
            current_dir(directory);

        } else if (strcmp("cd", tok1) == 0 && tok2 != NULL) { // cd
            char* path1 = &tok2[1];
            // absolute path
            if (strncmp("/", tok2, 1) == 0) {
                printf("absolute /");
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
        // printf("%d", strstr("user/cs131", "cs131/slides"));

        printf("print the original directory: \n");
        DIR *dir = opendir(".");
        struct dirent *entry = readdir(dir);
        while (entry != NULL) {

            struct stat path_stat;
            stat(entry->d_name, &path_stat);
            int size = path_stat.st_size;
            int mode = path_stat.st_mode;
            // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
            printf("size: %d mode: %o\n", size, mode);
            printf("check mode: %d\n", S_ISDIR(mode));

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

        // with path (changed)
        printf("\nprint the path the user entered (changed): \n");
        DIR *dir2 = opendir(directory);
        struct dirent *entry2 = readdir(dir2);

        char* copy = (char*)malloc(499 * sizeof(char));
        while (entry2 != NULL) {
            strcpy(copy, directory);
            strcat(copy, "/");
            strcat(copy, entry2->d_name);

            // strcat(directory, entry->d_name);
            printf("copy: %s\n", copy);
            // ls with no options
            if (strncmp(".", entry2->d_name, 1) != 0) {
                printf("%s\n", entry2->d_name);
            }

            struct stat path_stat;
            stat(copy, &path_stat);
            int size = path_stat.st_size;
            int mode = path_stat.st_mode;
            // printf("\nsize: %d mode: %o\n", path_stat.st_size, path_stat.st_mode);
            printf("\nsize: %d mode: %o\n", size, mode);

            entry2 = readdir(dir2);
        }

        // // bit manipulation
        struct stat path_stat;
        stat("C:\\Users\\SylviaYang\\Downloads\\Edmonds\\EdCC CS 131 Fall 2020/hw1Song/song.c", &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;


        // chdir("C:\\Users\\SylviaYang\\Downloads\\Edmonds");
        // printf("\nCurrent working dir: %s", getcwd(directory, 499));

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