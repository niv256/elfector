
#include <iostream>
#include "injector.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "wrong arguments" << std::endl;
		return 1;
	}

	Elf_target target{ std::string{ argv[1] } };
	target.load_headers();
	std::cout << "executed" << std::endl;
}
