#include "ConfigParser.h"

#include <fstream>

void exitWithError(const std::string &err_msg) {
    std::cout << err_msg;
    std::cout << "Press Any Key to Exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    exit(EXIT_FAILURE);
}
template <>
std::string Convert::string_to_T<std::string>(std::string const &val) {
    return val;
}

ConfigParser &ConfigParser::Get_Global_Parser() {
    static ConfigParser gParser;
    return gParser;
}

ConfigParser::ConfigParser(const std::string &fName) {
    // this->fName = fName;
    Parse_File(fName);
}

bool ConfigParser::Has_Key(const std::string &key) const { return contents.find(key) != contents.end(); }

void ConfigParser::Remove_Comment(std::string &line) const {
    if (line.find('#') != line.npos) {
        line.erase(line.find('#'));
    }
}

bool ConfigParser::Only_Whitespace_Line(const std::string &line) const {
    return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigParser::Valid_Line(const std::string &line) const {
    std::string tmp = line;
    tmp.erase(0, tmp.find_first_not_of("\t "));
    if (tmp[0] == '=') {
        // * No Key
        return false;
    }
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ' ' && tmp[i] != '\t') {
            return true;
        }
    }
    return false;
}

void ConfigParser::Extract_Key(std::string &key, int const &pos, const std::string &line) const {
    // * pos is the position of '=' in the line
    key = line.substr(0, pos);
    // if (key.find('\t') != line.npos || key.find(' ') != line.npos) {
    //     // remove extra space or tab at the end;
    //     key.erase(key.find_first_of("\t "));
    // }
    key.erase(0, key.find_first_not_of("\t "));
    key.erase(key.find_last_not_of("\t ") + 1);
}

void ConfigParser::Extract_Value(std::string &value, int const &pos, const std::string &line) const {
    value = line.substr(pos + 1);
    value.erase(0, value.find_first_not_of("\t "));
    value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigParser::Extract_Content(const std::string &line) {
    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    int pos = temp.find('=');
    std::string key, value;
    Extract_Key(key, pos, temp);
    Extract_Value(value, pos, temp);

    if (!Has_Key(key)) {
        contents.insert(std::pair<std::string, std::string>(key, value));
    } else {
        contents[key] = value;
    }
}

void ConfigParser::Parse_Line(const std::string &line, int const line_Number) {
    if (line.find('=') == line.npos) {
        std::cout << "ConfigParser: Can't find separator on line: " << line_Number << std::endl;
        std::cout << "Proceed to next line..." << std::endl;
    }
    if (!Valid_Line(line)) {
        std::cout << "ConfigParser: Bad format for line: " << line_Number << ", can't find either key or value"
                  << std::endl;
        std::cout << "Proceed to next line..." << std::endl;
    }
    Extract_Content(line);
}

void ConfigParser::Parse_File(const std::string &fName) {
    std::ifstream conf_file;
    conf_file.open(fName.c_str());
    if (!conf_file) return;

    std::string line;
    int line_Number = 0;
    while (std::getline(conf_file, line)) {
        line_Number++;
        std::string temp = line;

        if (temp.empty()) {
            continue;
        }

        Remove_Comment(temp);
        if (Only_Whitespace_Line(temp)) {
            continue;
        }

        Parse_Line(temp, line_Number);
    }
    conf_file.close();
}

void ConfigParser::Print_Config() const {
    for (auto &&inter : contents) {
        std::cout << "<" << inter.first << ">:<" << inter.second << ">" << std::endl;
    }
}
