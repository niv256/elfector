#pragma once

#include <cstdint>
#include <vector>

namespace shellcode {

using shellcode_t = std::vector<uint8_t>;

shellcode_t make_shellcode(int entry_offset);

}
