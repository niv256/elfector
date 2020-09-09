#include "elf_handler.hpp"
#include "inject.hpp"
#include <iostream>

using namespace std;
using namespace injector;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: elfector [binary]" << endl;
    return 1;
  }

  try {
    Elf_target target{string{argv[1]}};
    inject(target);

    cout << "[+] injection succesfull." << endl;
  } catch (const exception &e) {
    cout << "[-] erorr detected: " << e.what() << endl;
  } catch (...) {
    cout << "[-] unknown error." << endl;
  }
}
