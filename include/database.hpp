#ifndef IDATABASE_H
#define IDATABASE_H

#include <string>
#include <vector>

namespace tpt
{
    namespace db
    {
        class IDatabase
        {
        public:
            virtual ~IDatabase() {}

            // Writes data to the database.
            // Returns true if the operation is successful.
            virtual bool write(const std::string &data) = 0;

            // Reads all data from the database.
            // Returns a vector of strings, where each string represents a record.
            virtual std::vector<std::string> readAll() = 0;

            // Reads data from the database based on id.
            // Returns a string if an entry is found.
            virtual std::string read(unsigned int id) = 0;
        };
    }
}

#endif
