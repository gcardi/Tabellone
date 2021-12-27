//---------------------------------------------------------------------------

#ifndef DataModStyleResH
#define DataModStyleResH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TdmStyleRes : public TDataModule
{
__published:	// IDE-managed Components
    TStyleBook *StyleBook1;
private:	// User declarations
public:		// User declarations
    __fastcall TdmStyleRes(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmStyleRes *dmStyleRes;
//---------------------------------------------------------------------------
#endif
