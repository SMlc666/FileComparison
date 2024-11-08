#include <iostream>
#include "File.hpp"
int main(int argc, char* argv[]){
    if (argc != 4) {
        std::cerr << "Usage: " << " <Comparison program> <file1>" << " <Comparison program> <file2>" <<  "<Output file>" << std::endl;
        return 1;
    }
    try {
        File file1{std::string(argv[1]) , std::ios::in};
        File file2{std::string(argv[2]) , std::ios::in};
        File output(std::string(argv[3]), std::ios::out | std::ios::trunc);
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } 
    std::cout << "Hello, from FileComparison!\n";
}
