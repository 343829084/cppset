/**
* �ļ����ƣ�StringParser.h
* ժ    Ҫ���ַ���������
  ע��������ַ������ָ���Ӧ�ò���ͷβ�������ͷβ������ֿ�ֵ��
*/

#ifndef STRING_PARSER_H_
#define STRING_PARSER_H_
#include <string>
#include <vector>

using namespace std;

//��ֵ��
struct KeyValue
{
    string strName;
    string strValue;
};

class StringParser
{
public:
    /**
    * ���ù��캯����ʵ�ֽ���
    * @param1 chSep �ָ���
    * @param2 strSource ԭʼ�ַ���
    */
    StringParser(char chSep,string strSource);
    ~StringParser(void);

    //�������÷ָ����ʹ��������ַ�����Ȼ�����
    void ReSet(char chSep,string strSource);

    vector<string> m_vtResult;  //�������

    //���ؽ�����ƴ�ӵ��ַ�����Ĭ��Ӧ�õ���Ԫ�ַ������ú��������޸��˽����������ַ���ƴ��
    string GetPaserdStr() const;

    /**
    * StringParser��ĸ������������ڴ����ֵ�ԵĽ���
    * eg:PEK099:180|SHA777:900�Ľ���
    * ���룺ParserPair('|',':',str,vtResult);
    * @param1 chOut ���ָ��ַ�,���ڷָ��ֵ��
    * @param2 chIn  �ڲ�ָ��ַ������ڷָ����ֵ
    * @param3 ���������ַ���
    * @param4 [out] ������Ľ��
    * @return 0 suss -1 failed;
    */
    static int ParserPair(char chOut,char chIn, const string& strSource,
                          vector<KeyValue> & vtResult);
private:
    //�����ַ�����m_vtResult
    int Parser();

    /**
    * ��ȡ�ַ���str�е�nIndex���ָ�����λ�ã����޵�nIndex���ָ���������-1
    */
    int getDelPos( int nIndex);

    /**
    * ��ȡ�ַ���str�е�nPos���ַ��Ժ����һ���ָ�����λ�ã����ޣ�����-1
    */
    int getNextDelPos( int nPos);
    char m_chsep;   //�ָ��� eg��'|'
    string m_strOrg;    //ԭʼ�ַ���
};

#endif
