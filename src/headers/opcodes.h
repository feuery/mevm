#ifndef OPCODES_H
#define OPCODES_H

template<typename T>
struct result {
  T Result;
  bool success = false;
  result(T r, bool s): Result(r), success(s) { }

  result() { }
};

enum opcodes: unsigned char {
  LABEL,
  JMP,
  CJMP,
  CONS,
  CAR,
  CDR,
  LAMBDA,
  INC,
  DEC, 
  MUL,
  DIV,
  RET,
  NOP,
  GT,
  LT,
  GOEQ,
  LOEQ,
  EQ,
  CALL};

const char* opcode_to_str (opcodes c);
result<opcodes> char_to_opcode(const char* c);
#endif //OPCODES_H
