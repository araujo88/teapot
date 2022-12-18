#ifndef MODEL_H_
#define MODEL_H_

#include <string>

class Person
{
private:
    std::string name;
    std::string surname;

    Person();
    std::string getName();
    std::string getSurname();
    void setName(std::string name);
    void setSurname(std::string surname);
    ~Person();
};

#endif