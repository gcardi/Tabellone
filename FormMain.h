//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH

//---------------------------------------------------------------------------

#include <FMX.ActnList.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include "FMXFormAppMain.h"

#include <vector>

#include "FormPanel.h"

//---------------------------------------------------------------------------

class TfrmMain : public TfrmPanelAppMain
{
__published:	// IDE-managed Components
    TTimer *tmrPolling;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TMenuItem *MenuItem3;
    TMenuItem *MenuItem4;
    TMenuItem *MenuItem5;
    TMenuItem *MenuItem6;
    TMenuItem *MenuItem7;
    TMenuItem *MenuItem9;
    TMenuItem *MenuItem10;
    TMenuItem *MenuItem11;
    TMenuItem *MenuItem12;
    TMenuItem *MenuItem13;
    TMenuItem *MenuItem16;
    TMenuItem *MenuItem17;
    TMenuItem *MenuItem18;
    TMenuItem *MenuItem19;
    TMenuItem *MenuItem34;
    TLine *Line1;
    TGridLayout *GridLayout1;
    TAction *actPanelClear;
    TButton *Button4;
    TMenuBar *MenuBar1;
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall actPanelClearExecute(TObject *Sender);
    void __fastcall actPanelClearUpdate(TObject *Sender);


private:	// User declarations
    using PanelType = TfrmPanel;

    std::unique_ptr<TfrmPanel> panel_;
    std::vector<std::unique_ptr<TButton>> btns_;

    void BuildButtons();
    void DestroyButtons();
    void CreatePanel( FMXWinDisplayDev const * Display, bool Clipping,
                      bool Scaling, bool KeepAspectRatio );
    void DestroyPanel();
    void __fastcall NumButtonClick( TObject *Sender );
    void __fastcall NumberChanged( TObject* Sender, int Num, bool Val );
protected:
    virtual void RestoreProperties();
    virtual void SaveProperties() const;
    virtual void Start();
    virtual void Stop();
    virtual TfrmPanelBase* GetPanel() { return panel_.get(); }
    virtual void Config();

public:		// User declarations
    using inherited = TfrmPanelAppMain;

    __fastcall TfrmMain(TComponent* Owner);
    __fastcall ~TfrmMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
