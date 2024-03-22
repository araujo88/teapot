#ifndef JSONDATABASE_H_
#define JSONDATABASE_H_

#include "database.hpp"
#include <fstream>
#include <sstream>

namespace tpt
{
    namespace db
    {
        class JsonDatabase : public IDatabase
        {
        private:
            std::string filename;

        public:
            JsonDatabase(const std::string &filename);
            ~JsonDatabase();
            virtual bool write(const std::string &data) override;
            virtual std::vector<std::string> readAll() override;
            virtual std::string read(unsigned int id) override;
        };

    }
}

#endif
