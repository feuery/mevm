#ifndef LAMBDA_H
#define LAMBDA_H

#include <opcodes.h>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

class op {
 public:
  op(opcodes c, int p1 = 0, bool p1_pointer = false, bool p1_float = false, int p2 = 0, bool p2_pointer = false, bool p2_float = false)
  {
    code = c;
    param1 = p1;
    param1_pointer = p1_pointer;
    param2 = p2;
    param2_pointer = p2_pointer;
    param1_float = p1_float;
    param2_float = p2_float;
  }
  opcodes code;
  int param1;
  bool param1_pointer = false;
  bool param1_float = false;
  
  int param2;
  bool param2_pointer = false;
  bool param2_float = false;
};

class Lambda{
public:
  const char* name;
  vector<op> code;
  vector<int*> env;
  vector<int*> params;
  stack<int*> _stack;

  void call();

  int* getEnv(int *addr, bool is_ptr = true, bool is_float = false);
  void setEnv(int addr, int* ptr);
  
private:

};

#endif //LAMBDA_H
