//---------------------------------------------------------------------------

#pragma hdrstop

// si usa boost::tuple e non std::tuple per problemi di move semantics
// con String sui compilatori bcc32c e bcc64 (con bcc64x funzionano std::tuple)
#include <boost/tuple/tuple.hpp>

#include "CmdLineOptions.h"

using boost::make_tuple;

using CmdLineParser::OptionType;

//---------------------------------------------------------------------------
#pragma package(smart_init)

OptionType Options[3] = {
    make_tuple(
      /* Name          */  _D( "help" ),
      /* Desc          */  _D( "This help" ),
      /* Found         */  false,
      /* Mandatory     */  false,
      /* ValueRequired */  false,
      /* ValueDesc     */  _D( "" ),
      /* ValueLongDesc */  _D( "" ),
      /* Value         */  _D( "" )
    ),
    make_tuple(
      /* Name          */  _D( "logo" ),
      /* Desc          */  _D( "Specify the file to use for the logo" ),
      /* Found         */  false,
      /* Mandatory     */  false,
      /* ValueRequired */  true,
      /* ValueDesc     */  _D( "The file name full-path" ),
      /* ValueLongDesc */  _D( "the file (PNG, JPEG/JPG, BMP, GIF, TIFF) for the logo" ),
      /* Value         */  _D( "" )
    ),
    make_tuple(
      /* Name          */  _D( "session" ),
      /* Desc          */  _D( "Define a separate configuration session" ),
      /* Found         */  false,
      /* Mandatory     */  false,
      /* ValueRequired */  true,
      /* ValueDesc     */  _D( "A session name" ),
      /* ValueLongDesc */  _D( "the session name to differentiate the configuration" ),
      /* Value         */  _D( "" )
    ),
};
