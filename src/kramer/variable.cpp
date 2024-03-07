#include "variable.hpp"

#include <string.h>
#include <sstream>

#define check(str, err)       \
    if (*wrapper->ptr != str) \
        throw err;            \
    wrapper->increment();
#define ValidNumberChars ".+-1234567890"

int Variable::get_int()
{
    if (myVariable.index() == (Available_types)Int)
        return std::get<int>(myVariable);
    else
        throw("Just variable with type int can use this function");
};
float Variable::get_float()
{
    if (myVariable.index() == (Available_types)Float)
        return std::get<float>(myVariable);
    else
        throw("Just variable with type float can use this function");
};
bool Variable::get_bool()
{
    if (myVariable.index() == (Available_types)Bool)
        return std::get<bool>(myVariable);
    else
        throw("Just variable with type bool can use this function");
};
std::string Variable::get_string()
{
    if (myVariable.index() == (Available_types)String)
        return std::get<std::string>(myVariable);
    else
        throw("Just variable with type string can use this function");
};
VecType Variable::get_vector()
{
    if (myVariable.index() == (Available_types)Vector)
        return std::get<VecType>(myVariable);
    else
        throw("Just variable with type vector can use this function");
};
MapType Variable::get_map()
{
    if (myVariable.index() == (Available_types)Map)
        return std::get<MapType>(myVariable);
    else
        throw("Just variable with type map can use this function");
};
bool Variable::is_null()
{
    return myVariable.index() == 0;
};
int Variable::get_type()
{
    return myVariable.index();
}
void Variable::nullify()
{
    Type new_var;
    myVariable = new_var;
}

void write(std::string &data, Variable input, int indent)
{
    int type_index = input.get_type();
    try
    {
        if (type_index == Available_types::Int)
        {
            data = data + get_indent(indent);
            write_int(data, input.get_int());
            return;
        }
        if (type_index == Available_types::Float)
        {
            data = data + get_indent(indent);
            write_float(data, input.get_float());
            return;
        }
        if (type_index == Available_types::Bool)
        {
            data = data + get_indent(indent);
            write_bool(data, input.get_bool());
            return;
        }
        if (type_index == Available_types::String)
        {
            data = data + get_indent(indent);
            write_string(data, input.get_string());
            return;
        }
        if (type_index == Available_types::Map)
        {
            write_map(data, input.get_map(), indent);
            return;
        }
        if (type_index == Available_types::Vector)
        {
            write_vector(data, input.get_vector(), indent);
            return;
        }
        if (type_index == Available_types::Null)
        {
            write_null(data);
        }
    }
    catch (const char *error)
    {
        std::cout << error << std::endl;
    }
}
void write_int(std::string &data, int input)
{
    data + input;
}
void write_float(std::string &data, float input)
{
    data + input;
}
void write_bool(std::string &data, bool input)
{
    std::string str = (input) ? "true" : "false";
    data = data + str;
}
void write_string(std::string &data, std::string input)
{
    data = data + "\"" + input + "\"";
}
void write_null(std::string &data)
{
    data = data + "null";
}
void write_vector(std::string &data, VecType input, int indent)
{
    data = data + get_indent(indent + 1) + "[\n";
    for (int i = 0; i < input.size(); i++)
    {
        if (i != 0)
            data = data + ",\n";
        write(data, input[i], indent + 1);
    }
    data = data + "\n";
    data = data + get_indent(indent + 1) + "]";
}
void write_map(std::string &data, MapType input, int indent)
{
    data = data + get_indent(indent + 1) + "{\n";
    int counter = 0;
    for (const auto &pair : input)
    {
        if (counter++ != 0)
            data = data + ",\n";
        data = data + get_indent(indent + 2) + "\"" + pair.first + "\"" + ":";
        write(data, pair.second, indent + 1);
    }
    data = data + "\n";
    data = data + get_indent(indent + 1) + "}\n";
}
static void operator+(std::string &data, int input)
{
    data = data + std::to_string(input);
}

static void operator+(std::string &data, float input)
{
    std::stringstream stream;
    stream.precision(8);
    stream << input;
    std::string str = stream.str();
    data = data + str;
}
std::string get_indent(int number)
{
    std::string result = "";
    for (int i = 0; i < number; i++)
    {
        result = result + " ";
    }
    return result;
}
// ============================ start of reader part ==============================================

void operator<<(std::ostream &os, Error error)
{
    if (error == Error::CommaRequired)
        os << "',' required";
    if (error == Error::ColonRequired)
        os << "':' required after property key for an object";
    if (error == Error::NotValidDataType)
        os << "Not valid datatype";
    if (error == Error::ObjectWrongCloseChar)
        os << "Opening tag '{' requires a closing tag '}' or ','";
    if (error == Error::ArrayWrongCloseChar)
        os << "Opening tag '[' requires a closing tag ']' or ','";
    if (error == Error::ObjectCloseQuoteRequired)
        os << "A \" required for a string";
    os << std::endl;
}

void read_string(Wrapper *wrapper, Variable &variable)
{
    wrapper->increment();
    wrapper->skip_space();
    const char *ptr = wrapper->ptr;
    std::string value;
    while (!wrapper->end())
    {
        if (wrapper->charType() == Characters::DoubleQuote)
        {
            break;
        }
        wrapper->increment();
    }
    std::string result(ptr, wrapper->ptr);
    if (wrapper->end())
        throw Error::ObjectCloseQuoteRequired;
    wrapper->increment();
    variable = result;
}
void read_number(Wrapper *wrapper, Variable &variable)
{
    const char *ptr = wrapper->ptr;
    bool is_float = false;
    while (strchr(ValidNumberChars, *wrapper->ptr))
    {
        if (*wrapper->ptr == '.')
            is_float = true;
        wrapper->increment();
    }
    std::string str(ptr, wrapper->ptr);
    if (is_float)
        variable = std::stof(str);
    else
        variable = std::stoi(str);
}

void read_obj(Wrapper *wrapper, Variable &variable)
{
    wrapper->increment();
    wrapper->skip_space();
    if (wrapper->charType() != Characters::CloseBrace && wrapper->charType() != Characters::DoubleQuote)
    {
        throw Error::ObjectValidChar;
    }
    MapType map;
    while (true)
    {
        Variable stringVar;
        wrapper->skip_space();
        read_string(wrapper, stringVar);
        wrapper->skip_space();
        check(':', Error::ColonRequired);
        Variable valueVariable;
        start_read(wrapper, valueVariable);
        wrapper->skip_space();
        map[stringVar.get_string()] = valueVariable;
        if (wrapper->charType() == Characters::Comma)
        {
            wrapper->increment();
            continue;
        }
        else if (wrapper->charType() == Characters::CloseBrace)
        {
            break;
        }
        else
            throw Error::ObjectWrongCloseChar;
    }
    variable = map;
}
void read_array(Wrapper *wrapper, Variable &variable)
{
    VecType vec;
    while (true)
    {
        wrapper->increment();
        wrapper->skip_space();
        Variable variable;
        start_read(wrapper, variable);
        vec.push_back(variable);
        wrapper->skip_space();
        if (wrapper->charType() == Characters::Comma)
            continue;
        else if (wrapper->charType() == Characters::CloseBracket)
        {
            wrapper->increment();
            break;
        }
        else
            throw Error::ArrayWrongCloseChar;
    }
    variable = vec;
}
void start_read(Wrapper *wrapper, Variable &variable)
{
    wrapper->skip_space();
    if (wrapper->charType() == Characters::OpenBrace)
        return read_obj(wrapper, variable);
    if (wrapper->charType() == Characters::OpenBracket)
        return read_array(wrapper, variable);
    if (wrapper->charType() == Characters::DoubleQuote)
    {
        read_string(wrapper, variable);
        return;
    }
    if (strchr(ValidNumberChars, *wrapper->ptr))
    {
        read_number(wrapper, variable);
        return;
    }
    if (strncmp(wrapper->ptr, "true", 4) == 0)
    {
        variable = true;
        wrapper->increment(4);
        return;
    }
    if (strncmp(wrapper->ptr, "false", 5) == 0)
    {
        variable = false;
        wrapper->increment(5);
        return;
    }
    if (strncmp(wrapper->ptr, "null", 4) == 0)
    {
        wrapper->increment(4);
    }
    else
    {
        throw Error::NotValidDataType;
    }
}

Variable read(const std::string &jsonString)
{
    Variable variable;
    try
    {
        const char *ptr = &(jsonString[0]);
        Wrapper wrapper(ptr);
        start_read(&wrapper, variable);
        return variable;
    }
    catch (Error error)
    {
        std::cout << error;
        return variable;
    }
}

void operator<<(std::ostream &os, MapType map)
{
    std::cout << "{" << std::endl;
    for (auto it = map.begin(); it != map.end(); it++)
    {
        std::cout << it->first << ":";
        print(it->second);
        if (std::next(it) != map.end())
            std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "}" << std::endl;
}
void operator<<(std::ostream &os, VecType vec)
{
    std::cout << "[";
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        print(*it);
        if (std::next(it) != vec.end())
            std::cout << ",";
    }
    std::cout << "]" << std::endl;
}
void print(Variable &variable)
{
    if (variable.get_type() == Available_types::Map)
    {
        std::cout << variable.get_map();
    }
    if (variable.get_type() == Available_types::Vector)
    {
        std::cout << variable.get_vector();
    }
    if (variable.get_type() == Available_types::Int)
    {
        std::cout << variable.get_int();
    }
    if (variable.get_type() == Available_types::String)
    {
        std::cout << variable.get_string();
    }
    if (variable.get_type() == Available_types::Float)
    {
        std::cout << variable.get_float();
    }
    if (variable.get_type() == Available_types::Bool)
    {
        std::cout << variable.get_bool();
    }
    if (variable.get_type() == Available_types::Null)
    {
        std::cout << "null";
    }
}
