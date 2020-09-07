#include <elf.h>
#include <string>
#include <vector>

#pragma once

class Elf_target {
  int fd;
  Elf64_Ehdr header;
  std::vector<Elf64_Phdr> program_headers;
  std::vector<Elf64_Shdr> section_headers;

public:
  Elf_target(std::string name);
  Elf_target(const Elf_target &) = delete;

  Elf_target &operator=(const Elf_target &) = delete;

  ~Elf_target();

  void load_headers();

  void load_main_header();

  template <typename T>
  void load_program_headers(std::vector<T> &vector, int count);

  template <typename T> T get_header_type();
};
