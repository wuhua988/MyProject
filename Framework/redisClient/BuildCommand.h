/**
 * @brief 生成 redis 指令的代码。
 */

#ifndef __BuildCommand_h__
#define __BuildCommand_h__
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using std::stringstream;
using std::vector;
using std::string;

class BuildCommand
{
public:
    explicit BuildCommand(const string& cmd )
    {
        m_bMake = false;
        m_param.push_back(cmd);
    }
    ~BuildCommand()
    {

    }

    /**
    * @brief operator <<     add a param
    * @param param
    */
    template <typename T>
    inline BuildCommand& operator<<(const T& param)
    {
        std::stringstream str ;
        str << param;
        m_param.push_back(str.str());
        return *this;
    }

	template <typename T>
    inline BuildCommand & operator<<(const vector<T> & data) 
    {
        size_t n = data.size();
	    std::stringstream str ;
        for (size_t i = 0; i < n; ++i)
        {
            m_param.push_back( data[i] );
        }
        return *this;
    }
	
    void makeOprCode(void)
    {
        if ( m_bMake )
        {
            return;
        }
        m_dataString.str("");
        size_t size = m_param.size();
        m_dataString << '*' << size << m_CRLF;
        vector<string>::const_iterator it = m_param.begin();
        for ( ; it != m_param.end(); it++ )
        {
            m_dataString << '$' << it->size() << m_CRLF;
            m_dataString << *it << m_CRLF;
        }
    }

     /**
     * @brief operator string generate a BuildCommand
     */
    operator string()
    {
        makeOprCode();
        return m_dataString.str();
    }

    size_t getLength( void )
    {
        makeOprCode();
        return m_dataString.str().length();
    }

    const char* getData( void )
    {
        makeOprCode();
        return m_dataString.str().data();
    }

private:
    std::stringstream m_dataString;		// 实现各种类型与 string 的互转。
    std::vector<string> m_param;			// 存放一次交互的参数个数
    static const char* m_CRLF;			// 一行的结束标志
    bool m_bMake;	                        // 标记是否已经 makeOprCode
};

#endif // __BuildCommand_h__
