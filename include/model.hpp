#ifndef MODEL_H_
#define MODEL_H_

#include <string>

namespace tpt
{
    class Model
    {
    private:
        std::string name;
        std::string surname;

    public:
        Model();
        Model(std::string name, std::string surname);
        std::string getName();
        std::string getSurname();
        void setName(std::string name);
        void setSurname(std::string surname);
        ~Model();
    };
}

#endif