//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <memory>
#include <tuple>

#include <anafestica/FileVersionInfo.h>

#include <System.DateUtils.hpp>

#include "FMXFormAppMain.h"

#include "CmdLineOptions.h"
#include "CmdLineParser.h"

#include "AppUtils.h"

using std::make_unique;
using std::min;
using std::get;

using Anafestica::TFileVersionInfo;
using Anafestica::TConfigNode;

using AppUtils::ToIdx;
using AppUtils::GetConfigBaseNode;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

//TfrmPanelAppMain *frmPanelAppMain;

//---------------------------------------------------------------------------

LRESULT WINAPI NewSubclassproc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                                UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    switch ( uMsg ) {
        case WM_SYSCOMMAND:
            if ( wParam == SC_MINIMIZE ) {
                reinterpret_cast<TfrmPanelAppMain*>( dwRefData )->MinimizeToTrayBar();
                return 0;
            }
            break;
    }
    return ::DefSubclassProc( hWnd, uMsg, wParam, lParam );
}
//---------------------------------------------------------------------------

__fastcall TfrmPanelAppMain::TfrmPanelAppMain(TComponent* Owner)
    : TfrmPanelAppMain(
        Owner, StoreOpts::OnlyPos, &GetConfigBaseNode( GetConfigRootNode() )
      )
{
}
//---------------------------------------------------------------------------

__fastcall TfrmPanelAppMain::TfrmPanelAppMain( TComponent* Owner, StoreOpts StoreOptions,
                                               Anafestica::TConfigNode* const RootNode )
    : TConfigRegistryForm( Owner, StoreOpts::OnlyPos, RootNode )
{
    Init();
}
//---------------------------------------------------------------------------

__fastcall TfrmPanelAppMain::~TfrmPanelAppMain()
{
    try {
        Destroy();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::Init()
{
    ::SetWindowSubclass(
        Fmx::Platform::Win::WindowHandleToPlatform( Handle )->Wnd,
        &NewSubclassproc,
        0,
        reinterpret_cast<DWORD_PTR>( this )
    );

    trayIcon_ = std::move( make_unique<TTrayIcon>( nullptr, Handle ) );
    trayIcon_->Icon = _T( "MAINICON" );
    trayIcon_->Hint = _T( "Tabellone" );
    trayIcon_->OnDblClick = &TrayIconDblClick;
    trayIcon_->OnRClick = &TrayIconRClick;

    displays_.reserve( Screen->DisplayCount );

    EnumFMXDisplays( back_inserter( displays_ ) );

    int PrimaryIdx = -1;

    for( auto const & Disp : displays_ ) {
        auto const & FMXDisp = get<ToIdx( FMXWinDisplayDevField::FMXDisplay )>( Disp );

        if ( FMXDisp.Primary ) {
            PrimaryIdx = FMXDisp.Index;
        }

        auto const & GDisp =
            get<ToIdx( FMXWinDisplayDevField::Display )>( Disp );

        comboboxPanelTopLeftScreen->Items->AddObject(
            Format(
                _T( "%s (%s)" ),
                ARRAYOFCONST((
                    GDisp.DeviceString
                  , GDisp.DeviceName
                ))
            ),
            reinterpret_cast<TObject*>(
                const_cast<FMXWinDisplayDevCont::pointer>( &Disp )
            )
        );
    }

    comboboxPanelTopLeftScreen->ItemIndex = PrimaryIdx;

    SetupCaption();

    RestoreProperties();
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::Destroy()
{
    try {
        Stop();
    }
    catch ( ... ) {
    }
    SaveProperties();
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::SetupCaption()
{
    TFileVersionInfo Info( ParamStr( 0 ) );
    Caption =
        Format(
            _T( "%s, %s" ),
            ARRAYOFCONST((
                Info.FileDescription
              , Info.ProductVersion
            ))
        );
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::tmrAutoStartTimer(TObject *Sender)
{
    TTimer& Timer = dynamic_cast<TTimer&>( *Sender );
    Timer.Enabled = false;
    if ( AutoStart ) {
        Start();
        if ( AutoMinimizeOnTray ) {
            MinimizeToTrayBar();
        }
    }
}
//---------------------------------------------------------------------------

String TfrmPanelAppMain::GetDisplayDeviceID() const
{
    if ( FMXWinDisplayDev const * const Display = GetSelectedDisplay() ) {
        return get<ToIdx( FMXWinDisplayDevField::Display )>( *Display ).DeviceID;
    }
    return String();
}
//---------------------------------------------------------------------------

template<typename T>
class DisplayDeviceIDIsType {
public:
    explicit DisplayDeviceIDIsType( T const & ID ) : id_( ID ) {}

    template<typename D>
    bool operator()( D const & Display ) const {
        return SameText(
            get<ToIdx( FMXWinDisplayDevField::Display )>( Display ).DeviceID,
            id_
        );
    }
private:
    T id_;
};

template<typename T>
inline DisplayDeviceIDIsType<T> DisplayDeviceIDIs( T const & ID ) {
    return DisplayDeviceIDIsType<T>( ID );
}

void TfrmPanelAppMain::SetDisplayDeviceID( String Val )
{
    FMXWinDisplayDevCont::const_iterator ci =
        find_if(
            displays_.begin(), displays_.end(), DisplayDeviceIDIs( Val )
        );
    if ( ci != displays_.end() ) {
        comboboxPanelTopLeftScreen->ItemIndex =
            distance(
                static_cast<FMXWinDisplayDevCont const &>( displays_ ).begin(),
                ci
            ) + 1;
    }
    else {
        comboboxPanelTopLeftScreen->ItemIndex = 0;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::RestoreProperties()
{
    // Put code here to restore attribute(s)
    TConfigNode& BaseNode = GetConfigBaseNode( GetConfigRootNode() );

    //RESTORE_VALUE( BaseNode, _T( "DisplayDeviceID" ), DisplayDeviceID );
    String DispDevId = DisplayDeviceID;
    RestoreValue( BaseNode, _T( "DisplayDeviceID" ), DispDevId );
    DisplayDeviceID = DispDevId;

//    AutoStart =
        //BaseNode.GetItem( _T( "AutoStart" ), AutoStart ).operator bool();
    bool AutoS = AutoStart;
    BaseNode.GetItem( _T( "AutoStart" ), AutoS );
    AutoStart = AutoS;

//    AutoMinimizeOnTray =
//        BaseNode.GetItem( _T( "AutoMinimizeOnTray" ), AutoMinimizeOnTray ).operator bool();
    bool AutoMinOnTray = AutoMinimizeOnTray;
    BaseNode.GetItem( _T( "AutoMinimizeOnTray" ), AutoMinOnTray );
    AutoMinimizeOnTray = AutoMinOnTray;

    TConfigNode& PanelNode = BaseNode.GetSubNode( _T( "Panel" ) );

    //PanelClipping =
    //    PanelNode.GetItem( _T( "Clipping" ), PanelClipping ).operator bool();
    bool PnlClipping = PanelClipping;
    PanelNode.GetItem( _T( "Clipping" ), PnlClipping );
    PanelClipping = PnlClipping;

    //PanelScaling =
    //    PanelNode.GetItem( _T( "Scaling" ), PanelScaling ).operator bool();
    bool PnlScaling = PanelScaling;
    PanelNode.GetItem( _T( "Scaling" ), PnlScaling );
    PanelScaling = PnlScaling;

    //PanelKeepAspectRatio =
    //    PanelNode.GetItem( _T( "KeepAspectRatio" ), PanelKeepAspectRatio ).operator bool();
    bool PnlKeepAspectRatio = PanelKeepAspectRatio;
    PanelNode.GetItem( _T( "KeepAspectRatio" ), PnlKeepAspectRatio );
    bool PanelKeepAspectRatio = PnlKeepAspectRatio;
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::SaveProperties() const
{
    // Put code here to restore attribute(s)
    TConfigNode& BaseNode = GetConfigBaseNode( GetConfigRootNode() );

    //SAVE_VALUE( BaseNode, _T( "DisplayDeviceID" ), DisplayDeviceID );
    SaveValue( BaseNode, _T( "DisplayDeviceID" ), DisplayDeviceID );

    //SAVE_VALUE( BaseNode, _T( "AutoStart" ), AutoStart );
    SaveValue( BaseNode, _T( "AutoStart" ), AutoStart );

    //SAVE_VALUE( BaseNode, _T( "AutoMinimizeOnTray" ), AutoMinimizeOnTray );
    SaveValue( BaseNode, _T( "AutoMinimizeOnTray" ), AutoMinimizeOnTray );

    TConfigNode& PanelNode =
        BaseNode.GetSubNode( _T( "Panel" ) );
    //SAVE_VALUE( PanelNode, _T( "Clipping" ), PanelClipping );
    SaveValue( BaseNode, _T( "Clipping" ), PanelClipping );
    //SAVE_VALUE( PanelNode, _T( "Scaling" ), PanelScaling );
    SaveValue( BaseNode, _T( "Scaling" ), PanelScaling );
    //SAVE_VALUE( PanelNode, _T( "KeepAspectRatio" ), PanelKeepAspectRatio );
    SaveValue( BaseNode, _T( "KeepAspectRatio" ), PanelKeepAspectRatio );
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelMonoscopeExecute(TObject *Sender)
{
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->Monoscope = actPanelMonoscope->Checked;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelMonoscopeUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    Act.Enabled = GetPanel();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelClippingExecute(TObject *Sender)
{
    panelClipping_ = !panelClipping_;
    actPanelClipping->Checked = panelClipping_;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MonitorClipping = PanelClipping;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelClippingUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = !Panel || !Panel->WindowMode;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelScalingExecute(TObject *Sender)
{
    panelScaling_ = !panelScaling_;
    actPanelScaling->Checked = panelScaling_;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MonitorScaling = PanelScaling;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelScalingUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = !Panel || !Panel->WindowMode;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelKeepAspectRatioExecute(TObject *Sender)
{
    panelKeepAspectRatio_ = !panelKeepAspectRatio_;
    actPanelKeepAspectRatio->Checked = panelKeepAspectRatio_;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MaintainAspectRatio = PanelKeepAspectRatio;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelKeepAspectRatioUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = !Panel || !Panel->WindowMode;
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::SetPanelClipping( bool Val )
{
    actPanelClipping->Checked = Val;
    panelClipping_ = Val;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MonitorClipping = Val;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::SetPanelScaling( bool Val )
{
    actPanelScaling->Checked = Val;
    panelScaling_ = Val;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MonitorScaling = Val;
    }
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::SetPanelKeepAspectRatio( bool Val )
{
    actPanelKeepAspectRatio->Checked = Val;
    panelKeepAspectRatio_ = Val;
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->MaintainAspectRatio = Val;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actFileStartExecute(TObject *Sender)
{
    Start();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actFileStopExecute(TObject *Sender)
{
    Stop();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::tmrClockTimer(TObject *Sender)
{
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        TDateTime const LocalSystemTime = Now();
        String const Text =
            Format(
                _T( "%s" )
              , ARRAYOFCONST((
                    FormatDateTime( _T( "dddddd - tt" ), LocalSystemTime )
                ))
            );
        Panel->DateTimeString = Text;
    }
}
//---------------------------------------------------------------------------

FMXWinDisplayDev const * TfrmPanelAppMain::GetSelectedDisplay() const
{
    int Idx = comboboxPanelTopLeftScreen->ItemIndex;
    if ( Idx >= 0 ) {
        return reinterpret_cast<FMXWinDisplayDev*>(
            comboboxPanelTopLeftScreen->Items->Objects[Idx]
        );
    }
    throw Exception( _T( "No display selected" ) );
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::Start()
{
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::Stop()
{
    actPanelMonoscope->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelShowExecute(TObject *Sender)
{
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->Visible = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelShowUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = Panel && !Panel->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelHideExecute(TObject *Sender)
{
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->Visible = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelHideUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = Panel && Panel->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::EnabledIfNotRunning(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    Act.Enabled = !GetPanel();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::EnabledIfRunning(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    Act.Enabled = GetPanel();
}
//---------------------------------------------------------------------------

String __fastcall TfrmPanelAppMain::GetInstanceNameEvtHandler( System::TObject* Sender )
{
    return instanceName_;
}
//---------------------------------------------------------------------------

bool __fastcall TfrmPanelAppMain::ReportMessageHandler( System::TObject* Sender,
                                                String Message, bool Error )
{
    TMsgDlgButtons Buttons;

    Buttons = Buttons << TMsgDlgBtn::mbClose;

    int const Ret = MessageDlg(
        Message,
        Error ? TMsgDlgType::mtError : TMsgDlgType::mtInformation,
        Buttons,
        0
    );

    return Ret == mrClose || Ret == mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actFileConfigExecute(TObject *Sender)
{
    Config();
}
//---------------------------------------------------------------------------

HWND FindMainWIndow()
{
    HWND hWnd = NULL;
    DWORD Pid = 0;
    DWORD CurrentPid = ::GetCurrentProcessId();

    do {
        hWnd = ::FindWindowEx( NULL, hWnd, _T( "TFMAppClass" ), NULL );
        if( hWnd ) {
            ::GetWindowThreadProcessId( hWnd, &Pid );
            if ( CurrentPid == Pid ) {
                break;
            }
            else {
                ::Sleep( 1 );
            }
        }
    }
    while ( hWnd );
    return hWnd;
}
//---------------------------------------------------------------------------

void HideAppOnTaskbar()
{
    ::ShowWindow( FindMainWIndow(), SW_HIDE );
}
//---------------------------------------------------------------------------

void ShowAppOnTaskbar()
{
    ::ShowWindow( FindMainWIndow(), SW_SHOW );
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::MinimizeToTrayBar()
{
    HideAppOnTaskbar();
    Hide();
    trayIcon_->Visible = true;
}
//---------------------------------------------------------------------------

void TfrmPanelAppMain::RestoreFromTrayBar()
{
    ShowAppOnTaskbar();
    ::SetWindowPos(
        Fmx::Platform::Win::WindowHandleToPlatform( Handle )->Wnd,
        HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW
    );
    trayIcon_->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::TrayIconDblClick( System::TObject* Sender )
{
    RestoreFromTrayBar();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::TrayIconRClick( System::TObject* Sender )
{
    TPoint Pt;

    ::GetCursorPos( &Pt );

    // Sennò il menu popup rimane sotto la traybar
    ::SetWindowPos(
        Fmx::Platform::Win::WindowHandleToPlatform( Handle )->Wnd,
        HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE //| SWP_SHOWWINDOW
    );

    popupmenuTrayIcon->Popup( Pt.x, Pt.y );
    popupmenuTrayIcon->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actViewOpenMainWindowExecute(TObject *Sender)
{
    RestoreFromTrayBar();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actViewOpenMainWindowUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    Act.Enabled = trayIcon_->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelAutoFitExecute(TObject *Sender)
{
    if ( TfrmPanelBase* const Panel = GetPanel() ) {
        Panel->AutoFit();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actPanelAutoFitUpdate(TObject *Sender)
{
    TAction& Act = dynamic_cast<TAction&>( *Sender );
    TfrmPanelBase* const Panel = GetPanel();
    Act.Enabled = Panel && Panel->WindowMode;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::actFileExitExecute(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

String TfrmPanelAppMain::CreateInstanceName()
{
    GUID Id;
    CheckOSError( CreateGUID( Id ) );
    return Sysutils::GUIDToString( Id );
}
//---------------------------------------------------------------------------

void __fastcall TfrmPanelAppMain::ActionList1Update(TBasicAction *Action, bool &Handled)
{
    comboboxPanelTopLeftScreen->Enabled = !GetPanel();
}
//---------------------------------------------------------------------------

