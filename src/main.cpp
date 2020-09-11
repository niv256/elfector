#include "elf_handler.hpp"
#include "inject.hpp"
#include <iostream>

using namespace injector;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: elfector [binary]" << std::endl;
    return 1;
  }

  try {
    Elf_target target {std::string { argv[1] } };
    inject(target);

    std::cout << "[+] injection successfull." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "[-] error detected: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "[-] unknown error." << std::endl;
  }
}
