//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_TYPE_NAME_HPP_INCLUDED
#define MOCK_TYPE_NAME_HPP_INCLUDED

#include <stdexcept>
#include <typeinfo>
#ifdef __GNUC__
#include <cxxabi.h>
#include <memory.h>
#endif

namespace mock
{
namespace detail
{
    struct guard
    {
        explicit guard( char* p )
            : p_( p )
        {}
        ~guard()
        {
            free( p_ );
        }
    private:
        char* p_;
    };

    inline std::string type_full_name( const std::type_info& info )
    {
        const char* name = info.name();
#ifdef __GNUC__
        size_t size = 0;
        int status = 0;
        char* result = abi::__cxa_demangle( name, NULL, &size, &status );
        guard g( result );
        if( result )
            return result;
#endif
        return name;
    }

    template< typename T >
    std::string type_name()
    {
        const std::string name = type_full_name( typeid( T ) );
        std::size_t p = name.rfind( "::" );
        if( p != std::string::npos )
            return name.substr( p + 2 );
        p = name.rfind( " " );
        if( p != std::string::npos )
            return name.substr( p + 1 );
        return name;
    }
}
}

#endif // #ifndef MOCK_TYPE_NAME_HPP_INCLUDED
