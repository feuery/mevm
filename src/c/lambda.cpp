#include <cstdio>
#include <cassert>
#include <lambda.h>

#define doMath(opp) assert(op->param1_pointer);\
  int* top = _stack.top();\
  int* _register = getEnv(op->param1);	\
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
      _stack.push(getEnv(op->param1));
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

int* Lambda::getEnv(int addr)
{
  if(addr == 0) throw "Lambda::getEnv addr is 0";
  else if(addr < 0) {
    return params.at(-(addr + 1));
  }
  return env.at(addr - 1);
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
