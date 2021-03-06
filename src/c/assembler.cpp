#include <opcodes.h>
#include <assembler.h>
#include <lambda.h>

#include <cassert>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <regex>
#include <unordered_map>

using std::vector;
using std::string;
using std::istringstream;
using std::regex;
using std::regex_replace;
using std::regex_match;
using std::unordered_map;

Either<value_container, pointer_container> generateContainer(long Value) {
  Either<value_container, pointer_container> e;
  int meta_bits = Value & 07;
  int real_value = Value >> 3;
  bool isAddr = meta_bits & 04,
    isData = meta_bits & 02,
    isFloat = meta_bits & 01;
    
  if(isData || isAddr) {
    pointer_container *c = new pointer_container;
    c->value = real_value;
    c->is_data = isData;
    e.b = c;
  }
  else {
    value vvv;
    vvv.int_val = real_value;
    value_container *v = new value_container(vvv, isFloat? FLOAT:INT);
    e.a = v;
  }
  return e;
}

long filesize(FILE* f)
{
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  return size;
}

std::string read_source(const char* src)
{
  FILE* f = fopen(src, "r");
  if(f) {
    long f_size = filesize(f);
    char* buffer = new char[f_size];

    if(!buffer) {
      fclose(f);
      delete [] buffer;
      return "";
    }
    else {
      fread(buffer, 1, f_size, f);
      fclose(f);
      std::string toret = buffer;
      delete [] buffer;
      return toret;
    }
  } else puts("File wasn't opened");

  return "";
}

std::vector<std::string> resplit(const std::string & s, std::string rgx_str = "\\s+") {
  std::vector<std::string> elems;

  std::regex rgx (rgx_str);

  std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
  std::sregex_token_iterator end;

  while (iter != end)  {
    elems.push_back(*iter);
    ++iter;
  }

  return elems;

}

void split(const string& s, char c,
           vector<string>& v) {
  string::size_type i = 0;
  string::size_type j = s.find(c);

  while (j != string::npos) {
    v.push_back(s.substr(i, j-i).c_str());
    i = ++j;
    j = s.find(c, j);

    if (j == string::npos)
      v.push_back(s.substr(i, s.length()).c_str());
  }
}

vector<string> getLines (const char *file) {
  string src = read_source(file);
  vector<string> splitted;
  split(src, '\n', splitted);
  return splitted;
}

std::string & ltrim(std::string & str)
{
  auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( str.begin() , it2);
  return str;   
}

std::string & rtrim(std::string & str)
{
  auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( it1.base() , str.end() );
  return str;   
}

std::string & trim(std::string & str)
{
   return ltrim(rtrim(str));
}

std::string toUpper(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), toupper);
  return str;
}

int toInt(std::string& str)
{
  return atoi(str.c_str());
}

float toFloat(std::string& str)
{
  return strtof(str.c_str(), nullptr);
}

void testTypeTags (long bin_param1, int bin_param1_alkuperainen, bool isFloat, bool isData, bool isAddr) {
  if(!isFloat && !isData && !isAddr) {
    Either<value_container, pointer_container> reversed = generateContainer(bin_param1);
    reversed.delete_on_exit = true;
    assert(reversed.a);
    value_container *c = reversed.a;
    assert(c->type == isFloat? FLOAT: INT);
    assert(c->v.int_val == bin_param1_alkuperainen);
  }
  else if(isData || isAddr) {
    auto reversed = generateContainer(bin_param1);
    reversed.delete_on_exit = true;
    assert(reversed.b);
    pointer_container *c = reversed.b;
    assert(c->is_data || !isData);
    // You don't actually have to save the type tag to a pointer, only to a value
    assert(c->value == bin_param1_alkuperainen);
  }
  else printf("Mikään ei ole totta %d, %d, %d\n", isAddr, isData, isFloat);
}

void testTypeTags(long bin_param1, float bin_param1_alkuperainen, bool isFloat, bool isData, bool isAddr) {
  assert(isFloat);
  auto reversed = generateContainer(bin_param1);
  reversed.delete_on_exit = true;
  float f = reversed.a->v.float_val;
  assert(f == bin_param1_alkuperainen);
}

const char *symbol_regex = "^[\\w|\\d]+$";
unordered_map<string, long> label_internment;

result<long> makeFileParam(string& p) {
  string param1 = trim(p);
  result<long> r;
  bool is1Addr = param1.c_str()[0] == '\'',
    is1Data = param1.c_str()[0] == '!',
    is1Float = param1.find(".") != string::npos;

  const char* reg = "-?['!]?(\\d+\\.?\\d*)";
  regex addr_to_value(reg);

  if(regex_match(param1.c_str(), addr_to_value)) {
    string param1_sanitized = regex_replace(param1.c_str(), addr_to_value, "$1");

    if(!is1Float) {
      long bin_param1 = toInt(param1);
      int bin_param1_alkuperainen = bin_param1;
      bin_param1 = bin_param1 << 3;
      bin_param1 |= is1Addr? 0x4: 0x0;
      bin_param1 |= is1Data? 0x2: 0x0;
      bin_param1 |= is1Float? 0x1: 0x0;

      testTypeTags (bin_param1, bin_param1_alkuperainen, is1Float, is1Data, is1Addr);
      r.success = true;
      r.Result = bin_param1;
    }
    else {
      float f_param1 = toFloat(param1),
	f_param1_original = f_param1;

      value converter;
      converter.float_val = f_param1;
      long bin_param1 = converter.int_val;
            
      bin_param1 = bin_param1 << 3;
      bin_param1 |= is1Addr? 0x4: 0x0;
      bin_param1 |= is1Data? 0x2: 0x0;
      bin_param1 |= is1Float? 0x1: 0x0;

      assert(bin_param1 >> 3 == converter.int_val);

      testTypeTags(bin_param1, f_param1_original, is1Float, is1Data, is1Addr);

      r.success = true;
      r.Result = bin_param1;
    }
  }
  else if(regex_match(param1.c_str(), regex(symbol_regex))) {

    if(label_internment.find(param1) != label_internment.end()) {
      printf("Found symbol %s's value %lu\n", param1.c_str(), label_internment.at(param1));
      r.Result = label_internment.at(param1);
      r.success = true;
    }
    else {
      long val = label_internment.size() + 1;
      printf("Created a new value for %s: %lu\n", param1.c_str(), val);
      r.Result = val;
      label_internment[param1] = val;
      r.success = true;
    }
  }
  else if(param1 == "RET") {
    long l = -666 << 3;
    l |= 0x4;
    r.success = true;
    r.Result = l;
    
  }
  else {
    r.success = false;
  }
  return r;
}

struct prog_data_loc { vector<string>::const_iterator prog_location; vector<string>::const_iterator data_location; };

prog_data_loc getSectionLocations(vector<string> &lines) {
  return {std::find_if(lines.begin(), lines.end(), [](string line) { return trim(line) == ".prog"; }),
      std::find_if(lines.begin(), lines.end(), [](string line) { return trim(line) == ".data"; })};
}

bool isDigit(char c)
{
  bool result = c == '0' ||
    c == '1' ||
    c == '2' ||
    c == '3' ||
    c == '4' ||
    c == '5' ||
    c == '6' ||
    c == '7' ||
    c == '8' ||
    c == '9' ||
    c == '.';
  return result;
}

vector<Either3v<result<string>, result<float>, result<int>>> parseDataSection(string& data) {
  bool inString = false,
    inDigit = false;
  int lastTokenEnd = 0;
  vector<Either3v<result<string>, result<float>, result<int>>> toret;
  
  for(unsigned int i = 0; i<data.size(); i++) {
    string token = ""+data.at(i);
    if(!inString && !inDigit) {
      inDigit = isDigit(data.at(i));
      inString = data.at(i) == '"';
      if(inDigit || inString) lastTokenEnd = i + 1;
    }
    else if(inDigit) {
      inDigit = isDigit(data.at(i)) && i < data.size()-1;
      if (!inDigit) {
	// In between lastTokenEnd and i is a whole float or an int
	int len = (i+1) - lastTokenEnd;
	string substr = data.substr(lastTokenEnd - 1, len);

	bool is_float = substr.find(".") != string::npos;
	Either3v<result<string>, result<float>, result<int>> e;
	if(is_float) {
	  float rr = toFloat(substr);
	  result<float> r;
	  r.success = true;
	  r.Result = rr;
	  e.b = r;
	}
	else {
	  result<int> r;
	  r.success = true;
	  r.Result = toInt(substr);
	  e.c = r;
	}
	toret.push_back(e);
      }
    }
    else if(inString) {
      do { i++; } while(i<data.size() && data.at(i) != '"');
      inString = false;
      int len = i - lastTokenEnd;
      string substr = data.substr(lastTokenEnd, len);

      Either3v<result<string>, result<float>, result<int>> e;
      result<string> r;
      r.success = true;
      r.Result = substr.c_str();
      e.a = r;
      toret.push_back(e);
    }
  }

  return toret;
}

short containerSize(Either3v<result<string>, result<float>, result<int>> element) {
  if(element.a.success)
    return element.a.Result.length() * sizeof(char);
  else if(element.b.success)
    return sizeof(float);
  else
    return sizeof(int);
}

short dataBufferSize(vector<Either3v<result<string>, result<float>, result<int>>> &vec) {
  int dataLength = sizeof(short);
  vector<short> sizes(vec.size());
  transform(vec.begin(), vec.end(), sizes.begin(), [](Either3v<result<string>, result<float>, result<int>> element) {
      return containerSize(element);
    });
  int amountOfElementLens = vec.size() * sizeof(short);
  int amountOfTypeTags = vec.size() * sizeof(char);
  return dataLength + std::accumulate(sizes.begin(), sizes.end(), 0) + amountOfElementLens + amountOfTypeTags;
}

int copyToBuffer(void* buffer, Either3v<result<string>, result<float>, result<int>> e)
{
  result<string> rs = e.a;
  result<float> rf = e.b;
  result<int> ri = e.c;

  if(rs.success) {
    string& s = rs.Result;
    const char* cs = s.c_str();
    size_t bytes = s.size() * sizeof(char);
    memcpy(buffer, cs, bytes);
    return bytes;
  }
  else if (rf.success) {
    float *b = (float*) buffer;
    *b = rf.Result;
    return sizeof(float);
  }

  int *i = (int*)buffer;
  *i = ri.Result;
  return sizeof(int);
}

unsigned char getTypeTag(Either3v<result<string>, result<float>, result<int>> e) {
  result<string> rs = e.a;
  result<float> rf = e.b;

  if(rs.success) {
    return 3;
  }
  else if (rf.success) {
    return 2;
  }
  return 1;
}

void zeroMem(char *buffer, int N) {
  for (int i = 0; i < N; i++) buffer[i] = 0;
}

void generate_code(vector<string> lines, const char *outputFilename) {
  FILE *f = fopen(outputFilename, "w");
  if (!f) {
    printf("Opening %s for output failed\n", outputFilename);
    return;
  }

  auto iterators = getSectionLocations(lines);
  string data_list = "";

  for(auto line = iterators.data_location + 1; line != lines.end() && line != iterators.prog_location; ++line) {
    data_list += *line;
  }

  printf("data_list: %s\n", data_list.c_str());

  assert(sizeof(short) == 2);

  vector<Either3v<result<string>, result<float>, result<int>>> dataSection = parseDataSection(data_list);
  short dataCount = dataSection.size();
  int ioBufferSize = dataBufferSize(dataSection);
  char ioBuffer[ioBufferSize];
  zeroMem(ioBuffer, ioBufferSize);
  *ioBuffer = dataCount;
  printf("Datacount is %d\n", dataCount);
  int pointer = 2;
  auto dataIterator = dataSection.begin();
  do {
    Either3v<result<string>, result<float>, result<int>> e = *dataIterator;
    *(ioBuffer + pointer) = getTypeTag(e);
    pointer += 1;
    short len = containerSize(e);
    // printf("Length is %d\n", len);
    *(ioBuffer + pointer) = len;
    pointer += 2;
    int copiedAmount = copyToBuffer(ioBuffer + pointer, e);
    pointer += copiedAmount;
    ++dataIterator;
  } while(pointer < ioBufferSize && dataIterator != dataSection.end());
  
  fwrite(ioBuffer, ioBufferSize, 1, f);
  
  for(auto line = iterators.prog_location+1; line != lines.end() && line != iterators.data_location; ++line) {
    string str = *line;
    vector<string> components = resplit(trim(str), "[\\s|,]+");
    
    // split(str, ' ', components);
   
    string opcode = toUpper(trim(components.at(0)));
    if(opcode == "" || opcode == ";;") continue;
    printf("Opcode %s\n", opcode.c_str());

    result<opcodes> bin_opcode = char_to_opcode(opcode.c_str());

    if(!bin_opcode.success) { printf("Unknown opcode (%s) found\n", opcode.c_str()); throw ""; }
    printf("Hexopcode %x\n", bin_opcode.Result);

    int buffer_size = sizeof(char)+2 * sizeof(long);
    char *buffer = new char[buffer_size];
    buffer[0] = bin_opcode.Result;
    
    if(components.size()>1) {
      vector<string> parameters = components;
      parameters.erase(parameters.begin());

      int param_count = parameters.size();
      if(param_count > 0) {
	printf("Param1 is %s\n", parameters.at(0).c_str());
	result<long> r = makeFileParam(parameters.at(0));
	if(r.success) {
	  long param1 = r.Result;
	  buffer[1] = param1;
	  if(param_count > 1) {
	    printf("Param2 is %s\n", parameters.at(1).c_str());
	    r = makeFileParam(parameters.at(1));
	    if(r.success) {
	      long param2 = r.Result;
	      buffer[1 + sizeof(long)] = param2;
	    }
	    else {
	      // printf("Extracting second param, %s, failed\n", parameters.at(1).c_str());
	      buffer[1 + sizeof(long)] = 0;
	    }
	  }
	  else {
	    // puts("Param_count<1");
	    buffer[1 + sizeof(long)] = 0;
	  }
	}
	else {
	  // printf("Extracting first param, %s, failed\n", parameters.at(0).c_str());
	  buffer[1] = 0L;
	  buffer[1 + sizeof(long)] = 0L;
	}
      }
      else {
	// puts("Param_count < 0");
	buffer[1] = 0L;
	buffer[1 + sizeof(long)] = 0L;
      }
    }
    else {
      // puts("Parameterless instruction");
      buffer[1] = 0L;
      buffer[1 + sizeof(long)] = 0L;
    }

    fwrite(buffer, buffer_size, 1, f);
  }

  fclose(f);
}
