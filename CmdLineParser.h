//---------------------------------------------------------------------------

#ifndef CmdLineParserH
#define CmdLineParserH

#include <System.hpp>

// si usa boost::tuple e non std::boost tuple per problemi di move semantics
// con String
#include <boost/tuple/tuple.hpp>

#include <string>

#if 0
#if defined( _UNICODE )
//  #define  stdtstr  std::wstring
  #define  tcout    std::wcout
  #define  tcerr    std::wcerr
#else
//  #define  stdtstr  std::string
  #define  tcout    std::cout
  #define  tcerr    std::cerr
#endif
#endif

//---------------------------------------------------------------------------
namespace CmdLineParser {
//---------------------------------------------------------------------------

using OptionType =
    boost::tuple<
        String,
        String,
        bool,
        bool,
        bool,
        String,
        String,
        String
    >;

enum class OptPar {
    Name, Desc, Found, Mandatory, ValueRequired, ValueDesc, ValueLongDesc,
    Value
};

extern void ParseCmdLine( OptionType* const Options, size_t OptionCount );
extern void Validate( OptionType const * const Options, size_t OptionCount );
extern String GetHelpText( OptionType const * const Options, size_t OptionCount );

constexpr size_t ToIdx( OptPar Val ) { return static_cast<size_t>( Val ); }

template<typename T>
size_t GetTextLen( T const & Txt ) {
    return static_cast<size_t>( Txt.Length() );
}

template<typename T>
bool IsTextEmpty( T const & Txt ) { return Txt.IsEmpty(); }

inline bool WasFound( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::Found )>( Opt );
}

inline bool IsMandatory( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::Mandatory )>( Opt );
}

inline bool IsValueRequired( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::ValueRequired )>( Opt );
}

inline String GetName( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::Name )>( Opt );
}

inline String GetDesc( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::Desc )>( Opt );
}

inline String GetValue( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::Value )>( Opt );
}

inline String GetValueDesc( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::ValueDesc )>( Opt );
}

inline String GetValueLongDesc( OptionType const & Opt ) {
    return boost::get<ToIdx( OptPar::ValueLongDesc )>( Opt );
}

//---------------------------------------------------------------------------
}; // End of namespace CmdLineParser
//---------------------------------------------------------------------------
#endif
