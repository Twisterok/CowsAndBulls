#ifndef _UTILS_H_
#define _UTILS_H_
#include <utils/definitions.h>
#include <memory>

namespace utils
{
    template<typename Derived, typename Base>
    std::unique_ptr<Derived> 
    dynamic_unique_ptr_cast( std::unique_ptr<Base>& p )
    {
        std::unique_ptr<Derived> result(nullptr);
        if(Derived *wetResult = dynamic_cast<Derived *>(p.get())) 
        {
            p.release();
            result.reset(wetResult);
            return std::move(result);
        }
        return std::move(result);
    }        

    ODBCParameters	getODBCParameters(std::string	odbc);
    void			getCurrentUserName(std::string& UserName);
    bool            GetPath(const std::string& fullPath, std::string& result);
    std::string     getFileVersion(std::string fileName);
}

#endif
