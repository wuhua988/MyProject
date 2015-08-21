#ifndef __RedisClient_h__
#define __RedisClient_h__
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "SharedPtr.h"

using std::stringstream;
using std::vector;
using std::string;

class RedisConnection;
class RedisPoolCluster;
class RedisClient
{
public:
    typedef std::string string_type;
    typedef std::vector<string_type> string_vector;
    typedef std::set<string_type> string_set;

    typedef long int_type;
	static string_type missing_value;
	enum datatype 
    {
        datatype_none,      // key doesn't exist
        datatype_string,
        datatype_list,
        datatype_set
    };
	
    explicit RedisClient(RedisPoolCluster* pRedisPoolCluster);
    ~RedisClient();

    bool Set(const string_type & key, const string_type & value);
    string_type Get(const string_type & key);
    void MGet(const string_vector & keys, string_vector & out);
    bool Exists(const string_type & key);
    void Del(const string_type & key);
private:
    RedisClient(const RedisClient &);
    RedisClient& operator=(const RedisClient &);
	
	RedisPoolCluster* m_pRedisPoolCluster;
private:
    bool        Recv_OK_Reply(RedisConnection* pRedisConn);
    void        Recv_Int_OK_Reply(RedisConnection* pRedisConn);
    std::string Recv_Single_Line_Reply(RedisConnection* pRedisConn);
    int_type    Recv_Bulk_Reply(char prefix,RedisConnection* pRedisConn);
    std::string Recv_Bulk_Reply(RedisConnection* pRedisConn);
    int_type    Recv_Multi_Bulk_Reply(string_vector & out,RedisConnection* pRedisConn);
    int_type    Recv_Multi_Bulk_Reply(string_set & out,RedisConnection* pRedisConn);
    int_type    Recv_Int_Reply(RedisConnection* pRedisConn);
	
	bool RecvLine(RedisConnection* pRedisConn,std::string& strLine);
	SharedPtr<RedisConnection> GetRedisConnection();
};

#endif // __Command_h__
