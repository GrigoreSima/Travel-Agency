//
// Created by Grigore Sima on 25.04.2023.
//

#ifndef TA_VALIDATOR_H
#define TA_VALIDATOR_H

#include <iostream>
#include <cassert>

class Exception : public std::exception
{
    std::string _msg;
    std::string _functionality;
    std::string _field;
    std::string _full_msg;
public:
    explicit Exception(std::string  msg, std::string  functionality, std::string  field) :
    _msg(std::move(msg)), _functionality(std::move(functionality)), _field(std::move(field))
    {
        _full_msg = "\n" + _functionality + ": " + _field + " " + _msg + " !\n";
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        return _full_msg.c_str();
    }

    static void testWhat()
    {
        Exception exception = Exception("error", "Exception", "msg");
        std::string msg = "\nException: msg error !\n";
        assert(strcmp(exception.what(),msg.c_str()) == 0);
    }

};

class Validate
{
private:
    static void testNoWhitespaces();
    static void testStrToLower();
    static void testValidateDestination();
    static void testValidateType();
    static void testValidatePrice();
    static void testValidatePosition();
    static void testValidateNumber();
    static void testValidateModify();
    static void testValidateFilter();
    static void testValidateSort();

public:
    static void noWhitespaces(std::string&);
    static void strToLower(std::string&);

    static void validateDestination(const std::string&, const std::string&);
    static void validateType(const std::string&, const std::string&);
    static void validatePrice(const std::string&, const std::string&);
    static void validatePosition(const std::string&, const std::string&, const int &);
    static void validateNumber(const std::string&, const std::string&, const int &);


    static void validateModify(const int&, const std::string&, const std::string&, const int &);
    static void validateFilter(const int&, const std::string&);
    static void validateSort(const int&, const int&);


    static void tests();
};



#endif //TA_VALIDATOR_H
