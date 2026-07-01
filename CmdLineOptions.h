//---------------------------------------------------------------------------

#ifndef CmdLineOptionsH
#define CmdLineOptionsH

#include "CmdLineParser.h"

enum class Opt {
    Help, Logo, Session,
};

extern CmdLineParser::OptionType Options[3];

//---------------------------------------------------------------------------
#endif
