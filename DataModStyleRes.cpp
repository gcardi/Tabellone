//---------------------------------------------------------------------------


#pragma hdrstop

#include "DataModStyleRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "FMX.Controls.TControl"
#pragma resource "*.dfm"
TdmStyleRes *dmStyleRes;
//---------------------------------------------------------------------------
__fastcall TdmStyleRes::TdmStyleRes(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
