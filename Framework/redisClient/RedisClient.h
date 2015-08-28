#ifndef __RedisClient_h__
#define __RedisClient_h__
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include "SharedPtr.h"
#include "CResult.h"
#include "BuildCommand.h"
#include "log.h"
#include "RedisConnection.h"

#define REDIS_NIL "nil"
typedef enum
{
	DEFAULT,	///< -- A default option.
	NX,				///< -- Only set the key if it does not already exist.
	XX				///< -- Only set the key if it already exist.
} SET_OPTION;

using std::stringstream;
using std::vector;
using std::string;

class RedisConnection;
class RedisPoolCluster;
class RedisClient
{
public:
	typedef std::vector<std::string> VecString;
	typedef std::map<string, string> MapString;
	
	enum datatype 
    {
        datatype_none,      // key doesn't exist
        datatype_string,
        datatype_list,
        datatype_set
    };
	
    explicit RedisClient(RedisPoolCluster* pRedisPoolCluster);
    ~RedisClient();

	//---------------------------------common----------------------------------------

	bool ping( void );

	//-----------------------------------key---------------------------------------------

	/**
	 * @brief keys get all keys matching pattern.
	 * @param pattern [in] The conditions of the matching.
	 * @param keys [out] vector of keys maching pattern
	 * @return The number of keys returned.
	 */
	SInt64 keys( const string& pattern , VecString &values );

	SInt64 del( VecString &keys );
	bool exists( const string& key );

	/**
	 * @brief 作用和 EXPIRE 类似，都用于为 key 设置生存时间。
	 * 不同在于 EXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)。
	 * @param keys [in] name of key
	 * @param timestamp [in] the key will be destroy after this timestamp
	 * @return true for success,当 key 不存在或没办法设置生存时间，返回 false
	 * Example:
	 * redis> SET cache www.google.com
	 * OK
	 * redis> EXPIREAT cache 1355292000     # 这个 key 将在 2012.12.12 过期
	 * (integer) 1
	 * redis> TTL cache
	 * (integer) 45081860
	 */
	bool expireAt( const string& key , const UInt64& timestamp );

	/**
	 * @brief 作用和 EXPIREAT 类似，都用于为 key 设置生存时间,以毫秒为单位。
	 * 不同在于 pEXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)毫秒为单位。
	 * @param keys [in] name of key
	 * @param timestamp [in] the key will be destroy after this timestamp
	 * @return true for success,当 key 不存在或没办法设置生存时间，返回 false
	 */
	bool pExpireAt( const string& key , const UInt64& timestamp );

	/**
	 * @brief 以秒为单位设定,更新生存时间
	 * @param keys [in] name of key
	 * @param seconds [in] the key will be destroy after this second
	 * @return The true for set or reset success,false for failed.
	 */
	bool expire( const string& key , const UInt64& seconds );

	/**
	 * @brief 以毫秒为单位设定,更新生存时间
	 * @param keys [in] name of key
	 * @param msec [in] the key will be destroy after this millisecond
	 * @return The true for set or reset success,false for failed.
	 */
	bool pExpire( const string& key , const UInt64& msec );

	/**
	 * @brief 以秒为单位取得key的剩余生存周期
	 * @param keys [in] The name of key
	 * @return The number of sec before destroyed,-2 no such key,-1 key persist(alive for ever),
	 */
	SInt64 ttl( const string& key );

	/**
	 * @brief 以毫秒为单位取得key的剩余生存周期
	 * @param keys [in] The name of key
	 * @return The number of msec before destroyed,-2 no such key,-1 key persist(alive for ever),
	 */
	SInt64 pttl( const string& key );

	/**
	 * @brief 移除 key 的生存时间，让 key 重新成为一个『持久的』(persistent) key
	 * @param key [in] the key
	 * @return The true for success,fail for key isn't exists or the key is persist already.
	 */
	bool persist( const string& key );

	/**
	 * @brief move key from current DB to dstDBIndex
	 * @param key [in] the key
	 * @param dstDBIndex [in]  destination DB inex
	 * @return true for success,fals for failed if no such key or both DB have this key already.
	 */
	bool move( const string& key , const int& dstDBIndex );

	enum EobjSubCommand
	{
		REFCOUNT = 0, ENCODING, IDLETIME
	};
	/**
	 * @brief 命令允许从内部察看给定 key 的 Redis 对象
	 * @param key [in] the key
	 * @param subcommand [in]  子命令
	 * 		REFCOUNT <key> 返回给定 key 引用所储存的值的次数。此命令主要用于除错。
	 * 		ENCODING <key> 返回给定 key 锁储存的值所使用的内部表示(representation)。
	 *		IDLETIME <key> 返回给定 key 自储存以来的空闲时间(idle， 没有被读取也没有被写入)，以秒为单位。
	 * @return "nil" for no such key, other string for response from redis-server,int type will be convert to string
	 * @warning Throw ArgmentErr exception when input argument error.
	 */
	string object( const EobjSubCommand& subcommand , const string& key );

	/**
	 * @brief 从当前数据库中随机返回(不删除)一个 key 。
	 * @param  n/a
	 * @return "nil" for DB is empty, a random key was return;
	 */
	string randomKey( );

	/**
	 * @brief rename key as newkey
	 * @param  key [in] old key name
	 * @param  newKey [in] new key name
	 * @return true for success,false for failed;
	 * @warning throwing exception of 'ReplyErr' if no such key
	 */
	bool rename( const string& key , const string& newKey );

	/**
	 * @brief 当newKey不存在时,将key改名为newkey
	 * @param  key [in] old key name
	 * @param  newKey [in] new key name
	 * @return true for success,false for  newkey exists already or rename failed;
	 * @warning throwing exception of 'ReplyErr' if no such key
	 */
	bool renameNx( const string& key , const string& newKey );

	/**
	 * @brief 返回键值排序的结果。
	 * @param flag   [in] 0 小到大排序, 1 从大到小排序
	 * @param  key [in] key name
	 * @param  retVec [out] returned values arry
	 * @return true for success,false for  key is empty or nonexistent;
	 * @warning throwing exception of 'ReplyErr', if /key type isn't list nor set/one of the values isn't digital
	 */
	bool sort( const string& key , VecString& values , const bool& desc = false );

	/**
	 * @brief 返回 key 所储存的值的类型
	 * @param  key [in] key name
	 * @param type [out] key 所储存的值的类型
	 * 	none (key不存在)
	 * 	string (字符串)
	 * 	list (列表)
	 * 	set (集合)
	 * 	zset (有序集)
	 * 	hash (哈希表)
	 *
	 * @return true for success,false for failed
	 * @warning throwing exception of 'ReplyErr',
	 */
	bool type( const string& key , string& type );

	/**
	 * @brief 将 key 原子性地从当前redis-server移动到时目标redis-server指定的数据库上
	 * @param  key [in] key name
	 * @param  host [in] host of server
	 * @param  port [in] port of server
	 * @param  db [in] db No
	 * @param  timeout [in] timeout
	 * @return true for success,false for failed
	 * @warning throwing exception of 'ReplyErr',
	 */
	bool migrate( const string& key , const string& host , const uint16_t& port = 6379 ,
			const uint16_t& db = 0 , const uint16_t& timeout = 3 );

	/**
	 * @brief 基于游标的迭代器（cursor based iterator）： SCAN 命令每次被调用之后， 都会向用户返回一个新的游标， 用户在下次迭代时需要使用这个新游标作为 SCAN 命令的游标参数， 以此来延续之前的迭代过程。
	 * @param values [out] 得到的键名
	 * @param index [in] 迭代开始的位置，第一次为0
	 * @param pattern [in] 键名匹配限制
	 * @param count [in] 每次迭代出来的元素个数
	 * @return >0 下次迭代开始的位置，为0时完成本次遍历，<0失败
	 * @warning 不保证每次执行都返回某个给定数量的元素。
	 */
	int scan( VecString& values , const int& index , const string& pattern = "" ,
			const int& count = 10 );

	/**
	 * @brief 序列化给定 key ，并返回被序列化的值，使用 RESTORE 命令可以将这个值反序列化为 Redis 键
	 * @param key [in] the key
	 * @param retStr [out] return server response data
	 * @return true for success,false for failed(no such key)
	 * @warning the return data stored in retStr is binary data!
	 */
	bool dump( const string& key , string& retStr );

	/**
	 * @brief 序列化给定 key ，并返回被序列化的值，使用 RESTORE 命令可以将这个值反序列化为 Redis 键
	 * @param key [in] the key
	 * @param buf [in] return server response data
	 * @param ttl [in] time to alive for this key
	 * @return true for success,false for failed(no such key)
	 * @warning Throw ReplyErr exception when the key already exists
	 */
	bool restore( const string& key , const string& buf , const int& ttl = 0 );
	//-----------------------------script method--------------------------------------
	/**
	 * @brief 通过内置的 Lua 解释器，对 Lua 脚本进行求值。
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param keysVec [in] 表示在脚本中所用到的那些 Redis 键(key)，这些键名参数可以在 Lua 中通过全局变量 KEYS 数组，用 1 为基址的形式访问( KEYS[1] ， KEYS[2] ，以此类推)。
	 * @param argsVec [in] 附加参数 arg [arg ...] ，可以在 Lua 中通过全局变量 ARGV 数组访问，访问的形式和 KEYS 变量类似( ARGV[1] 、 ARGV[2] ，诸如此类)。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool eval( CResult& values , const string& script , const VecString& keysVec = VecString() ,
			const VecString& argsVec = VecString() );

	/**
	 * @brief 对缓存在服务器中的脚本进行求值
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param keysVec [in] 表示在脚本中所用到的那些 Redis 键(key)，这些键名参数可以在 Lua 中通过全局变量 KEYS 数组，用 1 为基址的形式访问( KEYS[1] ， KEYS[2] ，以此类推)。
	 * @param argsVec [in] 附加参数 arg [arg ...] ，可以在 Lua 中通过全局变量 ARGV 数组访问，访问的形式和 KEYS 变量类似( ARGV[1] 、 ARGV[2] ，诸如此类)。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool evalSha( CResult& values , const string& script , const VecString& keysVec =
			VecString() , const VecString& argsVec = VecString() );

	/**
	 * @brief 将脚本 script 添加到脚本缓存中，但并不立即执行这个脚本
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param values [out] 返回脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptLoad( string& values , const string& script );

	/**
	 * @brief 表示校验和所指定的脚本是否已经被保存在缓存当中
	 * @param script [in] 脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptExists( const string& script );

	/**
	 * @brief 清除所有 Lua 脚本缓存。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptFlush( void );

	/**
	 * @brief kill  scripts currently executing
	 * @return true for success,false for failed(BuildCommand failed)
	 * @warning Throw ReplyErr exception when No scripts in executing or the script is writing database.
	 */
	bool scriptKill( void );
	//-----------------------------string method--------------------------------------
	UInt64 append( const string& key , const string& value );

	UInt64 bitcount( const string& key , SInt64 start = 0 , SInt64 end = -1 );

	UInt64 bitop( const string& operation , const string& destkey , VecString& keys );

	SInt64 decr( const string& key );

	SInt64 decrby( const string& key , SInt64 decrement );

	/**
	 * @brief get
	 * @param key
	 * @param value
	 * @return true: get value successful, false: key is not exist.
	 */
	bool get( const string& key , string &value );

	uint8_t getbit( const string& key , uint32_t offset );

	bool getrange( const string& key , SInt64 start , SInt64 end , string &value );

	bool getset( const string& key , const string &value , string &oldvalue );

	SInt64 incr( const string& key );

	SInt64 incrby( const string& key , SInt64 increment );

	bool incrbyfloat( const string& key , float increment, float& value );

	void mget( VecString& keys , CResult& result );

	void mset( RedisClient::MapString &value );

	uint8_t msetnx( RedisClient::MapString &value );

	/**
	 * @brief set set a string type key = value
	 * @param key
	 * @param value
	 * @warning  could throw Poco::Exception and ProtocolErr exception
	 */
	void set( const string& key , const string& value );

	bool setEX( const string& key , const string& value , long time ,
			SET_OPTION opt = DEFAULT );
	bool setPX( const string& key , const string& value , long time ,
			SET_OPTION opt = DEFAULT );

	bool setNX( const string& key , const string& value );
	bool setXX( const string& key , const string& value );

	uint8_t setbit( const string& key , uint32_t offset , const string& value );

	UInt64 setrange( const string& key , uint32_t offset , const string& value );

	UInt64 strlen( const string& key );

	//------------------------------list method--------------------------------------

	void lpush( const string& key , const VecString& value , CResult &result );

	SInt64 lpush( const string& key , const VecString& value );

	/**
	 * @brief lpop
	 * @param key
	 * @param value
	 * @return true: successful. false: key is not exit.
	 */
	void lpop( const std::string &key , CResult &result );

	bool lpop( const std::string &key , string &value );

	//------------------------------hash method-----------------------------------
	/**
	 * @brief hset  insert into a value to hash name is key field is field .
	 * @param key  hash name
	 * @param field hash field
	 * @param value to insert data
	 * @return return 1, a new data insert into hash.return 0, a old data update.
	 */
	UInt8 hset( const string& key , const string& field , const string&value );

	/**
	 * @brief hget
	 * @param key
	 * @param field
	 * @param value
	 * @return true : get value successful,false get value failed.
	 */
	bool hget( const string& key , const string& field , std::string &value );

	UInt64 hdel( const string& key , const VecString& fields );

	bool hexists( const string& key , const string& field );

	UInt64 hgetall( const string& key , MapString& pairs );

	void hincrby( const string& key , const string& field , UInt64 increment ,
			CResult& result );

	UInt64 hincrby( const string& key , const string& field , UInt64 increment );

	float hincrbyfloat( const string& key , const string& field , float increment );

	UInt64 hkeys( const string& key , VecString& values );

	UInt64 hlen( const string& key );

	void hmget( const string& key , const VecString& fields , CResult& result );

	void hmset( const string& key , const MapString& pairs , CResult& result );

	void hmset( const string& key , const MapString& pairs );

	bool hsetnx( const string& key , const string& field , const string& value );

	UInt64 hvals( const string& key , VecString& values );

	/**
	 * @brief hscan
	 * @param key [in]
	 * @param cursor [in] 0: get value from the first. >=1 : get value from the cursor. <0 get value from last time call hscan.
	 * @param values [out] value returned.
	 * @param match [in] It is possible to only iterate elements matching a given glob-style pattern
	 * @param count	[in] Basically with COUNT the user specified the amount of work that should be done at every call in order to retrieve elements from the collection.
	 * @return true:There are some value you don't scan.  false: you have scaned all value.
	 *
	 * eg: get all key between pair_100 and pair_199
	 *       	redis.hscan( "testHash", 0, hscanPairs,"pair_1??" );
	 *			while ( redis.hscan( "testHash", -1, hscanPairs ,"pair_1??") );
	 */
	bool hscan( const string& key , SInt64 cursor , MapString& values , const string& match =
			"" , UInt64 count = 0 );

	//---------------------------Set---------------------------------------------------
	UInt64 sadd( const string& key , const VecString& members );

	UInt64 scard( const string& key );

	UInt64 sdiff( const VecString& keys , VecString& values );

	UInt64 sdiffstore( const string& destKey , const VecString& keys );

	UInt64 sinter( const VecString& keys , VecString& values );

	UInt64 sinterstore( const string& destKey , const VecString& keys );

	/**
	 * @brief sismember
	 * @param key
	 * @param member
	 * @return true : member   ∈ key.  false member ∉ key.
	 */
	bool sismember( const string& key , const string& member );

	UInt64 smembers( const string& key , VecString& members );

	/**
	 * @brief smove
	 * @param source
	 * @param dest
	 * @param member
	 * @return true: move members ok false: move member failed.
	 */
	bool smove( const string& source , const string& dest , const string& member );

	bool spop( const string& key , string& member );

	/**
	 * @brief srandmember
	 * @param key
	 * @param member
	 * @return true: get a member random is successful. false: key is empty
	 */
	bool srandmember( const string& key , string& member );

	/**
	 * @brief srandmember
	 * @param key
	 * @param count		如果 count 为正数，且小于集合基数，那么命令返回一个包含 count 个元素的数组，数组中的元素各不相同。
	 * 								如果 count 大于等于集合基数，那么返回整个集合。
	 *								如果 count 为负数，那么命令返回一个数组，数组中的元素可能会重复出现多次，而数组的长度为 count 的绝对值。
	 * @param members
	 * @return the number of fetched members.
	 */
	UInt64 srandmember( const string& key , int count , VecString& members );

	UInt64 srem( const string& key , VecString& members );

	UInt64 sunion( const VecString& keys , VecString& members );

	UInt64 sunionstroe( const string& dest , const VecString& keys );

	bool sscan( const string& key , SInt64 cursor , VecString& values , const string& match =
			"" , UInt64 count = 0 );
	//---------------------------SortedSet-------------------------------------------

	UInt64 zadd( const string& key , const MapString& pairs );

	UInt64 zcard( const string& key );

	UInt64 zcount( const string& key , const string& min , const string& max );

	string zincrby( const string& key , float increment , const string& member );

	UInt64 zrange( const string& key , const SInt64 start , const SInt64 stop ,
			VecString& reply );
	UInt64 zrange( const string& key , const SInt64 start , const SInt64 stop ,
			MapString& reply );

	UInt64 zrangebyscore( const string& key , const string& min , const string& max ,
			VecString& reply , SInt64 offset = 0 , SInt64 count = 0 );
	UInt64 zrangebyscore( const string& key , const string& min , const string& max ,
			MapString& reply , SInt64 offset = 0 , SInt64 count = 0 );

	bool zrank( const string& key , const string& member , SInt64& reply );	//cunzai nil

	UInt64 zrem( const string& key , VecString& members );

	UInt64 zremrangebyrank( const string& key , const SInt64 start , const SInt64 stop );

	UInt64 zremrangebyscore( const string& key , const string& min , const string& max );

	UInt64 zrevrange( const string& key , const SInt64 start , const SInt64 stop ,
			VecString& reply );
	UInt64 zrevrange( const string& key , const SInt64 start , const SInt64 stop ,
			MapString& reply );

	UInt64 zrevrangebyscore( const string& key , const string& max , const string& min ,
			VecString& reply , SInt64 offset = 0 , SInt64 count = 0 );
	UInt64 zrevrangebyscore( const string& key , const string& max , const string& min ,
			MapString& reply , SInt64 offset = 0 , SInt64 count = 0 );

	UInt64 zrevrank( const string& key , const string& member );

	string zscore( const string& key , const string& member );

	void addAggregate( BuildCommand& cmd , int aggregate );
	UInt64 zunionstore( const string& destination , const UInt64 numkeys ,
			const VecString& keys , const VecString& weigets , int aggregate = 0 );
	UInt64 zunionstore( const string& destination , const UInt64 numkeys ,
			const VecString& keys , int aggregate = 0 );

	UInt64 zinterstore( const string& destination , const UInt64 numkeys ,
			const VecString& keys , const VecString& weigets , int aggregate = 0 );
	UInt64 zinterstore( const string& destination , const UInt64 numkeys ,
			const VecString& keys , int aggregate = 0 );

	bool zscan( const string& key , SInt64 cursor , MapString& reply ,
			const string& match = "" , UInt64 count = 0 );

	UInt64 zrangebylex( const string& key , const string& min , const string& max ,
			VecString& reply , SInt64 offset = 0 , SInt64 count = 0 );

	UInt64 zlexcount( const string& key , const string& min , const string& max );

	UInt64 zremrangebylex( const string& key , const string& min , const string& max );
	//--------------------------transtraction method------------------------------

	void watch( const VecString& keys );

	void unwatch( void );

	void multi( void );

	void discard( void );

	bool transactionCmd( const string& cmd , VecString& params );

	void exec( CResult &result );

	//----------------------------HyperLogLog-------------------------------------------

	//----------------------------pub/sub--------------------------------------------------

	void psubscribe( VecString& pattern , CResult& result );

	UInt64 publish( const string& channel , const string& message );

	UInt64 psubchannels( VecString& value, const VecString& pattern = VecString() );

	UInt64 psubnumsub( RedisClient::MapString& value, const VecString& channel = VecString() );

	UInt64 psubnumpat( );

	void punsubscribe( CResult& result, const VecString& pattern = VecString() );

	void subscribe( VecString& channel , CResult& result );

	void unsubscribe( CResult& result, const VecString& channel = VecString() );

	//-----------------------------Script-----------------------------------------------------

	//-----------------------------connection----------------------------------------------

	//-----------------------------Server---------------------------------------------------

	string bgrewriteaof( );

	string bgsave( );

	string clientGetname( );

	bool clientKill( const string& ip , const uint32_t port );

	void clientList( CResult& result );

	bool clientSetname( const string& connectionName );

	UInt64 configGet( const string& parameter , VecString& reply );

	void configResetstat( );

	bool configRewrite( );

	bool configSet( const string& parameter , const string& value );

	UInt64 dbsize( );

	string debugObject( const string& key );

	void debugSegfault( );

	void flushall( );

	void flushdb( );

	UInt64 info( VecString& reply );

	UInt64 lastsave( );

	void monitor( void* input = NULL , void* output = NULL ,
			void (*p)( string& str , void* in , void* out )=NULL );

	bool save( );

	string shutdown( );

	void slaveof( const string& host , const string& port );

	void slowlog( const VecString& subcommand , CResult& reply );

	void time( string& currentseconds , string& microseconds );

protected:
	template< typename T >
	T _valueFromString( const string& data )
	{
		T value;
		std::istringstream istr(data);
		istr >> value;
		if ( istr.fail())
		{
			LOG_ERROR("convert from string:%s to other type value falied",data.c_str());
		}

		return value;
	}

	void _getStringVecFromArry( const CResult::ListCResult& arry , VecString& values );

	void _getStringMapFromArry( const CResult::ListCResult& arry , MapString& pairs );

	/**
	 * @brief set
	 * @param key
	 * @param value
	 * @param result
	 * @param suffix		"EX","PX"
	 * @param time		expire time.
	 * @param suffix2	"NX","XX"
	 */
	void _set( const string& key , const string& value , CResult& result ,
			const string& suffix = "" , long time = 0 , const string suffix2 = "" );

	/**
	 * @brief _getStatus
	 * @param cmd [in] BuildCommand you want send.
	 * @param data [out] recved from server.
	 * @return true:recv data successful. false: recv empty object.
	 */
	bool _getStatus( BuildCommand &cmd , string &status);
	bool _getInt( BuildCommand &cmd , SInt64 &number);
	bool _getString( BuildCommand &cmd , string &value);
	/**
	 * @brief _getArry
	 * @param cmd
	 * @param result [out] it must arry.
	 * @return
	 */
	bool _getArry( BuildCommand& cmd , CResult& result);
	UInt64 _getArry( BuildCommand &cmd , VecString& values);
	UInt64 _getArry( BuildCommand &cmd , MapString& pairs);

	bool _getReply( CResult& result,RedisConnection* pRedisConn);
    bool _getReply( CResult &result,BuildCommand& cmd);
	/**
	 * @brief _replyBulk
	 * @param result
	 * @param line
	 * @return true: successful false :not exists
	 */
	bool _replyBulk( CResult &result , const string& len,RedisConnection* pRedisConn);

	UInt64 _replyMultiBulk( CResult &result , const std::string &line,RedisConnection* pRedisConn);
private:
	enum
	{
		MAX_LINE_SIZE = 2048, MAX_RECV_SIZE = 1024 * 1024///< The max number of recved data.( 1M  )
	};
	static const char PREFIX_REPLY_STATUS;
	static const char PREFIX_REPLY_ERR;
	static const char PREFIX_REPLY_INT;
	static const char PREFIX_BULK_REPLY;
	static const char PREFIX_MULTI_BULK_REPLY;	
	
private:
    RedisClient(const RedisClient &);
    RedisClient& operator=(const RedisClient &);
	
    RedisPoolCluster* m_pRedisPoolCluster;
private:	
    bool RecvLine(RedisConnection* pRedisConn,std::string& strLine);
    SharedPtr<RedisConnection> GetRedisConnection();
};

#endif // __Command_h__
