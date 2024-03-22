#include "../include/model.hpp"

using namespace tpt;

Model::Model()
{
    this->name = "John";
    this->surname = "Doe";
}

Model::Model(std::string name, std::string surname)
{
    this->name = name;
    this->surname = surname;
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
