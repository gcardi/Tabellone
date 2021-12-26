//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <FMX.Filter.Effects.hpp>
#include <FMX.DialogService.Sync.hpp>

#include <memory>

#include "FormMain.h"
#include "FormConfig.h"
#include "AppUtils.h"

using Anafestica::TConfigNode;

using std::make_unique;

using AppUtils::GetConfigBaseNode;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FMXFormAppMain"
#pragma resource "*.fmx"
TfrmMain *frmMain;
//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TfrmPanelAppMain(Owner)
{
    // build buttons
    RestoreProperties();
}
//---------------------------------------------------------------------------

__fastcall TfrmMain::~TfrmMain()
{
    try {
        SaveProperties();
    }
    catch( ... ) {
    }
}
//---------------------------------------------------------------------------


void TfrmMain::RestoreProperties()
{
    TConfigNode& BaseNode = GetConfigBaseNode( GetConfigRootNode() );
    //RESTORE_PROPERTY( BaseNode, ServerName );
    //RESTORE_PROPERTY( BaseNode, ServerPort );
}
//---------------------------------------------------------------------------

void TfrmMain::SaveProperties() const
{
    TConfigNode& BaseNode = GetConfigBaseNode( GetConfigRootNode() );

    //SAVE_PROPERTY( BaseNode, ServerName );
    //SAVE_PROPERTY( BaseNode, ServerPort );
}
//---------------------------------------------------------------------------

void TfrmMain::BuildButtons()
{
    DestroyButtons();
    btns_.reserve( 90 );
    for ( int Idx = 1 ; Idx <= 90 ; ++Idx ) {
        auto Btn = make_unique<TButton>( nullptr );
        auto Effect = new TContrastEffect( Btn.get() );
        Btn->Parent = GridLayout1;
        Btn->Text = Idx;
        Btn->Tag = Idx;
        Btn->OnClick = &NumButtonClick;
        Effect->Parent = Btn.get();
        Effect->Enabled = false;
        btns_.push_back( std::move( Btn ) );
    }
}
//---------------------------------------------------------------------------

void TfrmMain::DestroyButtons()
{
    btns_.clear();
}
//---------------------------------------------------------------------------

void TfrmMain::Start()
{
    inherited::Start();
    BuildButtons();
    CreatePanel(
        GetSelectedDisplay(), PanelClipping, PanelScaling, PanelKeepAspectRatio
    );
    Width += 330;
}
//---------------------------------------------------------------------------

void TfrmMain::Stop()
{
    Width -= 330;
    if ( GetPanel() ) {
        tmrPolling->Enabled = false;
        inherited::Stop();
        DestroyPanel();
        DestroyButtons();
    }
}
//---------------------------------------------------------------------------

void TfrmMain::CreatePanel( FMXWinDisplayDev const * Display, bool Clipping,
                            bool Scaling, bool KeepAspectRatio )
{
    panel_ =
        make_unique<PanelType>(
            nullptr
          , Display
          , Display ? StoreOpts::None : StoreOpts::All
//          , &GetConfigRootNode().GetSubNode( _T( "Panel" ) )
          , &GetConfigBaseNode( GetConfigRootNode() ).GetSubNode( _T( "Panel" ) )
          , &NumberChanged
        );

    panel_->Show();
    panel_->MonitorClipping = Clipping;
    panel_->MonitorScaling = Scaling;
    panel_->MaintainAspectRatio = KeepAspectRatio;
//    panel_->OnNumberChanged = &NumberChanged;

    if ( auto BtnNum = panel_->CurrNum ) {
        btns_[BtnNum - 1]->SetFocus();
    }
}
//---------------------------------------------------------------------------

void TfrmMain::DestroyPanel()
{
    panel_.reset();
}
//---------------------------------------------------------------------------

void TfrmMain::Config()
{
    auto Form = make_unique<TfrmConfig>( nullptr );
    Form->AutoStart = AutoStart;
    Form->AutoMinimizeOnTray = AutoMinimizeOnTray;
    if ( Form->ShowModal() == mrOk ) {
        AutoStart = Form->AutoStart;
        AutoMinimizeOnTray = Form->AutoMinimizeOnTray;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    Stop();
    CanClose = true;
}
//---------------------------------------------------------------------------

static void SetButtonContrast( TButton& Btn, bool Enabled )
{
    for ( auto Child : Btn.Children ) {
        if ( auto Eff = dynamic_cast<TContrastEffect*>( Child ) ) {
            Eff->Enabled = Enabled;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::NumButtonClick(TObject *Sender)
{
    auto Btn = static_cast<TButton*>( Sender );

    if ( panel_ ) {
        //auto const State = panel_->ToggleTab( Btn->Tag );
        panel_->ToggleTab( Btn->Tag );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::NumberChanged( TObject* Sender, int Num, bool Val )
{
    auto& Btn = *btns_[Num-1];
    SetButtonContrast( Btn, Val );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actPanelClearExecute(TObject *Sender)
{
//
    auto const Reply = TDialogServiceSync::MessageDialog(
        _T( "Are you sure?" ),
        TMsgDlgType::mtConfirmation,
        TMsgDlgButtons() << TMsgDlgBtn::mbYes << TMsgDlgBtn::mbNo,
        TMsgDlgBtn::mbNo,
        {}
    );
    if ( Reply == mrYes ) {
        panel_->Clear();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actPanelClearUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    Act.Enabled = GetPanel();
}
//---------------------------------------------------------------------------


