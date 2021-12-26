//---------------------------------------------------------------------------

#pragma hdrstop

// si usa boost::tuple e non std::boost tuple per problemi di move semantics
// con String
#include <boost/tuple/tuple.hpp>

#include "CmdLineOptions.h"

using boost::make_tuple;

using CmdLineParser::OptionType;

//---------------------------------------------------------------------------
#pragma package(smart_init)

OptionType Options[2] = {
    make_tuple(
      /* Name          */  _T( "help" ),
      /* Desc          */  _T( "This help" ),
      /* Found         */  false,
      /* Mandatory     */  false,
      /* ValueRequired */  false,
      /* ValueDesc     */  _T( "" ),
      /* ValueLongDesc */  _T( "" ),
      /* Value         */  _T( "" )
    ),
    make_tuple(
      /* Name          */  _T( "session" ),
      /* Desc          */  _T( "Define a separate configuration session" ),
      /* Found         */  false,
      /* Mandatory     */  false,
      /* ValueRequired */  true,
      /* ValueDesc     */  _T( "Session name" ),
      /* ValueLongDesc */  _T( "the session name to differentiate the configuration" ),
      /* Value         */  _T( "" )
    ),
};
