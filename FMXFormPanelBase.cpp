//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <memory>
#include <tuple>

#include <DateUtils.hpp>
#include <FMX.Platform.Win.hpp>

#include "FMXFormPanelBase.h"
#include "AppUtils.h"

using std::make_unique;
using std::get;

using Fmx::Platform::Win::TWinWindowHandle;
using Fmx::Platform::Win::WindowHandleToPlatform;

using AppUtils::ToIdx;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
//TfrmPanel *frmPanel;
//---------------------------------------------------------------------------

__fastcall TfrmPanelBase::TfrmPanelBase( TComponent* Owner,
                                 FMXWinDisplayDev const * Display,
                                 StoreOpts StoreOptions,
                                 Anafestica::TConfigNode* const RootNode )
    : TConfigRegistryForm( Owner, StoreOptions, RootNode )
    , display_( Display )
{
    Init();
}
//---------------------------------------------------------------------------

__fastcall TfrmPanelBase::~TfrmPanelBase()
{
    try {
        Destroy();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

LONG TfrmPanelBase::GetMonitorWidth() const
{
    if ( display_ ) {
        TDisplay const & FMXDisp =
            get<ToIdx( FMXWinDisplayDevField::FMXDisplay )>( *display_ );
        //return FMXDisp.BoundsRect.Width();
        return FMXDisp.Bounds.Width();
    }
    else {
        return const_cast<TfrmPanelBase*>( this )->ClientWidth;
    }
}
//---------------------------------------------------------------------------

LONG TfrmPanelBase::GetMonitorHeight() const
{
    if ( display_ ) {
        TDisplay const & FMXDisp =
            get<ToIdx( FMXWinDisplayDevField::FMXDisplay )>( *display_ );

        return
            maintainAspectRatio_ ?
              static_cast<LONG>(
                  static_cast<float>( GetMonitorWidth() ) /
                  ( static_cast<float>( FormFactor->Width ) / FormFactor->Height )
              )
            :
              //FMXDisp.BoundsRect.Height();
              FMXDisp.Bounds.Height();
    }
    else {
        return
            maintainAspectRatio_ ?
              static_cast<float>(
                  const_cast<TfrmPanelBase*>( this )->ClientWidth
              ) /
              ( static_cast<float>( FormFactor->Width ) / FormFactor->Height )
            :
              static_cast<float>(
                const_cast<TfrmPanelBase*>( this )->ClientHeight
              );
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::Init()
{
    lblDateTime->Visible = false;

    if ( display_ ) {
        Width = 1920;
        Height = 1080;

        Cursor = crNone;
        layoutMain->Cursor = crNone;
        layoutTitle->Cursor = crNone;

        BorderStyle = TFmxFormBorderStyle::None;

        TDisplay const & FMXDisp =
            get<ToIdx( FMXWinDisplayDevField::FMXDisplay )>( *display_ );

        //Left = FMXDisp.BoundsRect.Left;
        //Top = FMXDisp.BoundsRect.Top;
        Left = FMXDisp.Bounds.Left;
        Top = FMXDisp.Bounds.Top;

        UpdateMonitorClipping( monitorClipping_ );
        UpdateMonitorScaling( monitorScaling_ );
    }
    else {
        BorderStyle = TFmxFormBorderStyle::Sizeable;
        Cursor = crDefault;
        layoutMain->Cursor = crDefault;
        layoutTitle->Cursor = crDefault;
        FormStyle = TFormStyle::Normal;
        RESTORE_LOCAL_PROPERTY( ClientWidth );
        RESTORE_LOCAL_PROPERTY( ClientHeight );
    }

    RestoreProperties();
    UpdateDateTimeControl();
}
//---------------------------------------------------------------------------

void TfrmPanelBase::Destroy()
{
    SaveProperties();
    if ( !display_ ) {
        SAVE_LOCAL_PROPERTY( ClientWidth );
        SAVE_LOCAL_PROPERTY( ClientHeight );
    }
    ClearRegion();
}
//---------------------------------------------------------------------------

void TfrmPanelBase::ClearRegion()
{
    if ( clipReg_ ) {
        ::DeleteObject( clipReg_ );
        clipReg_ = 0;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::RestoreProperties()
{
    // Put code here to restore attribute(s)
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SaveProperties() const
{
    // Put code here to save attribute(s)
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelBase::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
    POINT CursorPoint;

    Win32Check( GetCursorPos( &CursorPoint ) );

    if ( Button == TMouseButton::mbLeft && Shift.Contains( ssCtrl ) ) {
        dragging_ = true;
        dragStartLeft_ = Left;
        dragStartTop_ = Top;
        dragStartX_ = CursorPoint.x;
        dragStartY_ = CursorPoint.y;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelBase::FormMouseMove(TObject *Sender, TShiftState Shift, float X,
          float Y)
{
    if ( dragging_ ) {
        POINT CursorPoint;

        Win32Check( GetCursorPos( &CursorPoint ) );
        const LONG Dx = CursorPoint.x - dragStartX_;
        const LONG Dy = CursorPoint.y - dragStartY_;
        Left = dragStartLeft_ + Dx;
        Top = dragStartTop_ + Dy;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelBase::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
    dragging_ = false;
}
//---------------------------------------------------------------------------

bool TfrmPanelBase::GetMonoscope() const
{
    return monoscope_.get();
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SetMonoscope( bool Val )
{
    if ( Val ) {
        TDisplay const & FMXDisp =
            get<ToIdx( FMXWinDisplayDevField::FMXDisplay )>( *display_ );

        auto MonoscopeStream = make_unique<TResourceStream>(
            (unsigned)HInstance, "MONOSCOPE_1920x1080_PNG", RT_RCDATA
        );
        auto MonoscopeBitmap = make_unique<TBitmap>( MonoscopeStream.get() );
        monoscope_ = std::move( make_unique<TImage>( nullptr ) );
        monoscope_->Name = "imgMonoscope";
        monoscope_->Bitmap->Assign( MonoscopeBitmap.get() );
        monoscope_->Align = TAlignLayout::None;
        monoscope_->Parent = layoutMain;
        monoscope_->Position->X = 0;
        monoscope_->Position->Y = 0;
        monoscope_->Size->Height = layoutMain->Height;
        monoscope_->Size->Width = layoutMain->Width;
        monoscope_->BringToFront();
    }
    else {
        monoscope_.reset();
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SetMonitorClipping( bool Val )
{
    if ( monitorClipping_ != Val ) {
        UpdateMonitorClipping( Val );
        monitorClipping_ = Val;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::UpdateMonitorClipping( bool Val )
{
    if ( display_  ) {
        if ( Val ) {
            TWinWindowHandle* hWnd = WindowHandleToPlatform( Handle );
            ClearRegion();
            clipReg_ = ::CreateRectRgn( 0, 0, GetMonitorWidth(), GetMonitorHeight() );
            if ( clipReg_ ) {
                if ( !::SetWindowRgn( hWnd->Wnd, clipReg_, true ) ) {
                    ClearRegion();
                }
            }
            else {
                ::SetWindowRgn( hWnd->Wnd, 0, true );
            }
        }
        else if ( clipReg_ ) {
            TWinWindowHandle* hWnd = WindowHandleToPlatform( Handle );
            ::SetWindowRgn( hWnd->Wnd, 0, true );
            ClearRegion();
        }
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::UpdateMonitorScaling( bool Scale )
{
    if ( display_ ) {
        if ( Scale ) {
            layoutMain->Scale->X =
                static_cast<float>( GetMonitorWidth() ) / layoutMain->Width;
            layoutMain->Scale->Y =
                static_cast<float>( GetMonitorHeight() ) / layoutMain->Height;
        }
        else {
            layoutMain->Scale->X = 1.0;
            layoutMain->Scale->Y = 1.0;
        }
    }
    else if ( Scale ) {
        if ( maintainAspectRatio_ ) {
            double const LayoutFormFactor =
                (double)layoutMain->Width / (double)layoutMain->Height;
            double const WindowFormFactor =
                (double)ClientWidth / (double)ClientHeight;

            if ( WindowFormFactor > LayoutFormFactor ) {
                layoutMain->Scale->Y =
                    static_cast<float>( ClientHeight ) / layoutMain->Height;
                layoutMain->Scale->X = layoutMain->Scale->Y;
            }
            else {
                layoutMain->Scale->X =
                    static_cast<float>( ClientWidth ) / layoutMain->Width;
                layoutMain->Scale->Y = layoutMain->Scale->X ;
            }
        }
        else {
            layoutMain->Scale->X =
                static_cast<float>( GetMonitorWidth() ) / layoutMain->Width;
            layoutMain->Scale->Y =
                static_cast<float>( GetMonitorHeight() ) / layoutMain->Height;
        }
    }
    else {
        layoutMain->Scale->X = 1.0;
        layoutMain->Scale->Y = 1.0;
    }
}
//---------------------------------------------------------------------------

bool TfrmPanelBase::GetMonitorScaling() const
{
    return display_ ? monitorScaling_ : true;
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SetMonitorScaling( bool Val )
{
    if ( monitorScaling_ != Val ) {
        if ( display_ ) {
            UpdateMonitorScaling( Val );
        }
        monitorScaling_ = Val;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SetMaintainAspectRatio( bool Val )
{
    if ( maintainAspectRatio_ != Val ) {
        maintainAspectRatio_ = Val;
        UpdateMonitorClipping( monitorClipping_ );
        UpdateMonitorScaling( monitorScaling_ );
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::SetDateTimeString( String Val )
{
    Val = Trim( Val );
    if ( Val != dateTimeString_ ) {
        dateTimeString_ = Val;
        UpdateDateTimeControl();
    }
    lblDateTime->Visible = true;
}
//---------------------------------------------------------------------------

void TfrmPanelBase::UpdateDateTimeControl()
{
    lblDateTime->Text = dateTimeString_;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelBase::FormResize(TObject *Sender)
{
    if ( !display_ ) {
        UpdateMonitorScaling( true );
    }
}
//---------------------------------------------------------------------------

void TfrmPanelBase::AutoFit()
{
    if ( !display_ ) {
        double const LayoutFormFactor =
            (double)layoutMain->Width / (double)layoutMain->Height;
        double const WindowFormFactor =
            (double)ClientWidth / (double)ClientHeight;
        if ( WindowFormFactor > LayoutFormFactor ) {
            ClientWidth = ClientHeight * LayoutFormFactor;
        }
        else {
            ClientHeight = ClientWidth / LayoutFormFactor;
        }
    }
}
//---------------------------------------------------------------------------


