#include <opcodes.h>
#include <assembler.h>
#include <lambda.h>

#include <cassert>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
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

// Either<value_container, pointer_container> generateContainer(float Value) {
//   Either<value_container, pointer_container> e;
//   int meta_bits = Value & 07;
//   float real_value = Value >> 3;
//   bool isAddr = meta_bits & 04,
//     isData = meta_bits & 02,
//     isFloat = meta_bits & 01;
    
//   assert(!(isData || isAddr));
  
//   value vvv;
//   if(isFloat) {
//     vvv.float_val = (float)(Value >> 3);
//   }
//   else vvv.int_val = real_value;
//   value_container *v = new value_container(vvv, isFloat? FLOAT:INT);
//   e.a = v;

//   return e;
// }

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

// isAddr ja isData ei voi olla yhtä aikaa true
// ja jos jompikumpi kahdesta ekasta on tosi, osoitin ei voi olla float mutta ARVO VOI OLLA
// unsigned int typeMask(bool isAddr, bool isData, bool isFloat)
// {
//   static unordered_map<bool, unordered_map<bool, unordered_map<bool, int>>> taulu =
//     {{false, {{false, {{false, 0},
// 		       {true, 1}}},
// 	      {true, {{false, 2},
// 		      {true, 3}}}}},
//      {true, {{false, {{false, 4},
// 		      {true, 5}}}}}};
//   unsigned int sizeOf = sizeof(int);
//   printf("sizeof = %d\n", sizeOf);
//   return (taulu.at(isAddr).at(isData).at(isFloat)) << ((sizeOf * 32)-3);
// }

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

result<long> makeFileParam(string& p) {
  string param1 = trim(p);
  result<long> r;
  bool is1Addr = param1.c_str()[0] == '\'',
    is1Data = param1.c_str()[0] == '!',
    is1Float = param1.find(".") != string::npos;

  const char* reg = "['!]?(\\d+\\.?\\d*)";
  regex addr_to_value(reg);

  if(!regex_match(param1.c_str(), addr_to_value)) {
    r.success = false;
    return r;
  }
    
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
    r.result = bin_param1;
    return r;
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
    r.result = bin_param1;
    return r;
  }
}

void generate_code(vector<string> lines, const char *outputFilename) {
  FILE *f = fopen(outputFilename, "w");
  if (!f) {
    printf("Opening %s for output failed\n", outputFilename);
    return;
  }

  for(auto line = lines.begin(); line < lines.end(); ++line) {
    string str = *line;
    vector<string> components;
    split(str, ' ', components);
    if(!components.size()) continue;

    string opcode = toUpper(trim(components.at(0)));
    printf("Opcode %s\n", opcode.c_str());

    result<opcodes> bin_opcode = char_to_opcode(opcode.c_str());

    if(!bin_opcode.success) { throw "Unknown opcode ("+opcode+") found"; }
    printf("opcode %x\n", bin_opcode.result);
    
    vector<string> parameters;
    split(trim(components.at(1)), ',', parameters);

    int param_count = parameters.size(),
      buffer_size = sizeof(char)+param_count * sizeof(long);
    char *buffer = new char[buffer_size];
    buffer[0] = bin_opcode.result;
    if(param_count > 0) {
      result<long> r = makeFileParam(parameters.at(0));
      if(r.success) {
	long param1 = r.result;
	buffer[1] = param1;
	if(param_count > 1) {
	  r = makeFileParam(parameters.at(1));
	  if(r.success) {
	    long param2 = r.result;
	    buffer[1 + sizeof(long)] = param2;
	  }
	}
      }
    }

    fwrite(buffer, buffer_size, 1, f);
  }

  fclose(f);
}

  
