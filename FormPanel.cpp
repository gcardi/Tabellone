//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <System.IOUtils.hpp>

#include <algorithm>
#include <memory>

#include <cassert>

#include "FormPanel.h"
#include "DataModStyleRes.h"
#include "CmdLineOptions.h"

using std::make_unique;
using std::swap;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FMXFormPanelBase"
#pragma link "FMXFormPanelBase"
#pragma resource "*.fmx"
//TfrmPanel *frmPanel;
//---------------------------------------------------------------------------

__fastcall TfrmPanel::TfrmPanel( TComponent* Owner,
                                 FMXWinDisplayDev const * Display,
                                 StoreOpts StoreOptions,
                                 Anafestica::TConfigNode* const RootNode,
                                 TNumEvent OnNumberChanged )
    : TfrmPanelBase( Owner, Display, StoreOptions, RootNode )
    , onNumberChanged_{ OnNumberChanged }
{
    auto It = begin( numFrames_ );
    for ( int Idx = 1 ; Idx <= 90 ; ++Idx ) {
        auto NewFrame = make_unique<TfrmeNum>( nullptr );
        NewFrame->Text1->Text = Idx;
        NewFrame->Parent = GridLayout1;
        numFrames_[Idx-1] = std::move( NewFrame );
    }

    LoadExternalLogo();

    RestoreProperties();
}
//---------------------------------------------------------------------------

void TfrmPanel::LoadExternalLogo()
{
    // Carica il logo con questa precedenza:
    //   1) file indicato dall'opzione da riga di comando "-logo=<file>";
    //   2) "Logo.png" nella cartella dell'eseguibile;
    //   3) immagine incorporata nel form (risorse dell'EXE).
    // Se un file non esiste o non e' caricabile, si passa al livello successivo.
    auto const TryLoad = [this]( String const & FileName ) -> bool {
        if ( !FileName.IsEmpty() && Ioutils::TFile::Exists( FileName ) ) {
            try {
                Image1->Bitmap->LoadFromFile( FileName );
                return true;
            }
            catch ( Exception const & ) {
                // formato non supportato o file illeggibile: si prosegue col fallback
            }
        }
        return false;
    };

    auto const & LogoOpt = Options[static_cast<size_t>( Opt::Logo )];
    if ( CmdLineParser::WasFound( LogoOpt )
         && TryLoad( CmdLineParser::GetValue( LogoOpt ) ) ) {
        return;
    }

    TryLoad(
        Ioutils::TPath::Combine(
            Ioutils::TPath::GetDirectoryName( ParamStr( 0 ) ), _D( "Logo.png" )
        )
    );
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
    RESTORE_LOCAL_PROPERTY( TabsText );
}
//---------------------------------------------------------------------------

void TfrmPanel::SaveProperties() const
{
    // Put code here to save attribute(s)
    SAVE_LOCAL_PROPERTY( TabsText );
}
//---------------------------------------------------------------------------

String TfrmPanel::GetTabsText() const
{
    auto SB = make_unique<TStringBuilder>();
    for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
        SB->Append(
            String(
                GetTab( Idx ) ? currNum_ == Idx ? _D( "B" ) : _D( "T" ) : _D( "F" )
            )
        );
    }
    return SB->ToString();
}
//---------------------------------------------------------------------------

void TfrmPanel::SetTabsText( String Val )
{
    Val = Val.Trim();
    if ( Val.Length() == 90 ) {
        int CurrNum {};
        for ( size_t Idx = 1 ; Idx <= 90 ; ++Idx ) {
            switch( Val[Idx] ) {
                case _D( 'T' ):
                    SetTab( Idx, true );
                    break;
                case _D( 'B' ):
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

