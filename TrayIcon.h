//---------------------------------------------------------------------------

#ifndef TrayIconH
#define TrayIconH

#include <Winapi.ShellApi.hpp>
#include <Winapi.Windows.hpp>
#include <FMX.Types.hpp>

//---------------------------------------------------------------------------
namespace Anafestica {
//---------------------------------------------------------------------------

class TFMXTrayIcon : public TFmxObject {
public:
    enum TBalloonFlags {
        bfNone = NIIF_NONE, bfInfo = NIIF_INFO, bfWarning = NIIF_WARNING,
        bfError = NIIF_ERROR
    };
    TFMXTrayIcon( TFMXTrayIcon const & ) = delete;
    TFMXTrayIcon& operator=( TFMXTrayIcon const & ) = delete;
    TFMXTrayIcon( TFMXTrayIcon&& ) = delete;
    TFMXTrayIcon& operator=( TFMXTrayIcon&& ) = delete;
private:
    using inherited = TFmxObject;

    String icon_;
    HWND handle_ {};
    TNotifyIconData data_;
    String hint_;
    bool visible_{ false };
    TNotifyEvent onLClick_{ nullptr };
    TNotifyEvent onRClick_{ nullptr };
    TNotifyEvent onDblClick_{ nullptr };
    String balloonHint_;
    String balloonTitle_;
    TBalloonFlags balloonFlags_{ bfNone };
    int balloonTimeout_{ 5000 };

    void SetTray( unsigned AId ) { ::Shell_NotifyIcon( AId, &data_ ); }
    void __fastcall SetIcon( String Value );
    void __fastcall SetHint( String Value );
    void __fastcall SetVisible( bool Value );
    void UpdateShow();
    void __fastcall SetBalloonHint( String Value );
    void __fastcall SetBalloonTitle( String Value );
    void __fastcall SetBalloonTimeout( int Value ) { data_.uTimeout = Value; }
    int __fastcall GetBalloonTimeout() const { return data_.uTimeout; }
protected:
public:
    static UINT const WM_TRAYICON_MESSAGE = WM_USER + 128;

    __fastcall TFMXTrayIcon( TComponent* AOwner, TWindowHandle* const Handle );
    __fastcall ~TFMXTrayIcon();
    void ShowBalloonHint();
__published:
    __property String Hint = { read = hint_, write = SetHint };
    __property bool Visible = { read = visible_, write = SetVisible };
    __property String Icon = { read = icon_, write = SetIcon };

    __property String BalloonHint = { read = balloonHint_, write = SetBalloonHint };
    __property String BalloonTitle = { read = balloonTitle_, write = SetBalloonTitle };
    __property TBalloonFlags BalloonFlags = { read = balloonFlags_, write = balloonFlags_ };
    __property int BalloonTimeout = { read = GetBalloonTimeout, write = SetBalloonTimeout };

    __property TNotifyEvent OnLClick = { read = onLClick_, write = onLClick_ };
    __property TNotifyEvent OnRClick = { read = onRClick_, write = onRClick_ };
    __property TNotifyEvent OnDblClick = { read = onDblClick_, write = onDblClick_ };
};

//---------------------------------------------------------------------------
} // End of namespace Anafestica
//---------------------------------------------------------------------------
#endif
