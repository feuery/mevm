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
    }
  return "Unknown opcode";
}

result<opcodes> char_to_opcode(const char* c) {
  if(strcmp(c, "LABEL") == 0) {
    return {LABEL, true};
  }
  else if(strcmp(c, "JMP") == 0) {
    return {JMP, true};
  }
  else if(strcmp(c, "CJMP") == 0) {
    return {CJMP, true};
  }
  else if(strcmp(c, "CONS") == 0) {
    return {CONS, true};
  }
  else if(strcmp(c, "CAR") == 0) {
    return {CAR, true};
  }
  else if(strcmp(c, "CDR") == 0) {
    return {CDR, true};
  }
  else if(strcmp(c, "LAMBDA") == 0) {
    return {LAMBDA, true};
  }
  else if(strcmp(c, "INC") == 0) {
    return {INC, true};
  }
  else if(strcmp(c, "DEC") == 0) {
    return {DEC, true};
  }
  else if(strcmp(c, "INC_1") == 0) {
    return {INC_1, true};
  }
  else if(strcmp(c, "DEC_1") == 0) {
    return {DEC_1, true};
  }
  else if(strcmp(c, "MUL") == 0) {
    return {MUL, true};
  }
  else if(strcmp(c, "DIV") == 0) {
    return {DIV, true};
  }
  else if(strcmp(c, "RET") == 0) {
    return {RET, true};
  }
  else if(strcmp(c, "NOP") == 0) {
    return {NOP, true};
  }
  else if(strcmp(c, "PUSH") == 0) {
    return {PUSH, true};
  }
  else if(strcmp(c, "POP") == 0) {
    return {POP, true};
  }
  else if(strcmp(c, "PEEK") == 0) {
    return {PEEK, true};
  }
  else if(strcmp(c, "GT") == 0) {
    return {GT, true};
  }
  else if(strcmp(c, "LT") == 0) {
    return {LT, true};
  }
  else if(strcmp(c, "GOEQ") == 0) {
    return {GOEQ, true};
  }
  else if(strcmp(c, "LOEQ") == 0) {
    return {LOEQ, true};
  }
  else if(strcmp(c, "EQ") == 0) {
    return {EQ, true};
  }
  return {JMP, false};
}
