//---------------------------------------------------------------------------

#pragma hdrstop

#include <algorithm>

#include <tchar.h>

#include <System.SysUtils.hpp>

#include <FMX.Forms.hpp>
#include <FMX.Platform.Win.hpp>

#include <Winapi.Messages.hpp>

#include "TrayIcon.h"

using std::min;

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace Anafestica {
//---------------------------------------------------------------------------

LRESULT WINAPI NewSubclassproc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                                UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    auto uTrayIconClass = reinterpret_cast<TFMXTrayIcon*>( dwRefData );
    if ( uMsg == TFMXTrayIcon::WM_TRAYICON_MESSAGE ) {
        switch ( lParam ) {
            case WM_LBUTTONDOWN:
                if ( uTrayIconClass->OnLClick ) {
                    uTrayIconClass->OnLClick( uTrayIconClass );
                }
                break;
            case WM_RBUTTONDOWN:
                if ( uTrayIconClass->OnRClick ) {
                    uTrayIconClass->OnRClick( uTrayIconClass );
                }
                break;
            case WM_LBUTTONDBLCLK:
                if ( uTrayIconClass->OnDblClick ) {
                    uTrayIconClass->OnDblClick( uTrayIconClass );
                }
                break;
        }
    }
    return ::DefSubclassProc( hWnd, uMsg, wParam, lParam );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

__fastcall TFMXTrayIcon::TFMXTrayIcon( TComponent* AOwner, TWindowHandle* const Handle )
  : TFmxObject( AOwner )
  , handle_(
        Fmx::Platform::Win::WindowHandleToPlatform( Handle )->Wnd
    )
{
    data_.cbSize = sizeof( data_ );
    data_.hWnd = handle_;
    data_.uID = reinterpret_cast<UINT>( data_.hWnd );
    data_.uTimeout = balloonTimeout_;
    data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    data_.uCallbackMessage = WM_TRAYICON_MESSAGE;

    if ( ::IsWindow( handle_ ) ) {
        ::SetWindowSubclass(
            handle_,
            &NewSubclassproc,
            0,
            reinterpret_cast<DWORD_PTR>( this )
        );
    }
}

//---------------------------------------------------------------------------

__fastcall TFMXTrayIcon::~TFMXTrayIcon()
{
    if ( !ComponentState.Contains( csDesigning ) ) {
        SetTray( NIM_DELETE );
        if ( ::IsWindow( handle_ ) ) {
            ::RemoveWindowSubclass(
                handle_,
                &NewSubclassproc,
                0
            );
        }
    }
}
//---------------------------------------------------------------------------

void TFMXTrayIcon::ShowBalloonHint()
{
    data_.uFlags = data_.uFlags | NIF_INFO;
    data_.dwInfoFlags = balloonFlags_;
    UpdateShow();
}
//---------------------------------------------------------------------------

void __fastcall TFMXTrayIcon::SetIcon( String Value )
{
    if ( CompareStr( icon_, Value ) ) {
        icon_ = Value.IsEmpty() ? String( _T( "MAINICON" ) ) : Value;
        UpdateShow();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFMXTrayIcon::SetHint( String Value )
{
    if ( CompareStr( hint_, Value ) ) {
        hint_ = Value;
        Value.SetLength(
            min(
                static_cast<int>( sizeof data_.szTip / sizeof *data_.szTip ) - 1,
                Value.Length()
            )
        );
        ::ZeroMemory( data_.szTip, sizeof data_.szTip );
        StrPLCopy( data_.szTip, Value, Value.Length() );
        UpdateShow();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFMXTrayIcon::SetVisible( bool Value )
{
    if ( visible_ != Value ) {
        visible_ = Value;
        if ( !ComponentState.Contains( csDesigning ) ) {
            SetTray( visible_ ? NIM_ADD : NIM_DELETE );
        }
    }
}
//---------------------------------------------------------------------------

void TFMXTrayIcon::UpdateShow()
{
    if ( !ComponentState.Contains( csDesigning ) ) {
        data_.hIcon =
            ::LoadIcon( HInstance, icon_.c_str() );

        if ( visible_ ) {
            SetTray( NIM_MODIFY );
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFMXTrayIcon::SetBalloonHint( String Value )
{
    if ( CompareStr( balloonHint_, Value ) ) {
        balloonHint_ = Value;
        Value.SetLength(
            min(
                static_cast<int>( sizeof data_.szInfo / sizeof *data_.szInfo ) - 1,
                Value.Length()
            )
        );
        ::ZeroMemory( data_.szInfo, sizeof data_.szInfo );
        StrPLCopy( data_.szInfo, Value, Value.Length() );
        UpdateShow();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFMXTrayIcon::SetBalloonTitle( String Value )
{
    if ( CompareStr( balloonTitle_, Value ) ) {
        balloonTitle_ = Value;
        Value.SetLength(
            min(
                static_cast<int>( sizeof data_.szInfoTitle / sizeof *data_.szInfoTitle ) - 1,
                Value.Length()
            )
        );
        ::ZeroMemory( data_.szInfoTitle, sizeof data_.szInfoTitle );
        StrPLCopy( data_.szInfoTitle, Value, Value.Length() );
        UpdateShow();
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
} // End of namespace Anafestica
//---------------------------------------------------------------------------

