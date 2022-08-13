#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void pwd_command(char* dir){
    getcwd(dir, 199);
}

void cd_command(char* user, char* dir){
    if(user[3] == '/' || (strstr(user,":\\")) != NULL){
        for(int i = 0; i < strlen(user); i++){
                dir[i]=user[i+3];
            }
        dir[strlen(user)] = '\0';
    }else if(strlen(user) == 2){
    }else{
        for(int i = 0; i < strlen(user)-3; i++){
            user[i]=user[i+3];
        }
        user[strlen(user)-3] = '\0';
        strcat(dir,"/");
        strcat(dir, user);
    }    
}

void extra_info_loop(struct dirent *entptr, char* copy){
    //stat(entptr->d_name, &pathptr);
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
    printf(" %d %s", size, entptr->d_name);
    printf("\n");
}

void r_command_setup(char bckslash[], char* direcptr, struct dirent *entptr){
    strcpy(bckslash, direcptr);
    char* temp = bckslash;
    temp += strlen(direcptr);
    strcpy(temp, "/");
    temp++;
    strcpy(temp,entptr->d_name);
}

// void r_command(char* direc, bool a_ptr, bool l_ptr, int cnt_ptr){
//     struct stat path_stat;
//     DIR *dir; 
//     struct dirent *entry;
//     dir = opendir(direc); 
//     char backslash[500];
//     while((entry = readdir(dir)) != NULL){
//         if(l_ptr && a_ptr){
//             r_command_setup(backslash,direc,entry);
//             stat(backslash, &path_stat);
//             if(!S_ISDIR(path_stat.st_mode)){
//                 for(int i = 0; i < cnt_ptr; i++){
//                     printf("\t");
//                 }
//                 extra_info_loop(entry,path_stat);
//             }else if(entry->d_name[0]!='.'){            
//                 extra_info_loop(entry,path_stat);
//                 r_command(backslash,a_ptr,l_ptr,cnt_ptr+1);
//             }
//         }else if(a_ptr && !l_ptr){
//             r_command_setup(backslash,direc,entry);
//             stat(backslash, &path_stat);
//             if(!S_ISDIR(path_stat.st_mode)){
//                 for(int i = 0; i < cnt_ptr; i++){
//                     printf("\t");
//                 }
//                 printf("%s\n",entry->d_name);
//             }else if(entry->d_name[0]!='.'){            
//                 printf("%s\n",entry->d_name);
//                 r_command(backslash,a_ptr,l_ptr,cnt_ptr+1);
//             }
//         }else if(!a_ptr&&l_ptr){
//             r_command_setup(backslash,direc,entry);
//             stat(backslash, &path_stat);
//             if(!S_ISDIR(path_stat.st_mode)&&entry->d_name[0] != '.'){
//                 for(int i = 0; i < cnt_ptr; i++){
//                     printf("\t");
//                 }
//                 extra_info_loop(entry,path_stat);
//             }else if(entry->d_name[0] != '.'){            
//                 printf("%s\n",entry->d_name);
//                 r_command(backslash,a_ptr,l_ptr,cnt_ptr+1);
//             }
//         }else{
//             r_command_setup(backslash,direc,entry);
//             stat(backslash, &path_stat);
//             if(!S_ISDIR(path_stat.st_mode)&&entry->d_name[0] != '.'){
//                 for(int i = 0; i < cnt_ptr; i++){
//                     printf("\t");
//                 }
//                 printf("%s\n",entry->d_name);
//             }else if(entry->d_name[0] != '.'){            
//                 printf("%s\n",entry->d_name);
//                 r_command(backslash,a_ptr,l_ptr,cnt_ptr+1);
//             }
//         }
//     }
// }
// need ls -lar path
void ls_command(char* direc, char* user) {
    char backslash[500];
    DIR *dir; 
    struct dirent *entry;
    dir = opendir(direc);
    struct stat path_stat;
    stat(entry->d_name,&path_stat);
    int rcount = 0;
    int count = 0;
    for(int i = 1; i < strlen(user); i++){
        if(user[0]==user[i]){
            count++;
        }
    }
    char* copy = (char*)malloc(500 * sizeof(char));
    

    bool contain_a = strchr(user,'a')!=NULL;
    bool contain_l = strchr(user,'l') != NULL && count>0;
    bool contain_r = strchr(user,'r');
    if(strlen(user)==2 || strchr(user,'-')==NULL){
            char temp[500];
            strcpy(temp, direc);
            cd_command(user,temp);
            dir = opendir(temp);
        while((entry = readdir(dir)) != NULL){
            if(entry->d_name[0]!='.'){
                printf("%s\n",entry->d_name);
            }
        }
    }
    //This is the only function that's printing the wrong numbers for size and bit info when calling -l for anything but home!!!!
    // not changing directories each time in stat fucntion
    if(strchr(user,'-')!=NULL&&!contain_r){
        while((entry = readdir(dir)) != NULL){
            strcpy(copy, direc);
        strcat(copy, "/");
        strcat(copy, entry->d_name);
            if(contain_a){
                if(contain_l){
                    extra_info_loop(entry,copy);
                }else{
                    printf("%s\n",entry->d_name);
                }
            }else if(contain_l){
                if(entry->d_name[0]!='.'){
 
                    extra_info_loop(entry,copy);
                    }
                }
            }
        }
    else if(strchr(user,'-')!=NULL&&contain_r){
        // r_command(direc, contain_a, contain_l,rcount);
    }
}

int main(){
    char directory[500];
    char user_input[500];
    pwd_command(directory);
    printf("Current working dir: %s", directory);
    printf("\n$ ");
    gets(user_input);
    while(strcmp(user_input,"logout")!= 0){
        if(strcmp(user_input,"pwd")==0){
            printf("%s\n", directory);
        } else if(strncmp(user_input,"cd", 2)==0){
            cd_command(user_input, directory);
        } else if(strncmp(user_input,"ls",2)==0){
            ls_command(directory, user_input);
        }
        printf("$ ");
        scanf(" %[^\n]", &user_input);
    }
    return 0;
}
