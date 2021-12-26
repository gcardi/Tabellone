//---------------------------------------------------------------------------

#ifndef FormPanelH
#define FormPanelH
//---------------------------------------------------------------------------

#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
#include "FMXFormPanelBase.h"
#include "FrameNum.h"

#include <array>
#include <memory>

//---------------------------------------------------------------------------

using TNumEvent =
    void __fastcall (__closure *)( System::TObject* Sender, int Num, bool Val );

class TfrmPanel : public TfrmPanelBase
{
__published:	// IDE-managed Components
    TStyleBook *StyleBook1;
    TGridLayout *GridLayout1;
    TLayout *Layout1;
    TRectangle *Rectangle1;
    TImage *Image1;
private:	// User declarations
    std::array<bool,90> states_;
    std::array<std::unique_ptr<TfrmeNum>,90> numFrames_;
    TNumEvent onNumberChanged_ { nullptr };
    int currNum_ {};
    static constexpr auto TabsNodeName = _T( "Tabs" );

    void RestoreProperties();
    void SaveProperties() const;

    String GetTabsText() const;
    void SetTabsText( String Val );
    void NotifyNumberChanged( int Num, bool Val );
public:		// User declarations
    using inherited = TfrmPanelBase;

    __fastcall TfrmPanel( TComponent* Owner,
                          FMXWinDisplayDev const * Display,
                          StoreOpts StoreOptions,
                          Anafestica::TConfigNode* const RootNode = nullptr,
                          TNumEvent OnNumberChanged = nullptr );
    __fastcall ~TfrmPanel();
    bool GetTab( size_t Idx ) const;
    bool SetTab( size_t Idx, bool Val );
    bool ToggleTab( size_t Idx );
    void Clear();
    __property String TabsText = { read = GetTabsText, write = SetTabsText };
    __property int CurrNum = { read = currNum_ };

};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmPanel *frmPanel;
//---------------------------------------------------------------------------
#endif
