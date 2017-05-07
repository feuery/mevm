#include <cstdio>
#include <cassert>
#include <lambda.h>

#define doMath(opp) assert(op->param1_box->b);\
      assert(op->param2_box->a || op->param2_box->a);\
      value_container *a = getEnv(*op->param1_box->b);\
      a->v.int_val opp intVal(op->param2_box);\

std::unordered_map<int, value_container*> data_section;
std::unordered_map<int, vector<op>::iterator> labels;
value_container RET_register;

primitive Lambda::TYPEOF(Either<value_container, pointer_container> *e) {
  if(e->a) {
    value_container *v = e->a;
    return v->type;
  }
  else {
    value_container *v = getEnv(*e->b);
    return v->type;
  }
}

int Lambda::intVal(Either<value_container, pointer_container> *e) {
  if(e->a)
    return e->a->v.int_val;
  value_container *v = getEnv(*e->b);
  return v->v.int_val;
}

result<value_container> Lambda::call() {
  for(vector<op>::iterator op = code.begin(); op != code.end(); ++op) {
    switch(op->code) {
    case INC: {
      doMath(+=);
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
    case LT: {
      assert(op->param1_box->b);
      assert(op->param2_box->b);

      int v1 = intVal(op->param1_box),
	v2 = intVal(op->param2_box);
      value v;
      v.int_val = v1 < v2? 1:0;
      value_container vc(v, INT);
      RET_register = vc;
      break;
    }
    case GT: {
      assert(op->param1_box->b);
      assert(op->param2_box->b);

      int v1 = intVal(op->param1_box),
	v2 = intVal(op->param2_box);
      value v;
      v.int_val = v1 > v2? 1:0;
      value_container vc(v, INT);
      RET_register = vc;
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
      assert(op->param2_box->b);
      
      auto jmp_target = labels[op->param1_box->a->v.int_val];

      value_container *c = getEnv(*op->param2_box->b);

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

      value_container cons_container(CONS_type);
      cons_container.cons_ptr = (car << 4) | cdr;
      RET_register = cons_container;
      break;      
    }

    case CAR: {
      assert(op->param1_box->b);
      assert(op->param2_box->a);
      
      value_container *cons = getEnv(*op->param1_box->b);
      assert(cons->type == CONS_type);
      pointer_container *caar = (pointer_container*)(cons->cons_ptr >> 4);
      RET_register = *getEnv(*caar);      
      break;
    }

    case CDR: {
      assert(op->param1_box->b);
      assert(op->param2_box->a);
      
      value_container *cons = getEnv(*op->param1_box->b);
      assert(cons->type == CONS_type);
      pointer_container *caar = (pointer_container*)(cons->cons_ptr & 0xF);
      RET_register = *getEnv(*caar);
      break;
    }

    case RET: {
      assert(op->param1_box->a || op->param1_box->b);
      int i = intVal(op->param1_box);
      primitive t = TYPEOF(op->param1_box);
      value v;
      v.int_val = i;
      value_container vc(v, t);

      result<value_container> r(vc, true);
      
      return r;
    }
      
    default:
      printf("NOPping %s\n", opcode_to_str(op->code));
      break;
    }
  }

  result<value_container> r;
  return r;
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
