#include "File.hpp"

File::File(const std::string filename , const std::ios_base::openmode mode) : m_filename(filename), m_mode(mode), m_filestream(filename, mode) {
    if (m_filestream.fail()){
        throw std::ios_base::failure("Failed to open file " + filename);
    }
    if (has_mode(std::ios::in)){
        m_filestream.seekg(0, std::ios_base::end);
        file_size = m_filestream.tellg();
        m_filestream.seekg(0, std::ios_base::beg);
        std::string temp;
        while (std::getline(m_filestream, temp)){
            line_count++;
        }
        if (!has_mode(std::ios::ate)){
            m_filestream.seekg(0, std::ios_base::beg);
        }
        m_filestream.clear();
    }
}
File::~File(){
    m_filestream.close();
}
bool File::has_mode(std::ios_base::openmode mode) const{
    if (m_mode & mode){
        return true;
    }else{
        return false;
    }
}
void File::getLines(int start, int end,std::vector<std::string>& lines) const{
    if (has_mode(std::ios::in)){
        if (start < 0 || end > line_count){
            throw std::out_of_range("Invalid line range.");
        }
        double current_pos = m_filestream.tellg();
        m_filestream.seekg(0, std::ios_base::beg);
        std::string temp;
        for (int i = 1; i < start; i++){
            std::getline(m_filestream, temp);
        }
        //完成循环后，文件指针已经指向了start行的开头
        for (int i = start; i <= end; i++){
            std::getline(m_filestream, temp);
            lines.push_back(temp);
        }
        m_filestream.seekg(current_pos, std::ios_base::beg);//恢复文件指针
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
double File::getLinesSize(int start, int end) const{
    if (has_mode(std::ios::in)){
        if (start < 0 || end > line_count){
            throw std::out_of_range("Invalid line range.");
        }
        double current_pos = m_filestream.tellg();
        double target_start_pos = 0;
        double target_end_pos = 0;
        m_filestream.seekg(0, std::ios_base::beg);
        for (int i = 1; i < start; i++){
            std::string temp;
            std::getline(m_filestream, temp);
        }
        target_start_pos = m_filestream.tellg();
        //完成循环后，文件指针已经指向了start行的开头
        for (int i = start; i <= end; i++){
            std::string temp;
            std::getline(m_filestream, temp);
        }
        //完成循环后，文件指针已经指向了end行的结尾
        target_end_pos = m_filestream.tellg();
        m_filestream.seekg(current_pos, std::ios_base::beg);//恢复文件指针
        double size = target_end_pos - target_start_pos;
        return size;
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
void File::getLine(int line_number, std::string& line) const{
    if (has_mode(std::ios::in)){
        if (line_number < 0 || line_number >= line_count){
            throw std::out_of_range("Invalid line number.");
        }
        double current_pos = m_filestream.tellg();
        m_filestream.seekg(0, std::ios_base::beg);
        for (int i = 1; i < line_number; i++){
            if (i == line_number){
                std::getline(m_filestream, line);
            }
        }
        m_filestream.seekg(current_pos, std::ios_base::beg);
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
double File::getLineSize(int line_number) const{
    if (has_mode(std::ios::in)){
        if (line_number < 0 || line_number >= line_count){
            throw std::out_of_range("Invalid line number.");
        }
        double current_pos = m_filestream.tellg();
        double target_line_pos = 0;
        m_filestream.seekg(0, std::ios_base::beg);
        for (int i = 1; i < line_number; i++){
            std::string temp;
            if (i == line_number){
                target_line_pos = m_filestream.tellg();
                std::getline(m_filestream, temp);
            }
        }
        m_filestream.seekg(current_pos, std::ios_base::beg);
        std::string temp;
        std::getline(m_filestream, temp);
        double size = static_cast<double>(m_filestream.tellg()) - target_line_pos;
        return size;
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
int File::getLineCount() const{
    if (has_mode(std::ios::in)){
        return line_count;
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
double File::getFileSize() const{
    if (has_mode(std::ios::in)){
        return file_size;
    }else{
        throw std::ios_base::failure("File is not open for input.");
    }
}
std::fstream& File::getFileStream() const{
    return m_filestream;
}