#ifndef __FileReader_h__
#define __FileReader_h__
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "OSHeaders.h"

using namespace std;

//Define the structure of the file line - order of elements and data types
#define FILE_DATA_TYPES FileValue<UInt64,string, string, string>

template<class A, class B, class C, class D>
struct FileValue
{
    A user_id;
    B name;
    C pwd;
    D token;
};

class FileReader
{
public:
    //open file in constructor
    FileReader(const std::string filename);
    ~FileReader();

    //Read the actual data from the file
    void ReadValues();

    vector<FILE_DATA_TYPES> getData();
private:
    ifstream _file;
    //Vector of file data with data types declared
    vector<FILE_DATA_TYPES> _data;
};

#endif //__FileReader_h__