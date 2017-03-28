#include <cstdio>
#include <cassert>
#include <lambda.h>

#define doTypelessMath(opp, type) assert(op->param1_pointer);\
  type* top = _stack.top();						\
  printf("top is %d %f\n", *top, (float)(*top));			\
  type* _register = (type*)getEnv(&op->param1, op->param1_pointer);\
  _stack.pop();\
  type result = (type)(*_register) opp (type)(*top);\
  printf("Result is %d, %f\n", (int)result, (float)result);\
  *(_register) = result;

#define doMath(opp)\
  if(op->param1_float) { puts("Tehään float-matikkaa\n"); doTypelessMath(opp, int); }\
  else { puts("Tehään int-matikkaa\n"); doTypelessMath(opp, int); };

void Lambda::call() {
  for(vector<op>::iterator op = code.begin(); op != code.end(); ++op) {
    switch(op->code) {
      case INC: {
        doMath(+)
        break;
      }
      case DEC: {
        doMath(-);
        break;
      }
      case MUL: {
        doMath(*);
        break;
      }
      case DIV: {
        doMath(/);
        break;
      }
	// Pitäiskö tän tietää floattiudesta?
      case PUSH: {
	int* val = getEnv(&op->param1, op->param1_pointer);
	printf("Pushing %d/%f\n", *val, (float)(*val));
        _stack.push(val);
        break;
      }
      case POP: {
        if(op->param1_float) {
	  float* top = (float*)_stack.top();
	  _stack.pop();
	  setEnv(op->param1, (int*)top);
        }
        else {
	  int* top = _stack.top();
	  _stack.pop();
	  setEnv(op->param1, top);
        }
      }

      default:
        printf("NOPping %s\n", opcode_to_str(op->code));
        break;
    }
  }
}

int* Lambda::getEnv(int *addr, bool is_ptr, bool is_float)
{
  if(is_ptr) {
    int index = *addr;
    if(index == 0) throw "Lambda::getEnv index is 0";
    else if(index < 0) {
      return params.at(-(index + 1));
    }
    return env.at(index - 1);
  }
  else {
    printf("Lambda::getEnv() returning literal %d/%f\n", *addr, (float)(*addr));
    return addr;
  }
}

void Lambda::setEnv(int addr, int* ptr)
{
  if(addr == 0) throw "Lambda::getEnv addr is 0";
  else if(addr < 0) {
    params.at(-(addr + 1)) = ptr;
  }
  else
    env.at(addr-1);
}
