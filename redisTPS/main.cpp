#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <hiredis.h>
#include <cstring>
#include <stdexcept>
#include <sys/timeb.h>
#include <unistd.h>
#include <stdlib.h>
//for kbhit() 
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "StringParser.h"

//redis TPS 性能测试 
using namespace std;

//非阻塞键盘按下 
int kbhit(void); 

//redis-cli info 
int getInfo(redisContext *c,string & strVal)
{
	string strCmd = "INFO";
	
	redisReply *reply = (redisReply*) redisCommand(c, strCmd.c_str() );

    if (REDIS_OK != c->err ||  !reply)
	{
		cout << "reply err:" << c->err<< endl;
		return -1;
	}  

    int err = 0;
    if(REDIS_REPLY_STRING == reply->type)
	{
    	strVal.assign(reply->str, reply->len);
    }
	
	freeReplyObject( reply );
	return 0;
}

void parserInfo(const string & info,map<string,string> & mpVal)
{
	StringParser sp('\n',info);
	
	for(int i = 0 ; i < sp.m_vtResult.size(); i++)
	{
		StringParser sp2(':',sp.m_vtResult[i]);
		if(sp2.m_vtResult.size() == 2)
		{
			mpVal.insert(make_pair(sp2.m_vtResult[0],sp2.m_vtResult[1]));
		}
	}
}

//返回redis-cli info中total_commands_processed的结果 
long getCmdProcessNum(redisContext *c)
{
	string strVal;
	getInfo(c,strVal);
	
	map<string,string> mpVal;
	parserInfo(strVal,mpVal);
	
	map<string,string>::iterator  iter = mpVal.find("total_commands_processed");
	
	if(iter != mpVal.end())
	{
		return atol(iter->second.c_str());
	}
	
	cout << "[err] not found total_commands_processed" << endl;
	return 0;
}
//use:g++ main.cpp -I. -L/opt/app/lib -lhiredis

int main(int argc, char *argv[])
{
	redisContext *c;
	
	c = redisConnect("127.0.0.1",6379);
	if (c->err)
 	{
		cout << "Connection error: " << c->errstr << endl;
		return -1;
	}
	
	struct timeb tp;

    int i =0 ;
    char buf[10];
    while( !kbhit() )
    {   
  		long nMSecondStart, nMSecondEnd, nSecondStart, nSecondEnd;
		//得到服务启动时间
	    ftime(&tp);
	    nMSecondStart = tp.millitm;
	    nSecondStart = tp.time;
	    long nNumStart = getCmdProcessNum(c);
    
  		sleep(1);
  		long nNumEnd = getCmdProcessNum(c);
  		
 		ftime(&tp);
    	nMSecondEnd = tp.millitm;
    	nSecondEnd = tp.time;	
    	
    	long useMillSecond  = (nSecondEnd-nSecondStart)*1000+nMSecondEnd-nMSecondStart;
    	float tps = ((nNumEnd - nNumStart) * 1.0 / useMillSecond) * 1000;
    	cout << "Time:"  << setw(3)  << useMillSecond / 1000
    		 << "  Process:" << setw(3) << nNumEnd - nNumStart
    		 << "  TPS:" << setw(3) << setiosflags(ios::fixed) 
	 		 << setprecision(2)  << tps << endl;
    }
	
	redisFree(c);

	return 0;
}

//非阻塞io 
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}
