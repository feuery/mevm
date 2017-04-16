#include <cstdio>
#include <cassert>
#include <cstring>

long filesize(FILE* f)
{
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  return size;
}

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

  long size = filesize(f);
  char buffer[size];
  size_t howmuch = fread(buffer, 1, size, f);
  assert(size == howmuch);
  
  fclose(f);
  short dataCount = (short)(*buffer);
  int pointer = 2;
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
      printf("Reading %d of characters\n", len);
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
  return 0;
}
			       
  
