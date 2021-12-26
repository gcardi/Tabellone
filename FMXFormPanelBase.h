//---------------------------------------------------------------------------

#ifndef FMXFormPanelBaseH
#define FMXFormPanelBaseH

#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>

//---------------------------------------------------------------------------

#include <memory>

#include "anafestica/PersistFormWinFMX.h"
#include "anafestica/CfgRegistrySingleton.h"

#include "FMXWinDisplayDev.h"

//---------------------------------------------------------------------------

using TConfigRegistryForm =
    Anafestica::TPersistFormWinFMX<Anafestica::Registry::TConfigSingleton>;

class TfrmPanelBase : public TConfigRegistryForm
{
__published:	// IDE-managed Components
    TLayout *layoutMain;
    TLabel *lblDateTime;
    TLayout *layoutTitle;
    TLayout *layoutBorder;
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
    void __fastcall FormResize(TObject *Sender);

private:	// User declarations
    FMXWinDisplayDev const * display_;
    HRGN clipReg_ {};
    bool dragging_ {};
    int dragStartLeft_ {};
    int dragStartTop_ {};
    LONG dragStartX_ {};
    LONG dragStartY_ {};
    bool monitorClipping_ {};
    bool monitorScaling_ {};
    bool maintainAspectRatio_ {};
    std::unique_ptr<TImage> monoscope_;
    String dateTimeString_ { _T( "-" ) };

    void Init();
    void Destroy();
    void ClearRegion();
    void RestoreProperties();
    void SaveProperties() const;
    bool GetMonoscope() const;
    void SetMonoscope( bool Val );
    void SetMonitorClipping( bool Val );
    void UpdateMonitorClipping( bool Val );
    void UpdateMonitorScaling( bool Val );
    LONG GetMonitorWidth() const;
    LONG GetMonitorHeight() const;
    bool GetMonitorScaling() const;
    void SetMonitorScaling( bool Val );
    void SetMaintainAspectRatio( bool Val );
    void SetDateTimeString( String Val );
    void UpdateDateTimeControl();

    bool GetWindowMode() const { return !display_; }
public:		// User declarations
    __fastcall TfrmPanelBase( TComponent* Owner,
                              FMXWinDisplayDev const * Display );

    __fastcall TfrmPanelBase( TComponent* Owner,
                              FMXWinDisplayDev const * Display,
                              StoreOpts StoreOptions,
                              Anafestica::TConfigNode* const RootNode = nullptr );

    __fastcall ~TfrmPanelBase();
    __property bool Monoscope = { read = GetMonoscope, write = SetMonoscope };
    __property bool MonitorClipping = { read = monitorClipping_, write = SetMonitorClipping };
    __property bool MonitorScaling = { read = monitorScaling_, write = SetMonitorScaling };
    __property bool MaintainAspectRatio = { read = maintainAspectRatio_, write = SetMaintainAspectRatio };
    __property bool WindowMode = { read = GetWindowMode };
    __property String DateTimeString = { read = dateTimeString_, write = SetDateTimeString };

    void AutoFit();
};

//---------------------------------------------------------------------------
//extern PACKAGE TfrmPanel *frmPanel;
//---------------------------------------------------------------------------
#endif


