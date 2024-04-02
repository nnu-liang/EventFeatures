#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <iostream>
#include <map>
#include <sstream>
#include <string>

void exitWithError(const std::string &err_msg);

class Convert {
public:
    template <typename T>
    static std::string T_to_string(T const &val) {
        std::ostringstream ostr;
        ostr << val;
        return ostr.str();
    }

    template <typename T>
    static T string_to_T(std::string const &val) {
        std::istringstream istr(val);
        T returnVal;
        if (!(istr >> returnVal)) {
            exitWithError("ConfigParser: Not a valid " + static_cast<std::string>(typeid(T).name()) + " received!\n");
        }
        return returnVal;
    }
};

class ConfigParser {
private:
    std::map<std::string, std::string> contents;
    // std::string fName;

    void Remove_Comment(std::string &line) const;

    bool Only_Whitespace_Line(const std::string &line) const;

    bool Valid_Line(const std::string &line) const;

    void Extract_Key(std::string &key, int const &pos, const std::string &line) const;

    void Extract_Value(std::string &value, int const &pos, const std::string &line) const;

    void Extract_Content(const std::string &line);

    void Parse_Line(const std::string &line, int const line_Number);

    ConfigParser(){};

    ConfigParser(const std::string &fName);

public:
    static ConfigParser &Get_Global_Parser();

    bool Has_Key(const std::string &key) const;

    void Parse_File(const std::string &fName);

    void Print_Config() const;

    template <typename ValueType>
    ValueType Get_Value(const std::string &key, ValueType const &defaultValue = ValueType()) const {
        if (!Has_Key(key)) {
            std::cout << "Didn't Find key: " << key << ", using default value: " << defaultValue << std::endl;
            return defaultValue;
        }
        return Convert::string_to_T<ValueType>(contents.find(key)->second);
    }
};

#endif  // CONFIG_PARSER_H_
