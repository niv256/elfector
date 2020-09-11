#pragma once

#include "elf_handler.hpp"
#include "shellcode.hpp"

using namespace std;

namespace injector {

void inject(Elf_target& elf);

}
