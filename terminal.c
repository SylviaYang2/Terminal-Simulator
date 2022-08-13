// Mengmeng Yang, CS 131, Autumn 2020, Section 2084
// Programming Assignment #8, 12/4/2020
//
// This program's behavior is to produce a cumulative song
// in which successive verses build on previous verses. This
// assignment tests our understanding of functions and printf statements.
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// prints out the current directory
void current_dir(char* directory) {
    printf("%s\n", directory);
}

// sets the current directory to the path the user entered
void change_dir(char* directory, char* tok2) {
    char* path1 = &tok2[1];
    // absolute path
    if (strncmp("/", tok2, 1) == 0) {
        directory = tok2;
    } else if (strncmp(":\\", path1, 2) == 0) {
        directory = tok2;
    } else { // relative path
        strcat(directory, "/");
        strcat(directory, tok2);
    }
}

// list the files
// "ls" with an absolute path or a relative path, or any option ot any combination of options
void ls_path(char* directory, char* tok2) {
    char* copy = (char*)malloc(500 * sizeof(char));
    DIR *dir = opendir(directory);
    struct dirent *entry;
    
    // with relative path or absolute path
    if (tok2 != NULL && strncmp("-", tok2, 1) != 0) {
        char* copy = (char*)malloc(500 * sizeof(char));
        change_dir(directory, tok2);
        strcpy(copy, directory);
        dir = opendir(copy);

        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(".", entry->d_name, 1) != 0) {
                printf("%s\n", entry->d_name);
            }
        }
    }

    // to check if the user's option contains a, l, r, or any combinations
    bool a = (strchr(tok2,'a') != NULL);
    bool l = strchr(tok2,'l') != NULL;
    bool r = strchr(tok2,'r') != NULL;

    // if has options starting with "-" and not equal to "r"
    if (strncmp("-", tok2, 1) == 0 && r == 0) {
        while ((entry = readdir(dir)) != NULL) {
            strcpy(copy, directory);
            strcat(copy, "/");
            strcat(copy, entry->d_name);
            if (a != 0) { 
                if (l != 0) { // if the option is "al" or "la" 
                    ls_l(entry, copy);
                } else { // if the option is "a" 
                    printf("%s\n",entry->d_name);
                }
            } else if (l != 0) { // if the option is "l" 
                if (strncmp(".", entry->d_name, 1) != 0) {
                    ls_l(entry, copy);
                }
            }
        }
    } else if (strncmp("-", tok2, 1) == 0 && r != 0) { // if has options starting with "-" and equal to "r"
        int level = 0;
        ls_r(directory, level, a, l);
    }
}

// list the files when called with no path or options
void ls_null(char* directory) {
        DIR *dir = opendir(directory);
        struct dirent *entry = readdir(dir);
        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(".", entry->d_name, 1) != 0) {
                printf("%s\n", entry->d_name);
            }
        }
}

// list files with all extra information including the file permissions, file size and the file name
void ls_l(struct dirent *entry, char* copy) {
    struct stat path_stat;
    stat(copy, &path_stat);
    int size = path_stat.st_size;
    int mode = path_stat.st_mode;

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

// print spaces based on the indent level
void print_spaces(int level) {
    for (int i = 0; i < 4 * level; i++) {
        printf(" ");
    }
}

// list files recursively, list all files and folders in the directory
void ls_r(char* directory, int level, bool a, bool l) {
    char* copy = (char*)malloc(500 * sizeof(char));
    DIR *dir = opendir(directory);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        strcpy(copy, directory);
        strcat(copy, "/");
        strcat(copy, entry->d_name);

        struct stat path_stat;
        stat(copy, &path_stat);
        int size = path_stat.st_size;
        int mode = path_stat.st_mode;

        if (a != 0 && l == 0) { // option: "ar" or "ra"
            if (S_ISDIR(mode) == 0 || strncmp(".", entry->d_name, 1) == 0) {
                print_spaces(level);
                printf("%s\n", entry->d_name);
            } else {
                print_spaces(level);
                printf("%s\n", entry->d_name);
                ls_r(copy, level+1, a, l);
            }
        } else if (a == 0 && l != 0) { // option: "lr" or "rl"
            if (S_ISDIR(mode) == 0) {
                print_spaces(level);
                ls_l(entry, copy);
            } else if (strncmp(".", entry->d_name, 1) != 0) {
                print_spaces(level);
                ls_l(entry, copy);
                ls_r(copy, level+1, a, l);
            }
        } else if (a != 0 && l != 0) { // option: "arl" or "alr" or "ral" or "rla" or "lar" or "lra"
            if (S_ISDIR(mode) == 0 || strncmp(".", entry->d_name, 1) == 0) {
                print_spaces(level);
                ls_l(entry, copy);
            } else {
                print_spaces(level);
                ls_l(entry, copy);
                ls_r(copy, level+1, a, l);
            }
        } else { // option: "r"
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

    // prompt the user to input a command
    char input[200];
    while (strcmp(input, "logout") != 0) {
        printf("$ "); 
        gets(input);
        char* tok1 = strtok(input, " ");
        char* tok2 = strtok(NULL, " ");
        
        if (strcmp("pwd", tok1) == 0) {
            current_dir(directory);
        } else if (strcmp("cd", tok1) == 0 && tok2 != NULL) { // cd
            change_dir(directory, tok2);
        } else if (strcmp("ls", tok1) == 0) {
            if (tok2 == NULL) {
                ls_null(directory);
            } else {
                ls_path(directory, tok2);
            }
        }
    }
}