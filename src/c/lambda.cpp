#include <cstdio>
#include <cassert>
#include <lambda.h>

#define doMath(opp) value_container *top = _stack.top();\
  switch(top->type) {						\
  case INT: {							\
    int top_int = top->v.int_val;				\
								\
    if(op->param1_box->b) {					\
      pointer_container *vm_address = op->param1_box->b;	\
      value_container *machine_box = getEnv(*vm_address);	\
								\
      switch(machine_box->type) {				\
      case INT: {						\
	machine_box->v.int_val opp top_int;			\
	break;							\
      }								\
      case FLOAT: {						\
	machine_box->v.float_val opp top_int;			\
	break;							\
      }								\
      default: {						\
	puts("Illegal state in doMath - macro");		\
	throw "";						\
      };							\
      }								\
    }								\
    else throw "op->param1_box->b is null";			\
    break;							\
  }								\
  case FLOAT: {							\
    float top_float = top->v.float_val;				\
								\
    if(op->param1_box->b) {					\
      pointer_container *vm_address = op->param1_box->b;	\
      value_container *machine_box = getEnv(*vm_address);	\
								\
      switch(machine_box->type) {				\
      case INT: {						\
	machine_box->v.int_val opp top_float;			\
	break;							\
      }								\
      case FLOAT: {						\
	machine_box->v.float_val opp top_float;			\
	break;							\
      }								\
      default: {						\
	puts("Illegal state in doMath - macro");		\
	throw "";						\
      };							\
      }								\
    }								\
    else  throw "op->param1_box->b is null";			\
								\
    break;							\
  }								\
  default: {							\
    puts("Illegal state in doMath - macro");			\
    throw "";							\
  };								\
  }								\

std::unordered_map<int, value_container*> data_section;
std::unordered_map<int, vector<op>::iterator> labels;

void Lambda::call() {
  for(vector<op>::iterator op = code.begin(); op != code.end(); ++op) {
    switch(op->code) {
    case INC: {
      doMath(+=);
      break;
    }
    case DEC: {
      doMath(-=);
      break;
    }
    case MUL: {
      doMath(*=);
      break;
    }
    case DIV: {
      doMath(/=);
      break;
    }
    case PUSH: {
      if(!op->param1_box->a) throw "Has to push a value";
      value_container *val = op->param1_box->a;
      _stack.push(val);
      break;
    }
    case POP: {
      if(!op->param1_box->b) throw "Can't POP to null";
      value_container* c = _stack.top();
      _stack.pop();
      setEnv(*(op->param1_box->b), c);
      break;
    }

    case LABEL: {
      assert(op->param1_box->a);
      int unboxed = op->param1_box->a->v.int_val;
      auto result = labels.find(unboxed);
      if(result == labels.end()) {
	labels[unboxed] = op;
      }
      break;
    }

    case JMP: {
      assert(op->param1_box->a);
      auto jmp_target = labels[op->param1_box->a->v.int_val];
      // ++op; should at the top of the loop should make this jump to the line following the label
      op = jmp_target;
      break;
    }

    case CJMP: {
      assert(op->param1_box->a);
      auto jmp_target = labels[op->param1_box->a->v.int_val];

      value_container *c = _stack.top();
      _stack.pop();

      if((c->type == INT && c->v.int_val != 0) ||
	 (c->type == FLOAT && c->v.float_val != 0.0f))
	op = jmp_target;
      break;
    }

      // Wish we had a GC
    case CONS: {
      assert(op->param1_box->b);
      assert(op->param2_box->b);

      pointer_container *a = op->param1_box->b,
	*b = op->param2_box->b;
      long long car = (long long)a,
	cdr = (long long)b;

      value_container *cons_container = new value_container(CONS_type);
      cons_container->cons_ptr = (car << 4) | cdr;
      _stack.push(cons_container);
      break;
      
    }

    case CAR: {
      value_container *cons = _stack.top();
      assert(cons->type == CONS_type);
      pointer_container *caar = (pointer_container*)(cons->cons_ptr >> 4);
      _stack.push(getEnv(*caar));
      break;
    }

    case CDR: {
      value_container *cons = _stack.top();
      assert(cons->type == CONS_type);
      pointer_container *caar = (pointer_container*)(cons->cons_ptr & 0xF);
      _stack.push(getEnv(*caar));
      break;
    }

    default:
      printf("NOPping %s\n", opcode_to_str(op->code));
      break;
    }
  }
}

value_container* Lambda::getEnv(pointer_container addr)
{
  int val = addr.value;
  if(!addr.is_data) {
    if(val == 0) throw "Can't get variables from addr 0";
    if (val < 0) { return params.at(-(val+1)); }
    else return env.at(val-1);
  }
  else return data_section[val]; 
}

void Lambda::setEnv(pointer_container addr, value_container* ptr)
{
  int val = addr.value;
  if(!addr.is_data) {
    if(val == 0) throw "Can't set variables to addr 0";
    val--;
    if (val < 0) { params[-(val+1)] = ptr; }
    else env[val-1] = ptr;
  }
  else data_section[val] = ptr;
}
