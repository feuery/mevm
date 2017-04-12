#include <cstring>
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
    case CALL: return "CALL";
    }
  return "Unknown opcode";
}

result<opcodes> char_to_opcode(const char* c) {
  if(strcmp(c, "LABEL") == 0) {
    return result<opcodes>(LABEL, true);
  }
  else if(strcmp(c, "JMP") == 0) {
    return result<opcodes>(JMP, true);
  }
  else if(strcmp(c, "CJMP") == 0) {
    return result<opcodes>(CJMP, true);
  }
  else if(strcmp(c, "CONS") == 0) {
    return result<opcodes>(CONS, true);
  }
  else if(strcmp(c, "CAR") == 0) {
    return result<opcodes>(CAR, true);
  }
  else if(strcmp(c, "CDR") == 0) {
    return result<opcodes>(CDR, true);
  }
  else if(strcmp(c, "LAMBDA") == 0) {
    return result<opcodes>(LAMBDA, true);
  }
  else if(strcmp(c, "INC") == 0) {
    return result<opcodes>(INC, true);
  }
  else if(strcmp(c, "DEC") == 0) {
    return result<opcodes>(DEC, true);
  }
  else if(strcmp(c, "INC_1") == 0) {
    return {INC_1, true};
  }
  else if(strcmp(c, "DEC_1") == 0) {
    return {DEC_1, true};
  }
  else if(strcmp(c, "MUL") == 0) {
    return result<opcodes>(MUL, true);
  }
  else if(strcmp(c, "DIV") == 0) {
    return result<opcodes>(DIV, true);
  }
  else if(strcmp(c, "RET") == 0) {
    return result<opcodes>(RET, true);
  }
  else if(strcmp(c, "NOP") == 0) {
    return result<opcodes>(NOP, true);
  }
  else if(strcmp(c, "PUSH") == 0) {
    return result<opcodes>(PUSH, true);
  }
  else if(strcmp(c, "POP") == 0) {
    return result<opcodes>(POP, true);
  }
  else if(strcmp(c, "PEEK") == 0) {
    return result<opcodes>(PEEK, true);
  }
  else if(strcmp(c, "GT") == 0) {
    return result<opcodes>(GT, true);
  }
  else if(strcmp(c, "LT") == 0) {
    return result<opcodes>(LT, true);
  }
  else if(strcmp(c, "GOEQ") == 0) {
    return result<opcodes>(GOEQ, true);
  }
  else if(strcmp(c, "LOEQ") == 0) {
    return result<opcodes>(LOEQ, true);
  }
  else if(strcmp(c, "EQ") == 0) {
    return result<opcodes>(EQ, true);
  }
  else if(strcmp(c, "CALL") == 0) {
    return result<opcodes>(CALL, true);
  }
  return result<opcodes>(JMP, false);
}
