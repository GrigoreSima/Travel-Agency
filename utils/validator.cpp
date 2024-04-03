//
// Created by Grigore Sima on 25.04.2023.
//

#include "validator.h"

void Validate::noWhitespaces(std::string &string)
{
    std::string result;

    for(auto it: string)
        if(it!=' ' && it!='\n')
            result += it;

    string = result;
}

void Validate::strToLower(std::string &string)
{
    std::transform(string.begin(), string.end(), string.begin(),
                   [](unsigned char c){ return std::tolower(c); });
}

void Validate::validateDestination(const std::string &destination, const std::string &functionality)
{
    for(auto it: destination)
        if(!isalpha(it) && !strchr("- ", it))
            throw Exception("needs to contain just letters and -", functionality, "Destination");
}

void Validate::validateType(const std::string &type, const std::string &functionality)
{
    try {
        stoi(type);
    }
    catch (std::exception& exception)
    {
        throw Exception("needs to be a number", functionality, "Type");
    }
}

void Validate::validatePrice(const std::string &price, const std::string &functionality)
{
    try {
        stoi(price);
    }
    catch (std::exception& exception)
    {
        throw Exception("needs to be a number", functionality, "Price");
    }
}

void Validate::validatePosition(const std::string &position, const std::string &functionality, const int &max_size)
{
    try {
        stoi(position);
    }
    catch (std::exception& exception)
    {
        throw Exception("needs to be a number", functionality, "Index");
    }
    int integer_position = stoi(position);
    std::string msg = "needs to be between " + std::to_string(1) + " and " + std::to_string(max_size);
    if(integer_position < 1 || integer_position > max_size)
        throw Exception(msg, functionality, "Index");
}

void Validate::validateNumber(const std::string &number, const std::string &functionality, const int &max_size)
{
    try {
        stoi(number);
    }
    catch (std::exception& exception)
    {
        throw Exception("needs to be a number", functionality, "Number");
    }
    int integer_position = stoi(number);
    std::string msg = "needs to be between " + std::to_string(1) + " and " + std::to_string(max_size);
    if(integer_position < 1 || integer_position > max_size)
        throw Exception(msg, functionality, "Number");
}

void Validate::validateModify(const int& field, const std::string& updated_value, const std::string& position, const int &max_size)
{
    if(field == -1)
        throw Exception("needs to be a in this list: [ \"Name\", \"Destination\", "
                        "\"Type\", \"Price\" ]", "Modify", "Field");


    switch (field)
    {
        case 2: // Destination
            validateDestination(updated_value, "Modify");
            break;

        case 3: // Type
            validateType(updated_value, "Modify");
            break;

        case 4: // Price
            validatePrice(updated_value, "Modify");
            break;

        default:
            break;
    }

    Validate::validatePosition(position, "Modify", max_size);
}

void Validate::validateFilter(const int &field, const std::string &searched_value)
{
    if(field == -1)
        throw Exception("needs to be a in this list: [ \"Destination\", "
                        "\"Price\" ]", "Filter", "Field");


    switch (field)
    {
        case 1: // Destination
            validateDestination(searched_value, "Filter");
            break;

        case 2: // Price
            validatePrice(searched_value, "Filter");
            break;

        default:
            break;
    }
}

void Validate::validateSort(const int &field, const int &mode)
{
    if(field == -1)
        throw Exception("needs to be a in this list: [ \"Name\", \"Destination\", "
                        "\"Type\" ]", "Sort", "Field");

    if(mode == -1)
        throw Exception(R"(needs to be a in this list: [ "Descendent", "Ascendent" ])", "Sort", "Mode");
}



// Tests

void Validate::tests()
{
    Exception::testWhat();

    testNoWhitespaces();
    testStrToLower();
    testValidateDestination();
    testValidateType();
    testValidatePrice();
    testValidatePosition();
    testValidateNumber();
    testValidateModify();
    testValidateFilter();
    testValidateSort();
}

void Validate::testNoWhitespaces()
{
    std::string field = "ab cd ef";

    noWhitespaces(field);
    assert(field == "abcdef");

    field = "cd\nef";
    noWhitespaces(field);
    assert(field == "cdef");
}

void Validate::testStrToLower()
{
    std::string field = "Paris";

    strToLower(field);
    assert(field == "paris");

    field = "DuBaI";
    strToLower(field);
    assert(field == "dubai");

    field = "bucharesT";
    strToLower(field);
    assert(field == "bucharest");

}

void Validate::testValidateDestination()
{
    try {
        validateDestination("12d", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateDestination("? ?", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validateDestination("Cluj-Napoca", "TEST");
    assert(true);
}

void Validate::testValidateType()
{
    try {
        validateType("d12d", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateType("-?", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validateType("12", "TEST");
    assert(true);
}

void Validate::testValidatePrice()
{
    try {
        validatePrice("d12d", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validatePrice("-?", "TEST");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validatePrice("12", "TEST");
    assert(true);

}

void Validate::testValidatePosition()
{
    try {
        validatePosition("d12d", "TEST", 13);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validatePosition("-?", "TEST", 13);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validatePosition("12", "TEST", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validatePosition("12", "TEST", 13);
    assert(true);
}

void Validate::testValidateNumber()
{
    try {
        validateNumber("d12d", "TEST", 13);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateNumber("-?", "TEST", 13);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateNumber("12", "TEST", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validateNumber("12", "TEST", 13);
    assert(true);
}


void Validate::testValidateModify()
{
    try {
        validateModify(-1, "Dubai", "1", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateModify(2, "?_", "1", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateModify(3, "Dubai", "2", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateModify(4, "Dubai", "3", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateModify(4, "12", "15", 10);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validateModify(2, "Dubai", "1", 10);
    assert(true);

    validateModify(3, "12", "1", 10);
    assert(true);

    validateModify(4, "5", "1", 10);
    assert(true);
}

void Validate::testValidateFilter()
{
    try {
        validateFilter(-1, "Dubai");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateFilter(1, "?-");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateFilter(2, "Dubai");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }


    validateFilter(213, "Dubai");
    assert(true);

    validateFilter(1, "Dubai");
    assert(true);

    validateFilter(2, "120");
    assert(true);
}

void Validate::testValidateSort()
{
    try {
        validateSort(-1, 1);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    try {
        validateSort(1, -1);
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    validateSort(1, 1);
    assert(true);


    validateSort(2, 0);
    assert(true);
}
