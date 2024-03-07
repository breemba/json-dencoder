#include <gtest/gtest.h>
#include <variable.hpp>
#include <iostream>
#include <string>
#include <vector>
using AT = Available_types;

TEST(read_test, object_first_testCase)
{
    const std::string data = "{\"id\":1, \"name\":\"Alex\", \"value\":0.2}";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), 6);
    std::vector<Available_types> types = {AT::Int, AT::String, AT::Float};
    int counter = 0;
    for (auto &element : variable.get_map())
    {
        EXPECT_EQ((element.second).get_type(), types[counter]);
        counter++;
    }
}
TEST(read_test, array_first_testCase)
{
    const std::string data = "[1,1.3,\"Alex\",true]";
    Variable variable = read(data);
    VecType vector = variable.get_vector();
    std::vector<Available_types> types = {AT::Int, AT::Float, AT::String, AT::Bool};
    int counter = 0;
    for (auto it = vector.begin(); it != vector.end(); it++)
    {
        Variable element = *it;
        ASSERT_EQ(element.get_type(), types[counter]);
        counter++;
    }
}
TEST(read_test, int_testCase)
{
    const std::string data = "10";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);
}
TEST(read_test, float_testCase)
{
    const std::string data = "1.2";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), AT::Float);
    EXPECT_EQ(variable.get_float(), 1.2f);
}
TEST(read_test, bool_testCase)
{
    const std::string data = "true";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), AT::Bool);
    EXPECT_EQ(variable.get_bool(), true);
}
TEST(read_test, string_testCase)
{
    const std::string data = "\"Alex\"";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), AT::String);
    EXPECT_EQ(variable.get_string(), "Alex");
}
TEST(read_test, string_testCase_withSpace)
{
    const std::string data = "\"Alex is:\"";
    Variable variable = read(data);
    ASSERT_EQ(variable.get_type(), AT::String);
    EXPECT_EQ(variable.get_string(), "Alex is:");
}
TEST(write_test, int_invalid_call_testCase)
{
    try
    {
        Variable variable(1.2f);
        variable.get_int();
    }
    catch (const char *error)
    {
        ASSERT_STREQ(error, "Just variable with type int can use this function");
    }
}
TEST(write_test, float_invalid_call_testCase)
{
    try
    {
        Variable variable(2);
        variable.get_float();
    }
    catch (const char *error)
    {
        ASSERT_STREQ(error, "Just variable with type float can use this function");
    }
}
TEST(variable_class_test, int_testCase)
{
    Variable variable(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);

    variable = int8_t(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);

    variable = int16_t(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);

    variable = int32_t(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);

    variable = uint8_t(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);

    variable = uint16_t(10);
    ASSERT_EQ(variable.get_type(), AT::Int);
    EXPECT_EQ(variable.get_int(), 10);
}
TEST(variable_class_test, null_testCase)
{
    Variable variable;
    EXPECT_EQ(variable.get_type(), AT::Null);
    variable = 2;
    EXPECT_NE(variable.get_type(), AT::Null);
    variable.nullify();
    EXPECT_EQ(variable.get_type(), AT::Null);
}
TEST(variable_class_test, float_testCase)
{
    Variable variable(1.2f);
    ASSERT_EQ(variable.get_type(), AT::Float);
    EXPECT_EQ(variable.get_float(), 1.2f);
}
TEST(variable_class_test, bool_testCase)
{
    Variable variable(true);
    ASSERT_EQ(variable.get_type(), AT::Bool);
    EXPECT_EQ(variable.get_bool(), true);
}
TEST(variable_class_test, object_testCase)
{
    MapType map;
    map["a"] = "b";
    map["c"] = 2;
    map["d"] = true;
    Variable mapVar(map);
    EXPECT_EQ(mapVar.get_type(), AT::Map);
}
TEST(write_test, int_testCase)
{
    Variable variable(10);
    std::string result = "";
    write(result, variable);
    EXPECT_EQ(result, "10");
}
TEST(write_test, float_testCase)
{
    Variable variable(100.2f);
    std::string result = "";
    write(result, variable);
    EXPECT_EQ(result, "100.2");

    variable = 1000.345f;
    result = "";
    write(result, variable);
    EXPECT_EQ(result, "1000.345");
}
TEST(write_test, bool_testCase)
{
    Variable variable(true);
    std::string result = "";
    write(result, variable);
    EXPECT_EQ(result, "true");

    variable = false;
    result = "";
    write(result, variable);
    EXPECT_EQ(result, "false");
}
TEST(write_test, string_testCase)
{
    Variable variable("Alex");
    std::string result = "";
    write(result, variable);
    EXPECT_EQ(result, "\"Alex\"");

    variable = false;
    result = "";
    write(result, variable);
    EXPECT_EQ(result, "false");
}
TEST(write_test, null_testCase)
{
    Variable variable;
    std::string result = "";
    write(result, variable);
    EXPECT_EQ(result, "null");
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}