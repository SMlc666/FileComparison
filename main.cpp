#include <iostream>
#include <memory>
#include <sys/sysinfo.h>
#include "File.hpp"

int main(int argc, char* argv[]){
    if (argc != 4) {
        std::cerr << "Usage: " << " <Comparison program> <file1>" << " <Comparison program> <file2>" <<  "<Output file>" << std::endl;
        return 1;
    }
    std::unique_ptr<File> file1, file2, output;
    try {
        file1 = std::make_unique<File>(std::string(argv[1]) , std::ios::in);
        file2 = std::make_unique<File>(std::string(argv[2]) , std::ios::in);
        output = std::make_unique<File>(std::string(argv[3]), std::ios::out | std::ios::trunc);
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        std::cerr << "Error: Failed to get system information" << std::endl;
        return 1;
    }
    long free_ram = info.freeram;
    long usable_ram = free_ram - free_ram / 10;
    
    std::cout << "Hello, from FileComparison!\n";
}
