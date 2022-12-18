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

std::string &Utils::replaceString(std::string &s, const std::string &from, const std::string &to)
{
    if (!from.empty())
        for (size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}

std::string &Utils::formatJSON(std::string &json)
{
    json = Utils::replaceString(json, "{", "{\n\t");
    json = Utils::replaceString(json, ",", ",\n\t");
    json = Utils::replaceString(json, "}", "\n}");
    return json;
}

bool Utils::isValidJSON(const std::string &str)
{
    {
        std::stack<char> brackets;
        bool in_string = false;
        char string_delimiter = '\0';
        for (unsigned int i = 0; i < str.length(); i++)
        {
            char c = str[i];
            if (c == '"' || c == '\'')
            {
                if (in_string)
                {
                    if (c == string_delimiter)
                    {
                        in_string = false;
                    }
                }
                else
                {
                    in_string = true;
                    string_delimiter = c;
                }
            }
            else if (!in_string)
            {
                if (c == '{' || c == '[' || c == '(')
                {
                    brackets.push(c);
                }
                else if (c == '}')
                {
                    if (brackets.empty() || brackets.top() != '{')
                    {
                        return false;
                    }
                    brackets.pop();
                }
                else if (c == ']')
                {
                    if (brackets.empty() || brackets.top() != '[')
                    {
                        return false;
                    }
                    brackets.pop();
                }
                else if (c == ')')
                {
                    if (brackets.empty() || brackets.top() != '(')
                    {
                        return false;
                    }
                    brackets.pop();
                }
            }
        }
        return !in_string && brackets.empty();
    }
}