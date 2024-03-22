#include "../include/model.hpp"

using namespace tpt;

unsigned int tpt::Model::count = 0;

Model::Model()
{
    this->name = "John";
    this->surname = "Doe";
    this->id = count;
    count++;
}

Model::Model(std::string name, std::string surname)
{
    this->name = name;
    this->surname = surname;
    this->id = count;
    count++;
}

unsigned int Model::getId()
{
    return this->id;
}

std::string Model::getName()
{
    return this->name;
}

std::string Model::getSurname()
{
    return this->surname;
}

void Model::setName(std::string name)
{
    this->name = name;
}

void Model::setSurname(std::string surname)
{
    this->surname = surname;
}

Model::~Model()
{
}
