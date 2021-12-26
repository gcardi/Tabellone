// ---------------------------------------------------------------------------

#ifndef FMXFormAppMainH

#define FMXFormAppMainH
#include <FMX.ActnList.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Menus.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>

// ---------------------------------------------------------------------------

#include <memory>
#include <vector>

#include "anafestica/PersistFormWinFMX.h"
#include "anafestica/CfgRegistrySingleton.h"

#include "TrayIcon.h"
#include "FormPanel.h"
#include "FMXWinDisplayDev.h"
#include "FMXFormPanelBase.h"

// ---------------------------------------------------------------------------

using TConfigRegistryForm =
    Anafestica::TPersistFormWinFMX<Anafestica::TConfigRegistrySingleton>;

class TfrmPanelAppMain : public TConfigRegistryForm {
__published: // IDE-managed Components

    TActionList *ActionList1;
    TAction *actPanelMonoscope;
    TAction *actPanelClipping;
    TAction *actPanelScaling;
    TAction *actPanelKeepAspectRatio;
    TTimer *tmrClock;
    TAction *actFileStart;
    TAction *actFileStop;
    TButton *Button5;
    TButton *Button7;
    TTimer *tmrAutoStart;
    TLayout *layoutMain;
    TButton *Button8;
    TEdit *edtEventCode;
    TButton *Button11;
    TButton *Button1;
    TAction *actPanelShow;
    TAction *actFileConfig;
    TSwitch *switchPanelKeepAspectRatio;
    TSwitch *switchPanelClipping;
    TSwitch *switchPanelScaling;
    TStatusBar *StatusBar1;
    TLabel *Label3;
    TLabel *Label8;
    TLabel *Label9;
    TAction *actPanelHide;
    TButton *Button2;
    TPopupMenu *popupmenuTrayIcon;
    TMenuItem *MenuItem8;
    TAction *actViewOpenMainWindow;
    TMenuItem *MenuItem23;
    TMenuItem *MenuItem24;
    TMenuItem *MenuItem25;
    TMenuItem *MenuItem26;
    TMenuItem *MenuItem27;
    TMenuItem *MenuItem28;
    TMenuItem *MenuItem29;
    TMenuItem *MenuItem30;
    TMenuItem *MenuItem31;
    TButton *Button3;
    TMenuItem *MenuItem32;
    TMenuItem *MenuItem33;
    TAction *actPanelAutoFit;
    TMenuItem *MenuItem35;
    TAction *actFileExit;
    TMenuItem *MenuItem36;
    TMenuItem *MenuItem37;
    TComboBox *comboboxPanelTopLeftScreen;
    TLabel *Label1;

    void __fastcall actPanelMonoscopeExecute(TObject *Sender);
    void __fastcall actPanelClippingExecute(TObject *Sender);
    void __fastcall actPanelScalingExecute(TObject *Sender);
    void __fastcall actPanelKeepAspectRatioExecute(TObject *Sender);
    void __fastcall actFileStartExecute(TObject *Sender);
    void __fastcall actFileStopExecute(TObject *Sender);
    void __fastcall tmrClockTimer(TObject *Sender);
    void __fastcall tmrAutoStartTimer(TObject *Sender);
    void __fastcall actPanelShowExecute(TObject *Sender);
    void __fastcall actPanelShowUpdate(TObject *Sender);
    void __fastcall actPanelKeepAspectRatioUpdate(TObject *Sender);
    void __fastcall actPanelScalingUpdate(TObject *Sender);
    void __fastcall actPanelMonoscopeUpdate(TObject *Sender);
    void __fastcall EnabledIfNotRunning(TObject *Sender);
    void __fastcall EnabledIfRunning(TObject *Sender);
    void __fastcall actFileConfigExecute(TObject *Sender);
    void __fastcall actPanelClippingUpdate(TObject *Sender);
    void __fastcall actPanelHideExecute(TObject *Sender);
    void __fastcall actPanelHideUpdate(TObject *Sender);
    void __fastcall actViewOpenMainWindowExecute(TObject *Sender);
    void __fastcall actViewOpenMainWindowUpdate(TObject *Sender);
    void __fastcall actPanelAutoFitExecute(TObject *Sender);
    void __fastcall actPanelAutoFitUpdate(TObject *Sender);
    void __fastcall actFileExitExecute(TObject *Sender);
    void __fastcall ActionList1Update(TBasicAction *Action, bool &Handled);

private: // User declarations
    using TTrayIcon = Anafestica::TFMXTrayIcon  ;
    using FMXWinDisplayDevCont = std::vector<FMXWinDisplayDev>;

    WNDPROC oldWndProc_;
    FMXWinDisplayDevCont displays_;
    std::unique_ptr<TTrayIcon> trayIcon_;

    bool panelClipping_ { true };
    bool panelScaling_{ true };
    bool panelKeepAspectRatio_{ true };
    bool autoStart_{ false };
    bool autoMinimizeOnTray_{ false };
    String instanceName_{ CreateInstanceName() };

    static String CreateInstanceName();
    void Init();
    void Destroy();
    void SetupCaption();
    void SetPanelClipping( bool Val );
    void SetPanelScaling( bool Val );
    void SetPanelKeepAspectRatio( bool Val );
    String GetDisplayDeviceID() const;
    void SetDisplayDeviceID( String Val );

    void SetPanelForceMonitorFullScreen( bool Val );

    void StartMulticast();
    void StopMulticast();
    void __fastcall MultiCastOnReadEvtHandler( System::TObject* Sender, String Text );

    void __fastcall TrayIconDblClick( System::TObject* Sender );
    void __fastcall TrayIconRClick( System::TObject* Sender );

    void RestoreFromTrayBar();

    void RestoreProperties();
    void SaveProperties() const;
protected:
    virtual void Start();
    virtual void Stop();
    virtual TfrmPanelBase* GetPanel() = 0;
    virtual void Config() = 0;

    String __fastcall GetInstanceNameEvtHandler( System::TObject* Sender );
    bool __fastcall ReportMessageHandler( System::TObject* Sender,
                                          String Message, bool Error );
    FMXWinDisplayDev const * GetSelectedDisplay() const;
    void EnableAlarm();
    void DisableAlarm();

    __property bool PanelClipping = {
        read = panelClipping_, write = SetPanelClipping
    };
    __property bool PanelScaling = {
        read = panelScaling_, write = SetPanelScaling
    };
    __property bool PanelKeepAspectRatio = {
        read = panelKeepAspectRatio_, write = SetPanelKeepAspectRatio
    };
    __property bool AutoStart = { read = autoStart_, write = autoStart_ };
    __property bool AutoMinimizeOnTray = {
        read = autoMinimizeOnTray_, write = autoMinimizeOnTray_
    };
    __property String DisplayDeviceID = {
        read = GetDisplayDeviceID, write = SetDisplayDeviceID
    };

public: // User declarations
    __fastcall TfrmPanelAppMain( TComponent* Owner );
    __fastcall TfrmPanelAppMain( TComponent* Owner, StoreOpts StoreOptions,
                                 Anafestica::TConfigNode* const RootNode = nullptr );
    __fastcall ~TfrmPanelAppMain();
    __property WNDPROC OldWindowProc = { read = oldWndProc_ };
    void MinimizeToTrayBar();
};

// ---------------------------------------------------------------------------
//extern PACKAGE TfrmPanelAppMain *frmPanelAppMain;
// ---------------------------------------------------------------------------
#endif
