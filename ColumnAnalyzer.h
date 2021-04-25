#ifndef DIPLOMA_COLUMNANALYZER_H
#define DIPLOMA_COLUMNANALYZER_H

#include <vector>
#include <string>
#include <map>
#include <climits>
#include <iostream>
#include <istream>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <memory>

class ColumnAnalyzer {
public:
    std::map<std::string, unsigned short int> strMap;
    std::map<char, unsigned short> charMap;
    std::shared_ptr<std::vector<std::string>> columnValues;
    std::vector<unsigned short> ushortCol;
    std::vector<int> intCol;
    std::vector<float> floatCol;
    std::vector<double> doubleCol;
    bool useTypes;
    bool useLZW;

    ColumnAnalyzer(std::shared_ptr<std::vector<std::string>> colPtr, const bool& usetypes, const bool& useLZW);

    char Analyze();

    bool is_Double(const std::string& s, double& res);

    bool is_Int(const std::string& s, int& res);

    bool is_Short(const std::string& s, unsigned short& res);

    bool is_Float(const std::string& s, float& res);

    bool is_Numeric(const std::string& s);


};


#endif //DIPLOMA_COLUMNANALYZER_H
