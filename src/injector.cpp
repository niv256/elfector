#include "injector.hpp"
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Elf_target::Elf_target(std::string name) { fd = open(name.c_str(), O_RDWR); }

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
void Elf_target::load_program_headers(std::vector<T> &vector, int count) {
  for (size_t i{}; i < count; i++) {
    vector.push_back(get_header_type<T>());
  }
}

template <typename T> T Elf_target::get_header_type() {
  T object{};
  read(fd, &object, sizeof(object));
  return object;
}
