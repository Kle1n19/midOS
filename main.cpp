#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

bool is_executable(const std::string& filepath) {
    if (access(filepath.c_str(), X_OK) != 0) {
        std::cout << "not executable" << std::endl;
    } else {
        std::cout << "executable" << std::endl;
    }

    return 0;
}

std::string determine_file_type(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return "Unknown";
    }

    // Read the first few bytes to identify the file type
    char buffer[4];
    file.read(buffer, sizeof(buffer));

    // Check if the file is an ELF file (magic number 0x7F 'E' 'L' 'F')
    if (buffer[0] == 0x7F && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
        return "ELF executable";
    }

    // Reset file to read shebang line
    file.seekg(0, std::ios::beg);
    std::string line;
    if (std::getline(file, line) && line.rfind("#!", 0) == 0) {
        if (line.find("bash") != std::string::npos) {
            return "Bash script";
        } else if (line.find("python") != std::string::npos) {
            return "Python script";
        } else if (line.find("lua") != std::string::npos) {
            return "Lua script";
        } else {
            return "Script: " + line.substr(2);
        }
    }

    return "Unknown or other file type";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];

    // Check if the file is executable
    if (!is_executable(filepath)) {
        std::cout << "The file is not executable." << std::endl;
        return 0;
    }

    // Determine and print the file type
    std::string file_type = determine_file_type(filepath);
    std::cout << "The file is executable and is identified as: " << file_type << std::endl;

    return 0;
}
