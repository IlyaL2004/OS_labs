#include "realization.h"
#include <stdlib.h> 


double power(double base, int exponent) {
    double result = 1;
    for (int i = 0; i < abs(exponent); i++) {
        result *= base;
    }
    if (exponent < 0) {
        return 1 / result;
    } else {
        return result;
    }
}


double factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

double cosx(double x) {
    double result = 0;
    for (int i = 0; i < 10; i++) {
        result += power(-1, i) * power(x, 2 * i) / factorial(2 * i);
    }
    return result;
}



float derivative(float point, float increment) { 
  return (cosx(point + increment) - cosx(point)) / increment;
}

char* translation(long num) {
  char* result = (char*)malloc(sizeof(char)); 
  int index = 0;
  
  while(num > 0) {
    result[index] = num % 2 + '0';
    index += 1;
    num /= 2;
    result = realloc(result, (index + 1) * sizeof(char));
  }
  
  result[index] = '\0';
  char symbol;

  for(int permutation_index = 0; permutation_index < index / 2; ++permutation_index) {
    symbol = result[permutation_index];
    result[permutation_index] = result[index - permutation_index - 1];
    result[index - permutation_index - 1] = symbol; 
  }
  
  return result; 
}
