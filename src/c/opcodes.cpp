#include <opcodes.h>

const char* opcode_to_str (opcodes c){
  switch(c)
    {
    case LABEL: return "LABEL";
    case JMP: return "JMP";
    case CJMP: return "CJMP";
    case CONS: return "CONS";
    case CAR: return "CAR";
    case CDR: return "CDR";
    case LAMBDA: return "LAMBDA";
    case INC: return "INC";
    case DEC:  return "DEC";
    case INC_1: return "INC_1";
    case DEC_1: return "DEC_1";
    case MUL: return "MUL";
    case DIV: return "DIV";
    case RET: return "RET";
    case NOP: return "NOP";
    case PUSH: return "PUSH";
    case POP: return "POP";
    case PEEK: return "PEEK";
    case GT: return "GT";
    case LT: return "LT";
    case GOEQ: return "GOEQ";
    case LOEQ: return "LOEQ";
    case EQ: return "EQ";
    }
  return "Unknown opcode";
}
