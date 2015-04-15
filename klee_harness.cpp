#include <klee/klee.h>
#include "new_calc.h"
#include "old_calc.h"
#include <cstdio>
#include <cassert>

#define TRACE_DEPTH 10 

int main() {
  unsigned int fs[TRACE_DEPTH];
  unsigned int output[TRACE_DEPTH];
  klee_make_symbolic(&fs, sizeof(fs), "fs");

  unsigned int o = 0;  // stupid hack for recording interleavings
  unsigned int *p;
  old_calc::init_pressed();
  new_calc::init_pressed();
  
  for (p = fs; p < &fs[TRACE_DEPTH]; ++p) {
    klee_assume (*p < 5);
    output[p - fs] = *p;
    switch (*p) {
      case 0 :
        old_calc::zero_pressed();
        new_calc::zero_pressed();
        break;
      case 1 :
        old_calc::one_pressed();
        new_calc::one_pressed();
        break;
      case 2 :
        old_calc::plus_pressed();
        new_calc::plus_pressed();
        break;
      case 3 :
        old_calc::mult_pressed();
        new_calc::mult_pressed();
        break;
      case 4 :
      {
        int old_r = old_calc::eval_pressed();
        int new_r = new_calc::eval_pressed();
        if (old_r != new_r) goto FAILURE;
        //assert(old_r == new_r);
        break;
      }
      default :
        break;
    }
  }

  for (int i = 0; i < TRACE_DEPTH; i++)
    printf("%d", output[i]);
  printf("\n");

  FAILURE:
  klee_assert(0);
}
