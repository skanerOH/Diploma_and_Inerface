#include "HuffmanK.h"

HuffmanKCoder::HuffmanKCoder(const std::shared_ptr<ColumnAnalyzer> &colAnal) {
    this->colAnal = colAnal;
    res = std::vector<char>();
    codesCount=0;
}

void HuffmanKCoder::Encode() {
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

HuffLeafCh *HuffmanKCoder::CreateTree() {
    {

        std::vector<HuffLeafCh*> huffVect;
        for (auto f=colAnal->charMap.begin(); f != colAnal->charMap.end(); ++f)
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

char HuffmanKCoder::pack_byte(bool bits[8]) {
    char res(0);
    for (unsigned i(8); i--;)
    {
        res <<=1;
        res |= char(bits[i]);
    }
    return res;
}

void HuffmanKCoder::WriteToFile(std::ofstream &out) {
    //dic size
    unsigned char dicSize = (unsigned char)colAnal->charMap.size();
    out.write((char*)&dicSize, sizeof(unsigned char));

    //dic
    for (auto i=colAnal->charMap.begin(); i!=colAnal->charMap.end(); ++i)
    {
        out.write((char*)&(i->first), sizeof(char));
        out.write((char*)&(i->second), sizeof(unsigned short));
    }

    //res size
    unsigned int ressize = res.size();
    out.write((char*)&ressize, sizeof(unsigned int));

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

HuffmanKDecoder::HuffmanKDecoder(unsigned short vc) {
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
    freqMap = std::map<char, unsigned short>();
    codesVect = std::vector<char>();
    valuesCount = vc;
    codesCount=0;

}

void HuffmanKDecoder::Read(std::ifstream &infile) {
    //dic size
    unsigned char dicSize;
    infile.read((char*)&dicSize, sizeof(unsigned char));
    //dic
    for (unsigned char i=0; i<dicSize; ++i)
    {
        char ch;
        unsigned short freq;
        infile.read((char*)&ch, sizeof(char));
        infile.read((char*)&freq, sizeof(unsigned short));
        freqMap[ch]=freq;
    }

    //reading codes vect size
    unsigned int inpSize;
    infile.read((char*)&inpSize, sizeof(unsigned int));
    //reading codes
    char c;
    for (unsigned int i = 0; i<inpSize; ++i)
    {
        infile.read((char*)&c, sizeof(char));
        codesVect.push_back(c);
    }

}

void HuffmanKDecoder::Decode() {
    std::shared_ptr<HuffLeafCh> TreeRoot=std::shared_ptr<HuffLeafCh>(CreateTree());
    HuffLeafCh* currLeaf=TreeRoot.get();

    std::vector<char> microbuff = std::vector<char>();

    std::deque<bool> boolsToDecode;

    for(auto c=codesVect.begin(); c!=codesVect.end(); ++c)
    {
        for (int i=0; i<8; ++i)
            boolsToDecode.push_back((*c) & (1 << i));

        while (boolsToDecode.size()>0)
        {
            if (currLeaf->isLeaf) {
                if (currLeaf->value == ',')
                {
                    std::string s ="";
                    for (auto ch=microbuff.begin(); ch!=microbuff.end(); ++ch)
                    {
                        s.push_back(*ch);
                    }
                    res->push_back(s);
                    microbuff.clear();
                }
                else
                    microbuff.push_back(currLeaf->value);
                currLeaf=TreeRoot.get();
            } else {
                if (boolsToDecode.front())
                    currLeaf = currLeaf->left;
                else
                    currLeaf = currLeaf->right;
                boolsToDecode.pop_front();
            }
        }
    }

    if (currLeaf->isLeaf) {
        if (currLeaf->value == ',') {
            std::string s = "";
            for (auto ch = microbuff.begin(); ch != microbuff.end(); ++ch) {
                s.push_back(*ch);
            }
            res->push_back(s);
            microbuff.clear();
            s ="";
            res->push_back(s);
        } else
            microbuff.push_back(currLeaf->value);
        currLeaf = TreeRoot.get();
    }

    if (microbuff.size()>0)
    {
        std::string s ="";
        for (auto ch=microbuff.begin(); ch!=microbuff.end(); ++ch)
        {
            s.push_back(*ch);
        }
        res->push_back(s);
        microbuff.clear();
    }

    bool ch = true;
    while (res->size()>valuesCount)
    {
        if (!ch)
        {
            while (res->size()>valuesCount)
            {
                res->pop_back();
            }
            break;
        }
        ch = false;
        for (int i=0; i<res->size()-1; ++i)
        {
            std::string s = res->at(i);
            if (s[0]=='"' && s[s.size()-1]!='"')
            {
                s.append(res->at(i+1));
                res->at(i)=s;
                res->erase(res->begin()+i+1);
                ch = true;
            }
        }
    }
}

HuffLeafCh *HuffmanKDecoder::CreateTree() {
    std::vector<HuffLeafCh*> huffVect;
    for (auto f=freqMap.begin(); f != freqMap.end(); ++f)
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
