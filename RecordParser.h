#ifndef DIPLOMA_RECORDPARSER_H
#define DIPLOMA_RECORDPARSER_H

#include <string>
#include <vector>

class RecordParser
{
public:
    static std::vector<std::string> csv_split(std::string source, char delimeter);

    static std::string csv_pack(std::vector<std::string>& source, char delimiter);
};


#endif //DIPLOMA_RECORDPARSER_H
