//
// Created by Skaner on 07.03.2021.
//

#include "LZWDecoder.h"

void LZWDecoder::InitDict() {
    dict = std::map<std::string, unsigned short>();
    for (char i=CHAR_MIN; i!=CHAR_MAX; ++i)
    {
        dict[std::string(1, i)]=dict.size();
    }
}

std::string LZWDecoder::GetStrByCode(unsigned short code) {
    for (auto &i : dict)
    {
        if (i.second == code)
            return i.first;
    }
    return "";
}

LZWDecoder::LZWDecoder(unsigned short vc) {
    codesVect = std::vector<unsigned short>();
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
    valuesCount = vc;
    InitDict();

}

void LZWDecoder::Read(std::ifstream &infile) {


    //codesCount
    infile.read((char*)&codesCount, sizeof(unsigned int));

    //codes vector
    unsigned short val;
    for (unsigned int i=0; i<codesCount; ++i)
    {
        infile.read((char*)&val, sizeof(unsigned short));
        codesVect.emplace_back(val);
    }

}


void LZWDecoder::Decode() {
    unsigned short int currCode;
    unsigned short int privCode;
    std::string currStr;
    std::string privStr;
    std::string S;

    std::vector<char> microbuff = std::vector<char>();

    privCode=codesVect.front();
    privStr=GetStrByCode(privCode);
    for (char k : privStr) {
        if (k==',')
        {
            std::string s="";
            for (char c : microbuff)
            {
                s+=c;
            }
            res->push_back(s);
            microbuff.clear();
        } else{
            microbuff.push_back(k);
        }
    }
    char C = privStr[0];

    for (auto currCode=codesVect.begin()+1; currCode!=codesVect.end();++currCode)
    {
        privStr=GetStrByCode(privCode);
        currStr=GetStrByCode(*currCode);

        if (dict.count(currStr))
        {
            S=currStr;
        } else
        {
            S=privStr;
            S=S+std::string(1,C);
        }

        for (char k : S)
            if (k==',')
            {
                std::string s="";
                for (char c : microbuff)
                {
                    s+=c;
                }
                res->push_back(s);
                microbuff.clear();
            } else{
                microbuff.push_back(k);
            }
        C=S[0];
        if (dict.size()<USHRT_MAX-1)
            dict[privStr+std::string(1,C)]=dict.size()+1;
        privCode=*currCode;
    }
    std::string s="";
    for (char c : microbuff)
    {
        s+=c;
    }
    res->push_back(s);

    while (res->size()>valuesCount)
    {
        for (int i=0; i<res->size()-1; ++i)
        {
            std::string s = res->at(i);
            if (s[0]=='"' && s[s.size()-1]!='"')
            {
                s.append(res->at(i+1));
                res->at(i)=s;
                res->erase(res->begin()+i+1);
            }
        }
    }

//    for (char k : S) {
//        if (!microbuff.empty())
//            microbuff.pop_back();
//        else
//        {
//            if (res->back().empty())
//                res->pop_back();
//            else
//                res->back().pop_back();
//        }
//    }
}
