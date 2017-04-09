#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>

std::vector<std::string> getLines (const char *file);
void generate_code(std::vector<std::string> lines, const char *outputFilename);

#endif //ASSEMBLER_H
