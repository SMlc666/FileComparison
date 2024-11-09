#include <iostream>
#include <memory>
#include "File.hpp"
#include <algorithm>
#include <thread>
#include <mutex>
typedef struct {
    int start;
    int end;
} Line_Range;
std::mutex mtx;
std::vector<std::pair<int, int>> split(int line_count, int split_count) {
    if (split_count <= 0) {
        throw std::invalid_argument("Split count must be greater than zero.");
    }
    if (line_count < 0) {
        throw std::invalid_argument("Line count must be non-negative.");
    }
    if (split_count > line_count) {
        throw std::invalid_argument("Split count must be less than or equal to line count.");
    }

    std::vector<std::pair<int, int>> result;
    result.reserve(split_count); // 预分配内存以提高效率

    int line_per_split = line_count / split_count;
    int remainder = line_count % split_count;

    for (int i = 0; i < split_count; i++) {
        int start = (i == 0) ? 1 : i * line_per_split + std::min(i, remainder);
        int end = std::min(start + line_per_split + (i < remainder ? 1 : 0), line_count);
        result.emplace_back(start, end); // 使用 emplace_back 提高效率
    }

    return result;
}

std::vector<std::string> compare(const File *file1,const File *file2,Line_Range file1_lines,Line_Range file2_lines){
    std::vector<std::string> result;
    std::vector<std::string> lines1, lines2;
    file1->getLines(file1_lines.start,file1_lines.end,lines1);
    file2->getLines(file2_lines.start,file2_lines.end,lines2);
    std::for_each(lines1.begin(), lines1.end(), [&result, &lines2](const std::string& line) {
        std::for_each(lines2.begin(), lines2.end(), [&result, &line](const std::string& line2) {
            if (line == line2) {
                result.push_back(line);
            }
        });
    });
    return result;
}

int main(int argc, char* argv[]){
    if (argc != 5) {
        std::cerr << "Usage: " << " <Comparison program> <file1>" << " <Comparison program> <file2>" <<  " <Output file>" << " <Split count>" << std::endl;
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
    std::cout << "file1 Size:" << file1->getFileSize() << " bytes" << std::endl;
    std::cout << "file2 Size:" << file2->getFileSize() << " bytes" << std::endl;
    std::cout << "file1 Line Count:" << file1->getLineCount() << std::endl;
    std::cout << "file2 Line Count:" << file2->getLineCount() << std::endl;
    int split_count;
    try {
        split_count = std::stoi(argv[4]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    std::vector<std::pair<int, int>> file1_split, file2_split;
    try {
        file1_split = split(file1->getLineCount(), split_count);
        file2_split = split(file2->getLineCount(), split_count);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::vector<std::thread> threads;
    std::vector<std::string> result;
    for (int i = 0; i < split_count; i++) {
        Line_Range file1_range = {file1_split[i].first, file1_split[i].second};
        Line_Range file2_range = {file2_split[i].first, file2_split[i].second};
        threads.emplace_back([&result, &file1, &file2, &file1_range, &file2_range]() {
            std::vector<std::string> temp_result = compare(file1.get(), file2.get(), file1_range, file2_range);
            mtx.lock();
            std::copy(temp_result.begin(), temp_result.end(), std::back_inserter(result));
            mtx.unlock();
        });
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::for_each(result.begin(), result.end(), [&output](const std::string& line) {
        output->getFileStream() << line << std::endl;
    });// 输出结果到文件
    std::cout << "Hello, from FileComparison!\n";
}
