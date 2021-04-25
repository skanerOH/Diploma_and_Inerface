#include "ColumnAnalyzer.h"

ColumnAnalyzer::ColumnAnalyzer(std::shared_ptr<std::vector<std::string>> colPtr, const bool& usetypes, const bool& uselzw) {
    strMap = std::map<std::string, unsigned short int>();
    charMap = std::map<char, unsigned short>();
    columnValues = colPtr;
    ushortCol = std::vector<unsigned short>();
    intCol = std::vector<int>();
    floatCol = std::vector<float>();
    doubleCol = std::vector<double>();
    useTypes=usetypes;
    useLZW=uselzw;
}

char ColumnAnalyzer::Analyze() {
    bool moveToNext = false;
    //int overdrawVal = columnValues->size()>1000 ? columnValues->size()/50 : columnValues->size()/10;
    int overdrawVal = columnValues->size()/10;
    for (auto val = columnValues->begin(); val!=columnValues->end(); ++val)
    {
        auto iter=strMap.find(*val);
        if (iter==strMap.end())
        {
            strMap[*val]=1;
        } else
        {
            if (strMap[*val]<UINT32_MAX) strMap[*val]++;
        }

        if (strMap.size()>overdrawVal) {
            moveToNext = true;
            break;
        }
    }
    if (!moveToNext)
        return 'h';

    moveToNext = false;

    if (useTypes) {

        for (auto val = columnValues->begin(); val != columnValues->end(); ++val) {
            unsigned short num = 0;
            if (is_Short(*val, num)) {
                ushortCol.push_back(num);
            } else {
                moveToNext = true;
                break;
            }
        }
        if (!moveToNext)
            return 'u';

        ushortCol = std::vector<unsigned short>();
        moveToNext = false;

        for (auto val = columnValues->begin(); val != columnValues->end(); ++val) {
            int num = 0;
            if (is_Int(*val, num)) {
                intCol.push_back(num);
            } else {
                moveToNext = true;
                break;
            }
        }
        if (!moveToNext) {
            return 'i';
        }

        intCol = std::vector<int>();
//        moveToNext = false;
//
//        for (auto val = columnValues->begin(); val != columnValues->end(); ++val) {
//            float num = 0;
//            if (is_Float(*val, num)) {
//                floatCol.push_back(num);
//            } else {
//                moveToNext = true;
//                break;
//            }
//        }
//        if (!moveToNext)
//            return 'f';
//
//        floatCol = std::vector<float>();
//        moveToNext = false;
//
//        for (auto val = columnValues->begin(); val != columnValues->end(); ++val) {
//            double num = 0;
//            if (is_Double(*val, num)) {
//                doubleCol.push_back(num);
//            } else {
//                moveToNext = true;
//                break;
//            }
//        }
//        if (!moveToNext)
//            return 'd';
//
//        doubleCol = std::vector<double>();
    }

    moveToNext = false;

    for (auto val = columnValues->begin(); val != columnValues->end(); ++val) {
        if (!is_Numeric(*val))
        {
            moveToNext = true;
            break;
        }
    }
    if (!moveToNext) {
        return 'n';
    }

    if (useLZW)
        return 'l';

    for (auto i=columnValues->begin(); i!=columnValues->end(); ++i)
    {
        for (auto c=i->begin(); c!=i->end(); ++c)
        {
            if ((++charMap[*c])==USHRT_MAX-1)
                for (auto k=charMap.begin(); k!=charMap.end();++k)
                {
                    k->second=k->second/10+k->second%10;
                }
        }
        if ((++charMap[','])==USHRT_MAX-1)
            for (auto k=charMap.begin(); k!=charMap.end();++k)
            {
                k->second=k->second/10+k->second%10;
            }
    }
    return 'k';
}

bool ColumnAnalyzer::is_Float(const std::string& s, float& res)
{
    if (s.empty())
    {
        res = FLT_MIN;
        return true;
    }

    int before_del = 0;
    int after_del = 0;
    bool was_del = false;
    int signes =0;

    for (auto i=s.begin(); i!=s.end();++i)
    {
        if (std::isdigit(*i))
        {
            if (was_del)
                after_del++;
            else
                before_del++;
        } else
        if (*i=='-')
        {
            signes++;
        }
        else if (*i=='.')
        {
            was_del=true;
        } else
        {
            return false;
        }
    }

    if (signes>2)
        return false;
    if (before_del+after_del+(was_del ? 1 : 0)+signes!=s.size())
        return false;
    if (before_del>=37)
        return false;
    if (after_del>=9)
        return false;

    try
    {
        res = std::stof(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool ColumnAnalyzer::is_Double(const std::string& s, double& res)
{
    if (s.empty())
    {
        res = DBL_MIN;
        return true;
    }

    int before_del = 0;
    int after_del = 0;
    bool was_del = false;
    int signes =0;

    for (auto i=s.begin(); i!=s.end();++i)
    {
        if (std::isdigit(*i))
        {
            if (was_del)
                after_del++;
            else
                before_del++;
        } else
        if (*i=='-')
        {
            signes++;
        }
        else if (*i=='.')
        {
            was_del=true;
        } else
        {
            return false;
        }
    }

    if (signes>2)
        return false;
    if (before_del+after_del+(was_del ? 1 : 0)+signes!=s.size())
        return false;
    if (before_del>=307)
        return false;
    if (after_del>=16)
        return false;
    try
    {
        res = std::stod(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool ColumnAnalyzer::is_Int(const std::string& s, int& res)
{
    if (s.empty())
    {
        res = INT32_MIN;
        return true;
    }

    int digcount=0;
    int signes=0;

    for (auto i=s.begin(); i!=s.end();++i)
    {
        if (std::isdigit(*i))
        {
            digcount++;
        } else
        if (*i=='-')
        {
            signes++;
        } else
        {
            return false;
        }
    }

    if (digcount>9)
        return false;
    if (signes>1)
        return false;
    if (signes+digcount!=s.size())
        return false;

    try
    {
        res = std::stoi(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

bool ColumnAnalyzer::is_Short(const std::string &s, unsigned short &res) {
    {
        if (s.empty())
        {
            res = USHRT_MAX;
            return true;
        }

        int digcount=0;

        for (auto i=s.begin(); i!=s.end();++i)
        {
            if (std::isdigit(*i))
            {
                digcount++;
            } else
            {
                return false;
            }
        }

        if (digcount>5)
            return false;

        int i;
        try
        {
            i = std::stoi(s);
        }
        catch(...)
        {
            return false;
        }

        if (i>USHRT_MAX-1)
            return false;

        res = (unsigned short) i;

        return true;
    }
}

bool ColumnAnalyzer::is_Numeric(const std::string &s) {
    for (auto i=s.begin(); i!=s.end();++i)
    {
        if (!(std::isdigit(*i) || *i==',' || *i=='.' || *i==':' || *i==' ' || *i=='/' || *i=='-'))
            return false;
    }

    return true;
}
