#include "shellcode.hpp"

using namespace std;

namespace shellcode {

shellcode_t make_shellcode(void) {
  shellcode_t shellcode{
      0xb8, 0x40, 0x10, 0x40, 0x00, // mov eax, 0x401040
      0xff, 0xe0                    // jmp eax
  };

  return shellcode;
}

} // namespace shellcode
