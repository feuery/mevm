#ifndef OPCODES_H
#define OPCODES_H

enum opcodes {
  LABEL,
  JMP,
  CJMP,
  CONS,
  CAR,
  CDR,
  LAMBDA,
  INC,
  DEC, 
  INC_1,
  DEC_1,
  MUL,
  DIV,
  RET,
  NOP,
  PUSH,
  POP,
  PEEK,
  GT,
  LT,
  GOEQ,
  LOEQ,
  EQ};

const char* opcode_to_str (opcodes c);
#endif //OPCODES_H
