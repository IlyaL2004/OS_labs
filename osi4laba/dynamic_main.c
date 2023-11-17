#include <stdio.h> 
#include <stdlib.h> 
#include <dlfcn.h>

int contr = 1;

const char* first_library_name = "libfirst.so"; 
const char* second_library_name = "libsecond.so";

float (*derivative)(float, float) = NULL; 
char* (*translation)(long) = NULL;

void* lib_handle = NULL;

void load_lib(int contr) { 
  const char* name;

    if(contr == 1){
      name = first_library_name; 
    }
        
    else if(contr == 2){
      name = second_library_name;
    }
  lib_handle = dlopen(name, RTLD_LAZY); 
  
  if (lib_handle == NULL) {
    perror("dlopen");
    exit(EXIT_FAILURE); 
  }
}

void load_contract() {
  load_lib(contr); 
  derivative = dlsym(lib_handle, "derivative");
  translation = dlsym(lib_handle, "translation"); 
}

void change_contract() { 
  dlclose(lib_handle); 

    if(contr == 1){
      contr = 2;
    }
    else if (contr == 2){ 
      contr = 1; 
    }
     
  load_contract(); 
}

int main() {
  load_contract();
  int command = 0;
  
  while (scanf("%d", &command) != EOF) {
      if(command == 0){
        change_contract();
        printf("Contract has been changed\n"); 
          if(contr == 1) {
            printf("Contract is first\n");
          }

          else if(contr == 2){ 
            printf("Contract is second\n");
          } 
        }
      else if(command == 1){
        float point, increment;
        
        if(scanf("%f %f", &point, &increment) == 2) {
          printf("%.6f\n", derivative(point, increment)); 
        }
      }
      else if (command == 2){
        long numeric;
        
        if(scanf("%ld", &numeric) == 1) {
          printf("Translation from 10 to "); 
        
            if(contr == 1){ 
              printf("2");
            }
            else if (contr == 2){ 
              printf("3");
            } 
          }
          printf(" number system: %s\n", translation(numeric)); 
        }
      else
        printf("Invalid request\n");
  }
  return 0; 
}
