#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(){
  if(OpenServiceA(OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS),"PuppetMaster",SERVICE_ALL_ACCESS)==NULL){
    system("sc create PuppetMaster type=own error=ignore binPath=%cd%\\puppetmaster.exe");
  }
  system("sc start PuppetMaster");
  return 0;
}
