#include "shellcode.hpp"
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace shellcode {

shellcode_t make_shellcode(int entry_offset) {
  // TODO: this very no good
  shellcode_t shellcode{};

  int fd = open("../shellcode/shellcode", O_RDONLY);
  uint8_t read_byte;
  while (read(fd, &read_byte, 1)) {
    shellcode.push_back(read_byte);
  }
  close(fd);

  *reinterpret_cast<int *>(&shellcode[0x33]) = entry_offset;

  return shellcode;
}

} // namespace shellcode
