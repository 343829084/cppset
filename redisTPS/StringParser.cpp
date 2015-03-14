#include "StringParser.h"

StringParser::StringParser(char chSep, string strSource)
{
    m_chsep = chSep;
    m_strOrg = strSource;
    //���뼴����
    Parser();
}

StringParser::~StringParser(void)
{
}

void StringParser::ReSet(char chSep,string strSource)
{
    m_chsep = chSep;
    m_strOrg = strSource;
    Parser();
}

int StringParser::Parser()
{
    m_vtResult.clear();

    int nBeginPos = 0;
    int nEndPos = 0;
    string strTmp = "";

    //û�ҵ��ָ���������Ϊֻ��һ��ֵ��ֱ�ӷ���
    if((nBeginPos = getDelPos(1)) == -1)
    {
        m_vtResult.push_back(m_strOrg);
        return 0;
    }
    if(nBeginPos == 0)
        m_vtResult.push_back("");
    else
        m_vtResult.push_back(m_strOrg.substr(0,nBeginPos));

    //��һ���ָ���
    while((nEndPos = getNextDelPos(nBeginPos)) != -1)
    {
        if(nBeginPos + 1 == nEndPos)
            m_vtResult.push_back("");
        else
            m_vtResult.push_back(m_strOrg.substr(nBeginPos+1, nEndPos-nBeginPos-1));
        nBeginPos = nEndPos;
    }

    //���һ��
    if(nBeginPos +1 == m_strOrg.length())
        m_vtResult.push_back("");
    else
        m_vtResult.push_back(m_strOrg.substr(nBeginPos+1));

    return 0;
}

string StringParser::GetPaserdStr() const
{
    string strTmp;
    int nSize = m_vtResult.size();
    for(int i = 0 ; i < nSize; i++)
    {
        if(i == 0)
            strTmp += m_vtResult[i];
        else
            strTmp += m_chsep + m_vtResult[i];
    }
    return strTmp;
}

/**
* ��ȡ�ַ���str�е�nIndex���ָ�����λ�ã����޵�nIndex���ָ���������-1
*/
int StringParser::getDelPos( int nIndex)
{
    int nPos = -1;
    int nCount = 0;
    for(int i=0; i<m_strOrg.length(); i++)
    {
        if(m_strOrg[i] == m_chsep)
        {
            nCount++;
        }
        if(nCount == nIndex)
        {
            nPos = i;
            break;
        }
    }
    return nPos;
}

/**
* ��ȡ�ַ���str�е�nPos���ַ��Ժ����һ���ָ�����λ�ã����ޣ�����-1
*/
int StringParser::getNextDelPos( int nPos)
{
    for(int i=nPos+1; i<m_strOrg.length(); i++)
    {
        if(m_strOrg[i] == m_chsep ) return i;
    }
    return -1;
}

int StringParser::ParserPair(char chOut,char chIn, const string& strSource,
                             vector<KeyValue> & vtResult)
{
    vtResult.clear();
    StringParser sp(chOut,strSource);
    for(int i = 0 ; i< sp.m_vtResult.size(); i++)
    {
        StringParser sp2(chIn, sp.m_vtResult[i]);
        if(sp2.m_vtResult.size() != 2)
            continue;
        KeyValue stPair;
        stPair.strName = sp2.m_vtResult[0];
        stPair.strValue = sp2.m_vtResult[1];
        vtResult.push_back(stPair);
    }

    return 0;
}
