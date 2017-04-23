#ifndef LAMBDA_H
#define LAMBDA_H

#include <opcodes.h>
#include <vector>
#include <stack>
#include <either.h>
#include <unordered_map>

using std::vector;
using std::stack;

union value {
  int int_val;
  float float_val;
};

enum primitive { INT, FLOAT, CONS_type };

struct value_container {
  value_container(value vv, primitive t): v(vv), type(t) { }
  value_container(primitive t): type(t) { }
  value v;
  // Remember to delete by hand every value_container whose type is CONS_type
  primitive type;
  // Consses are (currently) never serialized to a binary so this being a bit hacky is ok
  long long cons_ptr;
};
  

struct pointer_container {
  int value;
  bool is_data = false;
};

extern std::unordered_map<int, value_container*> data_section;

class op {
 public:
  op(opcodes c,
     Either<value_container, pointer_container> *p1 = nullptr,
     Either<value_container, pointer_container> *p2 = nullptr)
  {
    code = c;
    param1_box = p1;
    param2_box = p2;
  }
  
  opcodes code;
  Either<value_container, pointer_container> *param1_box, *param2_box;
};

class Lambda{
public:
  const char* name;
  vector<op> code;
  vector<value_container*> env;
  vector<value_container*> params;
  stack<value_container*> _stack;

  value_container* call();

  value_container* getEnv(pointer_container addr);
  void setEnv(pointer_container addr, value_container* ptr);
  
private:

};

#endif //LAMBDA_H
