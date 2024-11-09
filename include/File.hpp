#ifndef _FILE_HPP_
#define _FILE_HPP_
#include <fstream>
#include <vector>
class File {
    private:
        mutable std::fstream m_filestream;
        std::string m_filename;
        std::ios_base::openmode m_mode;
        int line_count;
        double file_size;
    public:
        File(const std::string filename, const std::ios_base::openmode mode);
        ~File();
        bool has_mode(const std::ios_base::openmode mode) const;
        void getLines(int strat,int end,std::vector<std::string> &lines) const;
        double getLinesSize(int strat,int end) const;
        void getLine(int line_num, std::string &line) const;
        double getLineSize(int line_num) const;
        int getLineCount() const;
        double getFileSize() const;
        std::fstream& getFileStream() const;
};
#endif