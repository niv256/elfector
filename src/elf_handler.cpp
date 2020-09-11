#include "elf_handler.hpp"
#include <algorithm>
#include <elf.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using namespace shellcode;

Elf_target::Elf_target(const string name) {
  fd = open(name.c_str(), O_RDWR);
  if (fd == -1) {
    throw invalid_argument{"can't open file"};
  }
}

Elf_target::~Elf_target() { close(fd); }

void Elf_target::load_headers() {
  Elf_target::load_main_header();
  Elf_target::load_program_headers(program_headers,
                                   static_cast<size_t>(header.e_phnum));
  Elf_target::load_program_headers(section_headers,
                                   static_cast<size_t>(header.e_shnum));
}

void Elf_target::load_main_header() {
  read(fd, static_cast<void *>(&header), sizeof(Elf64_Ehdr));
}

template <typename T>
void Elf_target::load_program_headers(vector<T> &vector, size_t count) {
  for (size_t i{}; i < count; i++) {
    vector.push_back(get_header_type<T>());
  }
}

template <typename T> T Elf_target::get_header_type() const {
  T object{};
  read(fd, &object, sizeof(object));
  return object;
}

bool Elf_target::is_elf(void) const {
  bool elf = true;
  elf &= header.e_ident[0] == ELFMAG0;
  elf &= header.e_ident[1] == ELFMAG1;
  elf &= header.e_ident[2] == ELFMAG2;
  elf &= header.e_ident[3] == ELFMAG3;
  return elf;
}

const Elf_target::code_cave_t Elf_target::find_biggest_code_cave(void) const {
  lseek(fd, 0, SEEK_SET);
  size_t biggest_offset{0};
  size_t biggest_found{0};
  size_t cur_cave{0};
  char byte{0};

  while (read(fd, &byte, 1)) {
    if (byte == '\0') {
      cur_cave++;
    } else {
      if (cur_cave > biggest_found) {
        biggest_found = cur_cave;
        biggest_offset = lseek(fd, 0, SEEK_CUR) - cur_cave;
      }
      cur_cave = 0;
    }
  }

  return {biggest_offset, biggest_found};
}

void Elf_target::write_shellcode(const shellcode_t &shellcode,
                                 const code_cave_t &code_cave) const {
  lseek(fd, code_cave.offset, SEEK_SET);
  write(fd, shellcode.data(), shellcode.size());
}

uint64_t Elf_target::entry_point(void) { return header.e_entry; }

void Elf_target::change_entry_point(uint64_t new_entry_point) {
  header.e_entry = new_entry_point;
  lseek(fd, 0, SEEK_SET);
  write(fd, &header, sizeof(header));
}

size_t Elf_target::get_text_segment_offset(void) const {
  for (auto ph : program_headers) {
    if (ph.p_type == PT_LOAD) {
      return ph.p_vaddr;
    }
  }
  throw runtime_error{
      "could not find a PT_LOAD type program header.\nIs it an executable?"};
}

bool Elf_target::set_executable(uint64_t vaddr) {
  auto ph{find_if(program_headers.begin(), program_headers.end(),
                  [=](Elf64_Phdr ph) { return ph.p_vaddr >= vaddr; })};
  if (ph == program_headers.end()) {
    return false;
  }

  // make executable
  ph->p_flags |= PF_X;

  // write the program header back into the file
  auto ph_index{static_cast<size_t>(ph - program_headers.begin())};
  auto file_offset{sizeof(Elf64_Ehdr) + ph_index * sizeof(Elf64_Phdr)};
  lseek(fd, file_offset, SEEK_SET);
  write(fd, &(*ph), sizeof(Elf64_Phdr));

  return true;
}
