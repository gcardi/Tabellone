//---------------------------------------------------------------------------

#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>

#include "CmdLineOptions.h"
#include "CmdLineParser.h"


//---------------------------------------------------------------------------
USEFORM("FMXFormAppMain.cpp", frmPanelAppMain);
USEFORM("FMXFormPanelBase.cpp", frmPanelBase);
USEFORM("FrameNum.cpp", frmeNum); /* TFrame: File Type */
USEFORM("FormConfig.cpp", frmConfig);
USEFORM("FormMain.cpp", frmMain);
USEFORM("FormPanel.cpp", frmPanel);
USEFORM("DataModStyleRes.cpp", dmStyleRes); /* TDataModule: File Type */
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
    try
    {
        CmdLineParser::ParseCmdLine(
            Options, sizeof Options / sizeof *Options
        );

        if ( CmdLineParser::WasFound( Options[static_cast<size_t>( Opt::Help )] ) ) {
            ShowMessage(
                CmdLineParser::GetHelpText(
                    Options, sizeof Options / sizeof *Options
                ).c_str()
            );
            return 0;
        }
        else {
            CmdLineParser::Validate( Options, sizeof Options / sizeof *Options );
        }

        Application->Initialize();
        Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TdmStyleRes), &dmStyleRes);
         Application->Run();
        while ( Screen->FormCount ) {
           delete Screen->Forms[0];
        }
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
