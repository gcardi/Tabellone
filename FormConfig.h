//---------------------------------------------------------------------------

#ifndef FormConfigH
#define FormConfigH

//---------------------------------------------------------------------------

#include <FMX.ActnList.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>

//---------------------------------------------------------------------------

class TfrmConfig : public TForm
{
__published:	// IDE-managed Components
    TLayout *Layout1;
    TLayout *Layout2;
    TLayout *Layout3;
    TTreeView *tvMain;
    TLabel *Label1;
    TTabControl *tabcontrolMain;
    TButton *Button1;
    TButton *Button2;
    TActionList *ActionList1;
    TAction *actOK;
    TAction *actCancel;
    TTreeViewItem *tvitemGeneral;
    TTabItem *tabItemGeneral;
    TSwitch *switchAutoStart;
    TLabel *Label4;
    TSwitch *switchAutoMinimizeOnTray;
    TLabel *Label6;
    TLabel *Label5;
    void __fastcall actOKExecute(TObject *Sender);
    void __fastcall actOKUpdate(TObject *Sender);
    void __fastcall actCancelExecute(TObject *Sender);
    void __fastcall tvMainChange(TObject *Sender);
private:	// User declarations
    bool GetAutoStart() const;
    void SetAutoStart( bool Val );
    bool GetAutoMinimizeOnTray() const;
    void SetAutoMinimizeOnTray( bool Val );

public:		// User declarations
    __fastcall TfrmConfig(TComponent* Owner);
    __fastcall ~TfrmConfig();
    __property bool AutoStart = {
        read = GetAutoStart, write = SetAutoStart
    };
    __property bool AutoMinimizeOnTray = {
        read = GetAutoMinimizeOnTray, write = SetAutoMinimizeOnTray
    };
};

//---------------------------------------------------------------------------
//extern PACKAGE TfrmConfig *frmConfig;
//---------------------------------------------------------------------------
#endif
