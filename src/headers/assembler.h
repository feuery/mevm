#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <either.h>
#include <lambda.h>

std::vector<std::string> getLines (const char *file);
void generate_code(std::vector<std::string> lines, const char *outputFilename);
// Needed in the disassembler
Either<value_container, pointer_container> generateContainer(long Value);
long filesize(FILE* f);

extern const char* symbol_regex;

#endif //ASSEMBLER_H
