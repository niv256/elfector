#pragma once

#include "elf_handler.hpp"
#include "shellcode.hpp"

namespace injector {

void inject(Elf_target& elf);

}
