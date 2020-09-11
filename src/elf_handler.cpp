#include "elf_handler.hpp"
#include <algorithm>
#include <elf.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace shellcode;

Elf_target::Elf_target(const char *name)
  : m_fd { open(name, O_RDWR) } {
  if (m_fd == -1) {
    throw std::invalid_argument { "can't open file" };
  }

  load_headers();

  if (!is_elf()) {
    throw std::invalid_argument { "file is not elf" };
  }
}

Elf_target::~Elf_target() {
  close(m_fd);
}

uint64_t Elf_target::entry_point(void) const {
  return header.e_entry ;
}

void Elf_target::change_entry_point(uint64_t new_entry_point) {
  header.e_entry = new_entry_point;
  lseek(m_fd, 0, SEEK_SET);
  write(m_fd, &header, sizeof(header));
}

size_t Elf_target::get_text_segment_offset(void) const {
  for (const auto& ph : m_program_headers) {
    if (ph.p_type == PT_LOAD) {
      // first PT_LOAD program header
      return ph.p_vaddr;
    }
  }
  throw std::runtime_error{
      "could not find a PT_LOAD type program header.\nIs it an executable?"};
}

void Elf_target::set_executable(uint64_t vaddr) {
  auto ph {find_if(m_program_headers.begin(),
           m_program_headers.end(),
           [=] (Elf64_Phdr ph) {
               return ph.p_vaddr >= vaddr;
             }
           )
          };
  if (ph == m_program_headers.end()) {
    throw std::runtime_error { "idk but you should probably check this" };
  }

  // make executable
  ph->p_flags |= PF_X;

  // write the program header back into the file
  auto ph_index {static_cast<size_t>(ph - m_program_headers.begin()) };
  auto file_offset {sizeof(Elf64_Ehdr) + ph_index * sizeof(Elf64_Phdr) };
  lseek(m_fd, file_offset, SEEK_SET);
  write(m_fd, &(*ph), sizeof(Elf64_Phdr));
}

void Elf_target::load_headers() {
  load_main_header();
  load_headers(m_program_headers, static_cast<size_t>(header.e_phnum));
  load_headers(m_section_headers, static_cast<size_t>(header.e_shnum));
}

void Elf_target::load_main_header() {
  read(m_fd, static_cast<void*>(&header), sizeof(Elf64_Ehdr));
}

template <typename T>
void Elf_target::load_headers(std::vector<T>& vector, size_t count) {
  for (size_t i {}; i < count; i++) {
    vector.push_back(get_header_type<T>());
  }
}

template <typename T> T Elf_target::get_header_type() const {
  T object {};
  read(m_fd, &object, sizeof(object));
  return object;
}

bool Elf_target::is_elf(void) const {
  return header.e_ident[0] == ELFMAG0 &&
         header.e_ident[1] == ELFMAG1 &&
         header.e_ident[2] == ELFMAG2 &&
         header.e_ident[3] == ELFMAG3;
}

const Elf_target::code_cave_t Elf_target::find_biggest_code_cave(void) const {
  size_t biggest_offset { 0 };
  size_t biggest_found { 0 };
  size_t cur_cave { 0 };
  char byte;

  /*
   * read contents of binary from start of file byte by byte.
   * use cur_cave as the size in bytes of current code cave.
   * if byte read is a null-byte, incrase the cur_cave counter.
   * else, update biggest size found and its offset if needed,
   * and reset cur_cave counter.
   */
  lseek(m_fd, 0, SEEK_SET);
  while (read(m_fd, &byte, 1)) {
    if (byte == '\0') {
      cur_cave++;
    } else {
      if (cur_cave > biggest_found) {
        biggest_found = cur_cave;
        biggest_offset = lseek(m_fd, 0, SEEK_CUR) - cur_cave;
      }
      cur_cave = 0;
    }
  }

  return {biggest_offset, biggest_found};
}

void Elf_target::write_shellcode(const shellcode_t &shellcode,
                                 const code_cave_t &code_cave) {
  lseek(m_fd, code_cave.offset, SEEK_SET);
  write(m_fd, shellcode.data(), shellcode.size());
}
