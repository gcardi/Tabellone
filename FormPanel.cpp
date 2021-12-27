//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <algorithm>
#include <memory>

#include <cassert>

#include "FormPanel.h"
#include "DataModStyleRes.h"

using std::make_unique;
using std::swap;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FMXFormPanelBase"
#pragma link "FMXFormPanelBase"
#pragma resource "*.fmx"
//TfrmPanel *frmPanel;
//---------------------------------------------------------------------------

/*
__fastcall TfrmPanel::TfrmPanel( TComponent* Owner,
                                 FMXWinDisplayDev const * Display,
                                 Anafestica::TConfigNode* const RootNode )
    : TfrmPanel( Owner, Display, StoreOpts::All, RootNode )
{
}
*/
//---------------------------------------------------------------------------

__fastcall TfrmPanel::TfrmPanel( TComponent* Owner,
                                 FMXWinDisplayDev const * Display,
                                 StoreOpts StoreOptions,
                                 Anafestica::TConfigNode* const RootNode,
                                 TNumEvent OnNumberChanged )
    : TfrmPanelBase( Owner, Display, StoreOptions, RootNode )
    , onNumberChanged_{ OnNumberChanged }
{
//    RestoreProperties();

    auto It = begin( numFrames_ );
    for ( int Idx = 1 ; Idx <= 90 ; ++Idx ) {
        auto NewFrame = make_unique<TfrmeNum>( nullptr );
        NewFrame->Text1->Text = Idx;
        NewFrame->Parent = GridLayout1;
        numFrames_[Idx-1] = std::move( NewFrame );
    }

    RestoreProperties();
}
//---------------------------------------------------------------------------

__fastcall TfrmPanel::~TfrmPanel()
{
    try {
        SaveProperties();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

void TfrmPanel::RestoreProperties()
{
    // Put code here to restore attribute(s)
//    RESTORE_LOCAL_PROPERTY( CurrNum );
    RESTORE_LOCAL_PROPERTY( TabsText );
}
//---------------------------------------------------------------------------

void TfrmPanel::SaveProperties() const
{
    // Put code here to save attribute(s)
    SAVE_LOCAL_PROPERTY( TabsText );
//    SAVE_LOCAL_PROPERTY( CurrNum );
}
//---------------------------------------------------------------------------

String TfrmPanel::GetTabsText() const
{
/*
    auto SB = make_unique<TStringBuilder>();
    for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
        SB->AppendFormat(
            _T( "%s" ),
            ARRAYOFCONST(( GetTab( Idx ) ? _T( "T" ) : _T( "F" ) ))
        );
    }
    return SB->ToString();
*/
    auto SB = make_unique<TStringBuilder>();
    for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
        SB->Append(
            String(
                GetTab( Idx ) ? currNum_ == Idx ? _T( "B" ) : _T( "T" ) : _T( "F" )
            )
        );
    }
    return SB->ToString();
}
//---------------------------------------------------------------------------

void TfrmPanel::SetTabsText( String Val )
{
/*
    Val = Val.Trim();
    if ( Val.Length() == 90 ) {
        auto const OldCurrNum = currNum_;
        for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
            if ( Idx != OldCurrNum ) {
                SetTab( Idx, Val[Idx] == _T( 'T' ) );
            }
        }
        //currNum_ = OldCurrNum;
        if ( OldCurrNum ) {
            SetTab( OldCurrNum, Val[OldCurrNum] == _T( 'T' ) );
        }
        else {
            currNum_ = 0;
        }
    }
*/
    Val = Val.Trim();
    if ( Val.Length() == 90 ) {
        int CurrNum {};
        for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
            switch( Val[Idx] ) {
                case _T( 'T' ):
                    SetTab( Idx, true );
                    break;
                case _T( 'B' ):
                    if ( !CurrNum ) {
                        CurrNum = Idx;
                    }
                    break;
                default:
                    SetTab( Idx, false );
                    break;
            }
        }
        if ( CurrNum ) {
            SetTab( CurrNum, true );
        }
    }
}
//---------------------------------------------------------------------------

bool TfrmPanel::GetTab( size_t Idx ) const
{
    assert( Idx >= 1 && Idx <= 90 );
    return states_[Idx - 1];
}
//---------------------------------------------------------------------------

bool TfrmPanel::SetTab( size_t Idx, bool Val )
{
    assert( Idx >= 1 && Idx <= 90 );

    const size_t Ofs = Idx - 1;

    swap( Val, states_[Ofs] );

    if ( currNum_ ) {
        const size_t CurrOfs = currNum_ - 1;
        if ( states_[CurrOfs] ) {
            numFrames_[CurrOfs]->GradientAnimation1->Stop();
            numFrames_[CurrOfs]->GradientAnimation1->AutoReverse = false;
            numFrames_[CurrOfs]->GradientAnimation1->Loop = false;
            numFrames_[CurrOfs]->GradientAnimation1->Start();
            currNum_ = 0;
        }
    }

    if ( Val ^ states_[Idx-1] ) {
        if ( Val ) {
            numFrames_[Ofs]->GradientAnimation1->Stop();
            numFrames_[Ofs]->GradientAnimation1->Inverse = true;
            numFrames_[Ofs]->GradientAnimation1->AutoReverse = false;
            numFrames_[Ofs]->GradientAnimation1->Loop = false;
            numFrames_[Ofs]->GradientAnimation1->Start();
        }
        else {
            currNum_ = Idx;
            numFrames_[Ofs]->GradientAnimation1->Inverse = false;
            numFrames_[Ofs]->GradientAnimation1->AutoReverse = true;
            numFrames_[Ofs]->GradientAnimation1->Loop = true;
            numFrames_[Ofs]->GradientAnimation1->Start();
        }
    }

    NotifyNumberChanged( Idx, states_[Ofs] );

    return Val;
}
//---------------------------------------------------------------------------

bool TfrmPanel::ToggleTab( size_t Idx )
{
    return SetTab( Idx, !GetTab( Idx ) );
}
//---------------------------------------------------------------------------

void TfrmPanel::NotifyNumberChanged( int Num, bool Val )
{
    if ( onNumberChanged_ ) {
        onNumberChanged_( this, Num, states_[Num - 1] );
    }
}
//---------------------------------------------------------------------------

void TfrmPanel::Clear()
{
    currNum_ = 0;
    for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
        SetTab( Idx, false );
    }
}
//---------------------------------------------------------------------------

