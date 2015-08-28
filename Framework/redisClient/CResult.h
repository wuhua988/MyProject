#ifndef __RESULT_H__
#define __RESULT_H__
#include <string>
#include <list>
#include "OSHeaders.h"
using std::string;

typedef enum ReplyType
{
    REDIS_REPLY_STRING,
    REDIS_REPLY_ARRAY,
    REDIS_REPLY_INTEGERER,
    REDIS_REPLY_NIL,
    REDIS_REPLY_STATUS,
    REDIS_REPLY_ERROR
} ReplyType;

class CResult : public std::string
{
public:

    typedef std::list<CResult> ListCResult;

    CResult();

    CResult( const CResult& other );

    CResult( const string& value );

    ~CResult();

    void setType( const ReplyType e );

    ReplyType getType( void ) const ;

    bool addElement(const CResult &ele);

    const ListCResult &getArry( void ) const;

    SInt64 getInt( void ) const;

    string getString( void )const;

    string getErrorString( void ) const;

    string getStatus( void ) const;

    static string display(const CResult &ele, int indent );

    void assign(const CResult &other );

    CResult& operator= ( const string& value );
    CResult& operator= ( const CResult& other );
    void clear();
    /**
     * @brief getTypeString
     * @param e
     * @return get a string to discribe a type of CResult.
     */
    static string getTypeString(ReplyType e);


    friend std::ostream &operator<<(std::ostream& out,  const CResult &value);

private:
    ReplyType _type;
    ListCResult _arry;
};

#endif // __RESULT_H__
