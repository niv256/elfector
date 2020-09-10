#include "shellcode.hpp"
#include <fstream>

using namespace std;

const string shellcode_file_name{"../shellcode/shellcode"};

namespace shellcode {

shellcode_t make_shellcode(int entry_offset) {
  // read file into shellcode
  ifstream shellcode_file{shellcode_file_name, std::ios_base::binary};

  shellcode_t shellcode{istreambuf_iterator<char>(shellcode_file),
                        istreambuf_iterator<char>()};

  // 0x33 hard coded 0xdeadbeef offset
  *reinterpret_cast<int *>(&shellcode[0x33]) = entry_offset;

  return shellcode;
}

} // namespace shellcode
