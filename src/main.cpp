
#include "injector.hpp"
#include <iostream>

using namespace std;

vector<unsigned char> shellcode{
    0xb8, 0x40, 0x10, 0x40, 0x00, // mov eax, 0x401040
    0xff, 0xe0                    // jmp eax
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "wrong arguments" << endl;
    return 1;
  }

  Elf_target target{string{argv[1]}};

  target.load_headers();
  if (!target.is_elf()) {
    cout << "file is not elf" << endl;
    return 2;
  }

  const Elf_target::code_cave_t code_cave = target.find_biggest_code_cave();
  if (!target.write_shellcode(shellcode, code_cave)) {
    cout << "can't find a big enough code cave" << endl;
    return 3;
  }
  target.change_entry_point(0x400000 + code_cave.offset);
}
