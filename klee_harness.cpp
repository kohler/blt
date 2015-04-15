#include <klee/klee.h>
#include "new_calc.h"
#include "old_calc.h"
#include <cstdio>
#include <cassert>

#define TRACE_DEPTH 2

int main() {
  unsigned int fs[TRACE_DEPTH];
  klee_make_symbolic(&fs, sizeof(fs), "fs");

  unsigned int o = 0;  // stupid hack for recording interleavings
  unsigned int *p;
  old_calc::init_pressed();
  new_calc::init_pressed();
  
  for (p = fs; p < &fs[TRACE_DEPTH]; ++p) {
    o *= 10;
    o += *p;
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
        int old_r = old_calc::eval_pressed();
        int new_r = new_calc::eval_pressed();
        assert(old_r == new_r);
        // uncomment to short-circuit
        // if (old_r != new_r) goto FAILURE;   
        break;
      default :
        break;
    }
  }

  printf("%d\n", o);
  return 0;

  FAILURE:
  printf("%d\n", o);
  klee_assert(0);
}
