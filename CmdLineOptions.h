//---------------------------------------------------------------------------

#ifndef CmdLineOptionsH
#define CmdLineOptionsH

#include "CmdLineParser.h"

enum class Opt {
    Help, Session
};

extern CmdLineParser::OptionType Options[2];

//---------------------------------------------------------------------------
#endif
