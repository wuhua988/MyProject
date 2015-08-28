#include "FileReader.h"

//Ctor
FileReader::FileReader(const std::string filename)
{
    _file.open(filename.c_str());
}

//Dtor
FileReader::~FileReader()
{
    _file.close();
}

//Read the data from the file
void FileReader::ReadValues()
{
    std::string line;

    while(getline(_file, line)) {

        FILE_DATA_TYPES parse;
        parse.name.clear();
        parse.pwd.clear();
        parse.token.clear();

        std::stringstream ss(line);

        //Read the first value from the line
        ss >> parse.name;

        // If the line starts with '#' or if it's an empty line, skip that line
        if (parse.name.compare(0, 1, "#") == 0 || parse.name.empty()) {
            continue;
        }

        //Read "PASSWORD" value from the line
        ss >> parse.pwd;
        //Read "TOKEN" value from the line
        ss >> parse.token;


        _data.push_back(parse);

    }
}

vector<FILE_DATA_TYPES> FileReader::getData()
{
    return _data;
}
