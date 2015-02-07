#include <iostream>
#include <vector>
#include <string>
#include <hiredis.h>
#include <cstring>
#include <stdexcept>

//redis mset 二进制数据接口实现 

using namespace std;

void msetNotBinary(redisContext *c, const vector<string> &vtKey, const vector<string> & vtVal )
{
	if(vtKey.size() != vtVal.size())
	{
		throw runtime_error( "Redis error" );
	}

	string strCmd = "MSET";
	for(int i = 0; i < vtKey.size(); i++)
	{
	    strCmd += " "+vtKey[i]+" "+vtVal[i];
	}
	cout << "strCmd:" << strCmd << endl;

	void * r =  redisCommand(c, strCmd.c_str() );
	if ( !r )
		throw runtime_error( "Redis error" );
	freeReplyObject( r );
}
/*
/opt/app/colin$./redis-cli get D
"AB\x00DE"
/opt/app/colin$./redis-cli get D
"AB"
*/

//use:g++ main.cpp -I. -L/opt/app/msg6/lib -lhiredis
void mset1( redisContext *c, const vector<string> &vtKey, const vector<string> & vtVal )
{
	if(vtKey.size() != vtVal.size())
	{
		throw runtime_error( "Redis error" );
	}
	
	char ** argv = new char*[vtKey.size() + vtVal.size() + 1 ];
	size_t * argvlen = new size_t[vtKey.size() + vtVal.size() + 1 ];
	
	int j = 0;
	//static char msetcmd[] = "MSET";
	argv[j] = new char[5];
	memcpy(argv[j],"MSET",4);
	argvlen[j] = 4;
	++j;
	

	for(int i = 0 ; i < vtKey.size();i++)
	{	
		argvlen[j] = vtKey[i].length();
		argv[j] = new char[argvlen[j]];
	 	memset((void*)argv[j],0,argvlen[j] );
		memcpy((void*)argv[j],vtKey[i].data(),vtKey[i].length());
		j++;
		
		argvlen[j] = vtVal[i].length();
		argv[j] = new char[argvlen[j]];
		memset((void*)argv[j],0,argvlen[j]);
		memcpy((void*)argv[j],vtVal[i].data(),vtVal[i].length());
		j++;
	}
	
	//if not use const_cast<const char**> ,compile error
	//for why assign from char** to const char** error,please see ...
 	void *r = redisCommandArgv(c, vtKey.size() + vtVal.size() + 1, const_cast<const char**>(argv), argvlen );
	if ( !r )
		throw runtime_error( "Redis error" );
	freeReplyObject( r );
	
	for(int i = 0;i < vtKey.size();i++)
	{
		delete [] argv[i];
		argv[i] = NULL;
	}
	
	delete []argv;
	delete []argvlen;
	argv = NULL;
}

void mset2( redisContext *c, const vector<string> &vtKey, const vector<string> & vtVal)
{
	if(vtKey.size() != vtVal.size())
	{
		throw runtime_error( "Redis error" );
	}
		
	vector<const char *> argv( vtKey.size() + vtVal.size() + 1 );
	vector<size_t> argvlen( vtKey.size() +  vtVal.size() + 1 );
	int j = 0;
	
	static char msetcmd[] = "MSET";
	argv[j] = msetcmd;
	argvlen[j] = sizeof(msetcmd)-1;
	++j;
	
	for(int i = 0;i< vtKey.size();++i)
	{
		argvlen[j] = vtKey[i].length();
		argv[j] = new char[argvlen[j]];
	 	memset((void*)argv[j],0,argvlen[j] );
		memcpy((void*)argv[j],vtKey[i].data(),vtKey[i].length());
		j++;
		
		argvlen[j] = vtVal[i].length();
		argv[j] = new char[argvlen[j]];
		memset((void*)argv[j],0,argvlen[j]);
		memcpy((void*)argv[j],vtVal[i].data(),vtVal[i].length());
		j++;
	}
	
	void *r = redisCommandArgv(c, argv.size(), &(argv[0]), &(argvlen[0]) );
	if ( !r )
		throw runtime_error( "Redis error" );
	freeReplyObject( r );
}
 
void do_test( redisContext *c )
{	
	vector<string> vtKey;
	vector<string> vtVal;
	
	vtKey.push_back("A");
	vtVal.push_back("AAAA");
	vtKey.push_back("B");
	vtVal.push_back("BBBB");
	vtKey.push_back("C");
	vtVal.push_back("CCCC");
	//add a binary data
	vtKey.push_back("D");
	vtVal.push_back("");
	char a[] = "ABCDE";
	a[2] = 0;
	vtVal[3].assign(a,5);
	
	try
	{
		msetNotBinary(c, vtKey, vtVal );
		//mset1( c, vtKey, vtVal );
		//mset2( c, vtKey, vtVal );
	}
	catch ( runtime_error & )
	{
		cout << "Error" << endl;
	}
}


int main(int argc, char *argv[])
{
	redisContext *c;
	
	c = redisConnect("127.0.0.1",6379);
	if (c->err)
 	{
		cout << "Connection error: " << c->errstr << endl;
		return -1;
	}
	
	do_test(c);
	
	redisFree(c);

	return 0;
}
