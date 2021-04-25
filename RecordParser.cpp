#include "RecordParser.h"

std::vector<std::string> RecordParser::csv_split(std::string source, char delimeter) {
    std::vector<std::string> ret;
    std::string word = "";
    int start = 0;

    bool inQuote = false;
    bool wasquoted = false;
    for(int i=0; i<source.size(); ++i){
        if(inQuote == false && source[i] == '"'){
            inQuote = true;
            wasquoted = true;
            continue;
        }
        if(inQuote == true && source[i] == '"'){
            if(source.size() > i && source[i+1] == '"'){
                ++i;
            } else {
                inQuote = false;
                continue;
            }
        }

        if(inQuote == false && source[i] == delimeter){
            if (wasquoted) {
                word = '"' + word + '"';
            }
            wasquoted = false;
            ret.push_back(word);
            word = "";
        } else {
            word += source[i];
        }
    }
    ret.push_back(word);

    return ret;
}

std::string RecordParser::csv_pack(std::vector<std::string> &source, char delimiter) {
    std::string res = "";
    for (auto a = source.begin(); a!=source.end();++a)
    {
        res.append(*a);
        res.push_back(delimiter);
    }
    res.pop_back();
    res.push_back('\n');
    return res;
}
