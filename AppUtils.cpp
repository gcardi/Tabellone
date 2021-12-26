//---------------------------------------------------------------------------

#pragma hdrstop

#include "AppUtils.h"
#include "CmdLineParser.h"
#include "CmdLineOptions.h"

using Anafestica::TConfigNode;

//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace AppUtils {

TConfigNode& GetConfigBaseNode( TConfigNode& RootNode )
{
    return CmdLineParser::WasFound( Options[static_cast<size_t>( Opt::Session )] ) ?
             RootNode.GetSubNode(
               CmdLineParser::GetValue( Options[static_cast<size_t>( Opt::Session )] ).c_str()
             )
           :
             RootNode;
}

//---------------------------------------------------------------------------
} // End of namespace AppUtils
//---------------------------------------------------------------------------



