#include "../include/utils.hpp"

using namespace tpt;

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

std::string Utils::formatFormDataToJson(std::unordered_map<std::string, std::string> formData)
{
    std::stringstream json;
    json << "{";

    bool isFirst = true; // Flag to handle comma placement
    for (const auto &pair : formData)
    {
        if (!isFirst)
        {
            json << ", ";
        }
        json << "\"" << pair.first << "\": \"" << pair.second << "\"";
        isFirst = false;
    }

    json << "}";
    return json.str();
}
std::vector<std::unordered_map<std::string, std::string>> Utils::parseJsonArray(const std::string &json)
{
    std::vector<std::unordered_map<std::string, std::string>> result;
    std::unordered_map<std::string, std::string> currentItem;
    std::string key, value, line;

    // Simulate a stream for line-by-line processing
    std::istringstream stream(json);
    bool isInQuotes = false;
    char prevChar = '\0';

    while (getline(stream, line, ','))
    {
        // Toggle isInQuotes based on unescaped quotes
        for (char c : line)
        {
            if (c == '\"' && prevChar != '\\')
            {
                isInQuotes = !isInQuotes;
            }
            prevChar = c;
        }

        // Skip processing if we're inside quotes
        if (isInQuotes)
            continue;

        line.erase(remove_if(line.begin(), line.end(), [](unsigned char x)
                             { return isspace(x); }),
                   line.end()); // Remove spaces

        if (line.find('{') != std::string::npos)
        {
            // New object start, clear old data
            currentItem.clear();
        }
        else if (line.find('}') != std::string::npos)
        {
            // Object end, save it to the result
            if (!currentItem.empty())
            {
                result.push_back(currentItem);
                currentItem.clear();
            }
        }
        else
        {
            // Extracting key-value pairs
            std::string::size_type pos = line.find(':');
            if (pos != std::string::npos)
            {
                key = line.substr(0, pos);
                value = line.substr(pos + 1);

                // Removing quotes and brackets from key and value
                key.erase(remove(key.begin(), key.end(), '\"'), key.end());
                value.erase(remove(value.begin(), value.end(), '\"'), value.end());
                value.erase(remove(value.begin(), value.end(), '['), value.end());
                value.erase(remove(value.begin(), value.end(), ']'), value.end());

                // Inserting into current map
                currentItem[key] = value;
            }
        }
    }

    // Add the last item if it wasn't added already
    if (!currentItem.empty())
    {
        result.push_back(currentItem);
    }

    return result;
}
