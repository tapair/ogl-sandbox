#include <iostream>
#include <filesystem>

int main() {
	std::cout << "Current directory: " << std::filesystem::current_path() << "\n";

}