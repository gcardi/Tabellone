//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include <tchar.h>

#include "FormConfig.h"
#include "DataModStyleRes.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
//TfrmConfig *frmConfig;

//---------------------------------------------------------------------------

__fastcall TfrmConfig::TfrmConfig(TComponent* Owner )
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

__fastcall TfrmConfig::~TfrmConfig()
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::actOKExecute(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::actOKUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::actCancelExecute(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::tvMainChange(TObject *Sender)
{
    if ( tabcontrolMain ) {
        tabcontrolMain->ActiveTab =
            tabcontrolMain->Tabs[tvMain->Selected->Index];
    }
}
//---------------------------------------------------------------------------

bool TfrmConfig::GetAutoStart() const
{
    return switchAutoStart->IsChecked;
}
//---------------------------------------------------------------------------

void TfrmConfig::SetAutoStart( bool Val )
{
    switchAutoStart->IsChecked = Val;
}
//---------------------------------------------------------------------------

bool TfrmConfig::GetAutoMinimizeOnTray() const
{
    return switchAutoMinimizeOnTray->IsChecked;
}
//---------------------------------------------------------------------------

void TfrmConfig::SetAutoMinimizeOnTray( bool Val )
{
    switchAutoMinimizeOnTray->IsChecked = Val;
}
//---------------------------------------------------------------------------

