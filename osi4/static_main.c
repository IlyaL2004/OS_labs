#include "realization.h"
#include <stdio.h>

int main() {
  int c = 0;
  
  while(scanf("%d", &c) != EOF) {
      if(c == 1){
        float point, increment;
        
        if(scanf("%f %f", &point, &increment) == 2) {
          printf("%.6f\n", derivative(point, increment)); 
        } 
      }

      else if(c ==2){ 
        long numeric;
        
        if(scanf("%ld", &numeric) == 1) {
          printf("%s\n", translation(numeric)); 
        }   
      }
      else{
        printf("Invalid\n");
      } 
    }

  return 0; 
}
