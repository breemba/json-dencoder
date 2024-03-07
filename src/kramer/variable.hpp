#pragma once
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <algorithm>

enum Available_types
{
    Null = 0,
    Int = 1,
    Float = 2,
    Bool = 3,
    String = 4,
    Vector = 5,
    Map = 6,
};

enum Error
{
    ObjectValidChar,
    ObjectCloseQuoteRequired,
    ObjectCloseBraceRequired,
    CommaRequired,
    ColonRequired,
    ArrayWrongCloseChar,
    ObjectWrongCloseChar,
    NotValidDataType,
};
enum Characters
{
    DoubleQuote,
    Comma,
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBracket,
    Colon,
    Space,
    None
};

class Variable;

using VecType = std::vector<Variable>;
using MapType = std::map<std::string, Variable>;
using Type = std::variant<std::monostate, int, float, bool, std::string, VecType, MapType>;

class Variable
{
private:
    Type myVariable;

public:
    Variable(Type variable)
    {
        myVariable = variable;
    }
    Variable() {}
    void operator=(Type var)
    {
        myVariable = var;
    }
    int get_int();
    float get_float();
    bool get_bool();
    bool is_null();
    void nullify();
    int get_type();
    std::string get_string();
    VecType get_vector();
    MapType get_map();
};
class Wrapper
{
public:
    const char *ptr;
    Wrapper(const char *pointer) : ptr(pointer)
    {
        Variable variable;
    }
    void increment()
    {
        ptr++;
    }
    void increment(int step)
    {
        ptr = ptr + step;
    }
    bool end()
    {
        return *ptr == '\0';
    }
    void skip_space()
    {

        while (*(ptr) == ' ')
        {
            ptr++;
        }
    }
    Characters charType()
    {
        if (*ptr == '}')
            return Characters::CloseBrace;
        if (*ptr == ',')
            return Characters::Comma;
        if (*ptr == '{')
            return Characters::OpenBrace;
        if (*ptr == '"')
            return Characters::DoubleQuote;
        if (*ptr == '[')
            return Characters::OpenBracket;
        if (*ptr == ']')
            return Characters::CloseBracket;
        if (*ptr == ':')
            return Characters::Colon;
        if (*ptr == ' ')
            return Characters::Space;
        else
            return Characters::None;
    }
};

void write(std::string &str, Variable data, int indent = 0);
void write_int(std::string &str, int input);
void write_string(std::string &data, std::string input);
void write_float(std::string &data, float input);
void write_bool(std::string &data, bool input);
void write_null(std::string &data);
void write_map(std::string &data, MapType input, int indent);
void write_vector(std::string &data, VecType input, int indent);
static void operator+(std::string &data, int input);
static void operator+(std::string &data, float input);
std::string get_indent(int number);
Variable read(const std::string &jsonString);
void start_read(Wrapper *wrapper, Variable &variable);
void read_obj(Wrapper *wrapper, Variable &variable);
void read_number(Wrapper *wrapper, Variable &variable);
void read_array(Wrapper *wrapper, Variable &variable);
void print(Variable &variable);