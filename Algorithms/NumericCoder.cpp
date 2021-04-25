#include "NumericCoder.h"



NumericCoder::NumericCoder(const std::shared_ptr<ColumnAnalyzer> &colAnal) {
    this->colAnal = colAnal;
    res = std::vector<char>();
    codesCount=0;
}

void NumericCoder::Encode() {
    std::shared_ptr<HuffLeafCh> TreeRoot=std::shared_ptr<HuffLeafCh>(CreateTree());
    std::map<char, std::string> encMap;


    std::vector<char> chars = std::vector<char>();

    for (auto str = colAnal->columnValues->begin(); str!=colAnal->columnValues->end(); ++str)
    {
        for (auto c=str->begin(); c!=str->end(); ++c)
        {
            chars.emplace_back(*c);
        }
        chars.emplace_back(',');
    }

    chars.pop_back();

    codesCount = chars.size();

    std::vector<bool> codedBools;
    std::string codeS;
    for (auto c = chars.begin(); c!=chars.end(); ++c)
    {
        if (!encMap.count(*c))
            encMap[*c]=TreeRoot->GetCode(*c);
        codeS=encMap[*c];
        for (int j=0; j<codeS.size(); ++j)
            codedBools.push_back(codeS[j]=='1');
        if (codedBools.size()>=8)
        {
            char s(0);
            bool bits[8];
            for (int i=0; i<8; ++i) {
                bits[i] = codedBools.front();
                codedBools.erase(codedBools.begin());
            }
            s = pack_byte(bits);
            res.push_back(s);
        }
    }
    if (codedBools.size()>0) {
        while (codedBools.size() < 8)
            codedBools.push_back(false);
        char s(0);
        bool bits[8];
        for (int i = 0; i < 8; ++i) {
            bits[i] = codedBools.front();
            codedBools.erase(codedBools.begin());
        }
        s = pack_byte(bits);
        res.push_back(s);
    }

}

HuffLeafCh *NumericCoder::CreateTree() {
    {
        std::map<char, unsigned short> templateData;
        templateData['0']=1;
        templateData['1']=1;
        templateData['2']=1;
        templateData['3']=1;
        templateData['4']=1;
        templateData['5']=1;
        templateData['6']=1;
        templateData['7']=1;
        templateData['8']=1;
        templateData['9']=1;
        templateData['/']=1;
        templateData['.']=1;
        templateData[' ']=1;
        templateData[':']=1;
        templateData[',']=1;
        templateData['-']=1;


        std::vector<HuffLeafCh*> huffVect;
        for (auto f=templateData.begin(); f != templateData.end(); ++f)
        {
            huffVect.push_back(new HuffLeafCh(f->first, nullptr, f->second));
        }

        while (huffVect.size()>1)
        {
            std::sort(huffVect.begin(), huffVect.end(), [](const HuffLeafCh* lhs, const HuffLeafCh* rhs) {
                return lhs->freq>rhs->freq;});
            HuffLeafCh* rightLeaf=huffVect.back();
            huffVect.pop_back();
            HuffLeafCh* leftLeaf=huffVect.back();
            huffVect.pop_back();
            HuffLeafCh* fuzed= new HuffLeafCh(rightLeaf, leftLeaf);
            rightLeaf->parent=fuzed;
            leftLeaf->parent=fuzed;
            huffVect.push_back(fuzed);
        }

        return huffVect.at(0);
    }
}

char NumericCoder::pack_byte(bool bits[8]) {
    char res(0);
    for (unsigned i(8); i--;)
    {
        res <<=1;
        res |= char(bits[i]);
    }
    return res;
}

void NumericCoder::WriteToFile(std::ofstream &out) {
    //res size
    out.write((char*)&codesCount, sizeof(unsigned int));

    //codes
    unsigned int from=0;
    char* buff=new char[USHRT_MAX];
    while (from<res.size())
    {
        if (from+USHRT_MAX<res.size())
        {
            for (int i=0; i<USHRT_MAX; ++i)
            {
                buff[i]=res.at(from+i);
            }
            out.write(buff, sizeof(char)*USHRT_MAX);
            from+=USHRT_MAX;
        }
        else {
            for (int i=0; i<res.size()-from; ++i)
            {
                buff[i]=res.at(from+i);
            }
            out.write(buff, sizeof(char)*(res.size()-from));
            from=res.size();
        }
    }
}

