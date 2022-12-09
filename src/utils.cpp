#include "../include/utils.hpp"
#include <fstream>
#include <sstream>

std::string Utils::readFileToBuffer(std::string filename)
{
    std::ifstream inFile;
    inFile.open(filename); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); // read the file
    return strStream.str();      // str holds the content of the file
}
