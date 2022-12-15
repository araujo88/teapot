#include "../include/utils.hpp"

std::string Utils::readFileToBuffer(std::string filename)
{
    std::ifstream file;
    file.open(filename); // open the input file

    if (!file)
    {
        throw FileNotFoundException();
    }

    std::stringstream strStream;
    strStream << file.rdbuf(); // read the file
    return strStream.str();    // str holds the content of the file
}

std::string Utils::date()
{
    time_t t;
    time(&t);

    std::string date = ctime(&t);
    date.erase(remove(date.begin(), date.end(), '\n'), date.end());

    return date;
}

std::string Utils::btos(bool x)
{
    if (x)
        return "true";
    return "false";
}