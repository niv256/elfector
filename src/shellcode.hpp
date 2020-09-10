#pragma once

#include <cstdint>
#include <vector>

using namespace std;

namespace shellcode {

using shellcode_t = vector<uint8_t>;

shellcode_t make_shellcode(int entry_offset);

} // namespace shellcode
