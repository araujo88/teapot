#include "../include/json_database.hpp"

using namespace tpt;
using namespace db;

JsonDatabase::JsonDatabase(const std::string &filename)
{
    this->filename = filename;
}

bool JsonDatabase::write(const std::string &data)
{
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open())
    {
        return false;
    }
    file << data << std::endl;
    file.close();
    return true;
}

std::vector<std::string> JsonDatabase::readAll()
{
    std::vector<std::string> records;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line))
    {
        records.push_back(line);
    }
    file.close();
    return records;
}

std::string JsonDatabase::read(unsigned int id)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        // Handle error, file not found or couldn't be opened
        return "File not found or couldn't be opened";
    }

    std::string line;
    unsigned int currentLine = 0;
    while (std::getline(file, line))
    {
        ++currentLine;
        if (currentLine == id)
        {
            return line; // Found the matching ID, return the line
        }
    }

    // If we reach here, the ID was not found in the file
    return "Record not found"; // Or return an empty string if preferred
}

JsonDatabase::~JsonDatabase()
{
}
