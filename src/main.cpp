
#include "injector.hpp"
#include <iostream>

using namespace std;

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

  Elf_target::code_cave_t code_cave = target.find_biggest_code_cave();
  cout << "offset: 0x" << hex << code_cave.offset << endl;
  cout << "size: 0x" << hex << code_cave.size << endl;

  cout << "executed" << endl;
}
