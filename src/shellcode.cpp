#include "shellcode.hpp"
#include <algorithm>
#include <array>
#include <fstream>

const std::string shellcode_file_name { "../shellcode/shellcode" };

namespace shellcode {

shellcode_t make_shellcode(int entry_offset) {
  // read file into shellcode
  std::ifstream shellcode_file{shellcode_file_name, std::ios_base::binary};

  shellcode_t shellcode { std::istreambuf_iterator<char>(shellcode_file),
                          std::istreambuf_iterator<char>() };

  // search for 0xdeadbeef inside the shellcode
  shellcode_t deadbeef{0xef, 0xbe, 0xad, 0xde};

  const auto it = search(shellcode.begin(), shellcode.end(),
                         deadbeef.begin(), deadbeef.end());

  if (it == shellcode.end()) {
    throw std::runtime_error { "0xdeadfeed not found in shellcode" };
  }

  const auto deadbeef_offset { it - shellcode.begin() };

  // patch the new entry address into the shellcode instead of deadbeef
  *reinterpret_cast<int*>(&shellcode[deadbeef_offset]) = entry_offset;

  return shellcode;
}

}
