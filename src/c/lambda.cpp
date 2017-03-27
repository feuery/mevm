#include <cstdio>
#include <cassert>
#include <lambda.h>

#define doMath(opp) assert(op->param1_pointer);\
  int* top = _stack.top();\
  int* _register = getEnv(&op->param1, op->param1_pointer);\
  _stack.pop();\
  *(_register) = *(_register) opp *top;

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
    case PUSH: {
      _stack.push(getEnv(&op->param1, op->param1_pointer));
      break;
    }
    case POP: {
      int* top = _stack.top();
      _stack.pop();
      setEnv(op->param1, top);
    }

    default:
      printf("NOPping %s\n", opcode_to_str(op->code));
      break;
    }
  }
}

int* Lambda::getEnv(int *addr, bool is_ptr)
{
  if(is_ptr) {
    int index = *addr;
    if(index == 0) throw "Lambda::getEnv index is 0";
    else if(index < 0) {
      return params.at(-(index + 1));
    }
    return env.at(index - 1);
  }
  // A literal!
  else 
    return addr;    
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
