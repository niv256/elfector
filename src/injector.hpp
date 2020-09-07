#include <elf.h>
#include <fcntl.h>
#include <string>
#include <vector>

#pragma once

using namespace std;

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

  bool is_elf(void);

  struct code_cave_t {
    size_t offset;
    size_t size;
  };

  code_cave_t find_biggest_code_cave(void);

  bool write_shellcode(vector<unsigned char> &shellcode,
                       const code_cave_t &code_cave);

  void change_entry_point(uint64_t new_entry_point);
};
