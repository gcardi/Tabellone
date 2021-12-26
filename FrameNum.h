//---------------------------------------------------------------------------

#ifndef FrameNumH
#define FrameNumH

//---------------------------------------------------------------------------

#include <FMX.Ani.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>

//---------------------------------------------------------------------------

class TfrmeNum : public TFrame
{
__published:	// IDE-managed Components
    TRectangle *Rectangle1;
    TText *Text1;
    TGradientAnimation *GradientAnimation1;
    TLayout *Layout1;
    TLayout *Layout2;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmeNum(TComponent* Owner);
};

//---------------------------------------------------------------------------
//extern PACKAGE TfrmeNum *frmeNum;
//---------------------------------------------------------------------------
#endif
