#include <fstream>

#include "file_reader.h"

namespace {
class file_reader_impl : public file_reader_ifc
{
public:
    bool open (std::ifstream &token_file, const std::string &file_path)
    {
        token_file.open (file_path);
        return token_file.good();
    }
    std::string getline (std::ifstream &token_file, std::string &line)
    {
        std::getline (token_file, line);
        return line;
    }
};

}

file_reader file_reader::create ()
{
    return file_reader (file_reader::delegate (new
                        file_reader_impl ));
}

