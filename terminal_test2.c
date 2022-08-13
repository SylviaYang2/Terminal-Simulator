#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>

void r(char* directory, int level) {
    // printf("print the original directory: \n");
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
        printf("check mode: %d\n", S_ISDIR(mode));

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
                r(copy, level+1);
            // }
        }
        entry = readdir(dir);
    }
}

int main() {
    // printf the current directory
    char* directory = (char*)malloc(500 * sizeof(char));
    getcwd(directory, 499);
    printf("\nCurrent working dir: %s", directory);

    // prompt the user to input a command
    char input[200];
    while (strcmp(input, "logout") != 0) {
        printf("\n$ "); 
        gets(input);
        printf("input: %s\n", input);

        char* command = strtok(input, " ");
        char* path = strtok(NULL, " ");
        char* path1 = &path[1];
        printf("command: %s\n", command); 
        printf("path: %s\n\n", path);
        
        // cd
        if (strcmp("cd", command) == 0 && path != NULL) {
            // absolute path
            if (strncmp("/", path, 1) == 0) {
                printf("absolute /");
                directory = path;
                printf("Current working dir: %s\n", directory);
            } else if (strncmp(":\\", path1, 2) == 0) {
                printf("absolute :\\\n");
                directory = path;
                printf("Current working dir: %s\n", directory);
            } else { // relative path
                strcat(directory, "/");
                strcat(directory, path);
                printf("Current working dir: %s\n", directory);
            }
        }
        int level = 0;
        r(directory, level);
    }

}