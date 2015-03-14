/**
* 文件名称：StringParser.h
* 摘    要：字符串解析类
  注，传入的字符串，分隔符应该不在头尾，如果在头尾，会出现空值项
*/

#ifndef STRING_PARSER_H_
#define STRING_PARSER_H_
#include <string>
#include <vector>

using namespace std;

//键值对
struct KeyValue
{
    string strName;
    string strValue;
};

class StringParser
{
public:
    /**
    * 调用构造函数即实现解析
    * @param1 chSep 分隔符
    * @param2 strSource 原始字符串
    */
    StringParser(char chSep,string strSource);
    ~StringParser(void);

    //重新设置分隔符和待解析的字符串，然后解析
    void ReSet(char chSep,string strSource);

    vector<string> m_vtResult;  //解析结果

    //返回解析后拼接的字符串（默认应该等于元字符串，该函数用于修改了解析结果后的字符串拼接
    string GetPaserdStr() const;

    /**
    * StringParser类的辅助方法，用于处理键值对的解析
    * eg:PEK099:180|SHA777:900的解析
    * 传入：ParserPair('|',':',str,vtResult);
    * @param1 chOut 外层分割字符,用于分割键值对
    * @param2 chIn  内层分割字符，用于分割键和值
    * @param3 待解析的字符串
    * @param4 [out] 解析完的结果
    * @return 0 suss -1 failed;
    */
    static int ParserPair(char chOut,char chIn, const string& strSource,
                          vector<KeyValue> & vtResult);
private:
    //解析字符串到m_vtResult
    int Parser();

    /**
    * 获取字符串str中第nIndex个分隔符的位置，若无第nIndex个分隔符，返回-1
    */
    int getDelPos( int nIndex);

    /**
    * 获取字符串str中第nPos个字符以后的下一个分隔符的位置，若无，返回-1
    */
    int getNextDelPos( int nPos);
    char m_chsep;   //分隔符 eg：'|'
    string m_strOrg;    //原始字符串
};

#endif
