//---------------------------------------------------------------------------

#ifndef FMXWinDisplayDevH
#define FMXWinDisplayDevH

#include <windows.h>

#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>

#include <tuple>

using FMXWinDisplayDev =
    std::tuple<DISPLAY_DEVICE,DISPLAY_DEVICE,TDisplay>  ;

enum class FMXWinDisplayDevField { Device, Display, FMXDisplay };

template<typename OutIt>
void EnumFMXDisplays( OutIt Out )
{
    DISPLAY_DEVICE Device = { 0 };
    Device.cb = sizeof Device;

    DISPLAY_DEVICE Display = { 0 };
    Display.cb = sizeof Display;

    for ( int Idx = 0 ; Idx < Screen->DisplayCount ; ++Idx ) {
        TDisplay Disp = Screen->Displays[Idx];
        if ( ::EnumDisplayDevices( NULL, Disp.Index, &Device, 0 ) ) {
            if ( ::EnumDisplayDevices( Device.DeviceName, 0, &Display, 0 ) ) {
                *Out++ = std::make_tuple( Device, Display, Disp );
            }
        }
    }
}

//---------------------------------------------------------------------------
#endif
