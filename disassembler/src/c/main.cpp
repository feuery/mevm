#include <cstdio>
#include <cassert>
#include <cstring>

#include <opcodes.h>
#include <assembler.h>

union converter {
  int i;
  float f;
};
  
int main(int argc, char** argv) {
  if(argc != 2) {
    puts("Usage: ./disassembler binary-path");
    return -1;
  }
  
  char* binary_path = argv[1];
  FILE *f = fopen(binary_path, "r");

  if(!f) {
    printf("Couldn't open %s, exiting\n", binary_path);
    return -1;
  }

  unsigned long size = filesize(f);
  char buffer[size];
  size_t howmuch = fread(buffer, 1, size, f);
  assert(size == howmuch);
  
  fclose(f);
  short dataCount = (short)(*buffer);
  unsigned int pointer = 2;
  printf("%d is the count of data elements\n", dataCount);

  for(int i = 0; i < dataCount; i++) {
    unsigned char type = (unsigned char)(*(buffer + pointer));
    pointer++;
    short len = (short)(*(buffer + pointer));
    pointer += 2;
    switch(type) {
    case 1: {
      assert(len == sizeof(int));
      int d = (int)(*(buffer + pointer));
      printf("Read an int: %d\n", d);
      break;
    }
    case 2: {
      assert(len == sizeof(float));
      converter c;
      int* f = (int*)(buffer + pointer);
      c.i = *f;
      printf("Read a float: %f\n", c.f);
      break;
    }
    case 3: {
      char *result = new char[len];
      strncpy(result, (char*)(buffer + pointer), len);
      printf("Read a string: %s\n", result);
      delete [] result;
      break;
    }
    }
    pointer += len;
  }
  puts("Data section loaded");
  printf(".prog section starts at %d\n", pointer);

  while(pointer < howmuch) {
    unsigned char opcode = (unsigned char)(*(buffer + pointer));
    pointer += sizeof(unsigned char);
    long param1 = (long)(*(buffer + pointer));
    pointer += sizeof(long);
    long param2 = (long)(*(buffer + pointer));
    pointer += sizeof(long);

    auto param1_container = generateContainer(param1),
      param2_container = generateContainer(param2);

    printf("Opcode is %s\n", opcode_to_str((opcodes)opcode));
    
    if(param1_container.a) {
      if(param1_container.a->type == FLOAT) {
	puts("Param1's type is float");
	printf("Param1's value is %f\n", param1_container.a->v.float_val);
      }
      else {
	puts("Param1's type is int");
	printf("Param1's value is %d\n", param1_container.a->v.int_val);
      }      
    }
    else if(param1_container.b) {
      if(param1_container.b->is_data) puts("Param1 is a data pointer");
      else puts("Param1 is an env pointer!");

      printf("Param1's pointer value is %d\n", param1_container.b->value);
    }
    else puts("Param1 doesn't have a meaningful value");

    if(param2_container.a) {
      if(param2_container.a->type == FLOAT) {
	puts("Param2's type is float");
	printf("Param2's value is %f\n", param2_container.a->v.float_val);
      }
      else {
	puts("Param2's type is int");
	printf("Param2's value is %d\n", param2_container.a->v.int_val);
      }      
    }
    else if(param2_container.b) {
      if(param2_container.b->is_data) puts("Param2 is a data pointer");
      else puts("Param2 is an env pointer!");

      printf("Param2's pointer value is %d\n", param2_container.b->value);
    }
    else puts("Param2 doesn't have a meaningful value");
  }
  
  return 0;
}
			       
  
