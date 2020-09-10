#include "shellcode.hpp"
#include <algorithm>
#include <array>
#include <fstream>

using namespace std;

const string shellcode_file_name{"../shellcode/shellcode"};

namespace shellcode {

shellcode_t make_shellcode(int entry_offset) {
  size_t deadbeef_offset;

  // read file into shellcode
  ifstream shellcode_file{shellcode_file_name, std::ios_base::binary};

  shellcode_t shellcode{istreambuf_iterator<char>(shellcode_file),
                        istreambuf_iterator<char>()};

  // search for 0xdeadbeef inside the shellcode
  shellcode_t deadbeef{0xef, 0xbe, 0xad, 0xde};

  auto it = search(shellcode.begin(), shellcode.end(), deadbeef.begin(),
                   deadbeef.end());

  if (it != shellcode.end()) {
    deadbeef_offset = it - shellcode.begin();
  } else {
    throw runtime_error{"0xdeadfeed not found in shellcode"};
  }

  *reinterpret_cast<int *>(&shellcode[deadbeef_offset]) = entry_offset;

  return shellcode;
}

} // namespace shellcode
