#pragma once

#include "shellcode.hpp"
#include <elf.h>
#include <fcntl.h>
#include <string>
#include <vector>

using namespace shellcode;

class Elf_target final {
  int m_fd;
  Elf64_Ehdr header;
  std::vector<Elf64_Phdr> m_program_headers;
  std::vector<Elf64_Shdr> m_section_headers;

public:
  Elf_target(const char *name);
  Elf_target(const Elf_target &) =delete;

  Elf_target& operator=(const Elf_target&) =delete;

  ~Elf_target();

  struct code_cave_t {
    size_t offset;
    size_t size;
  };

  const code_cave_t find_biggest_code_cave(void) const;

  void write_shellcode(const shellcode_t &shellcode,
                       const code_cave_t &code_cave);

  uint64_t entry_point(void) const ;

  void change_entry_point(uint64_t new_entry_point);

  size_t get_text_segment_offset(void) const;

  void set_executable(uint64_t vaddr);

private:
  void load_headers();

  void load_main_header();

  template <typename T>
  void load_headers(std::vector<T> &vector, size_t count);

  template <typename T>
  T get_header_type() const;

  bool is_elf(void) const;
};
