//---------------------------------------------------------------------------

#ifndef AppUtilsH
#define AppUtilsH

#include <cstddef>

#include <anafestica/CfgItems.h>

namespace AppUtils {

extern Anafestica::TConfigNode& GetConfigBaseNode( Anafestica::TConfigNode& RootNode );

template<typename T>
constexpr size_t ToIdx( T Val ) {
    return static_cast<size_t>( Val );
}

//---------------------------------------------------------------------------
} // End of namespace AppUtils
//---------------------------------------------------------------------------
#endif
