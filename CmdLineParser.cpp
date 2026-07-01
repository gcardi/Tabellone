//---------------------------------------------------------------------------

#pragma hdrstop

#include <System.SysUtils.hpp>

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <memory>

#include "CmdLineParser.h"

#include "RegexDefs.h"

using std::endl;
using std::max;
using std::setw;
using std::make_unique;
using std::lower_bound;
using std::get;

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace CmdLineParser {
//---------------------------------------------------------------------------

struct CmpName {
    template<typename T1, typename T2>
    bool operator()( T1 const & Lhs, T2 const & Rhs ) const {
        return GetName( Lhs ) < Rhs;
    }
};

void ParseCmdLine( OptionType* const Options, size_t OptionCount )
{
    regex_type re( _D( "^\\s*[-\?]+([^=\\s-]+)(\?:=([^\\s]+))\?\\s*$" ) );
    for ( int Idx = 1 ; Idx <= ParamCount() ; ++Idx ) {
        String const Param = ParamStr( Idx ).c_str();
        cmatch_type m;
        if ( regex_match( Param.c_str(), m, re ) ) {
            OptionType* const it =
                lower_bound(
                    Options,
                    Options + OptionCount,
                    String( m[1].str().c_str(), m[1].str().length() ),
                    CmpName()
                );

            if ( it != Options + OptionCount ) {
                if ( GetName( *it ) == m[1].str().c_str() ) {
                    if ( WasFound( *it ) ) {
                        throw Exception(
                            _D( "Duplicated option \'%s\'" ),
                            ARRAYOFCONST(( ParamStr( Idx ) ))
                        );
                    }
                    get<ToIdx( OptPar::Found )>( *it ) = true;
                    if ( IsValueRequired( *it ) ) {
                        if ( !m[2].matched ) {
                            throw Exception(
                                _D( "Option \'%s\' requires a value" ),
                                ARRAYOFCONST((
                                    GetName( *it )
                                ))
                            );
                        }
                        get<ToIdx( OptPar::Value )>( *it ) = m[2].str().c_str();
                    }
                    continue;
                }
            }
            throw Exception(
                _D( "Invalid option \'%s\'" ),
                ARRAYOFCONST(( ParamStr( Idx ) ))
            );
        }
        else {
            throw Exception(
                _D( "Invalid parameter \'%s\'" ),
                ARRAYOFCONST(( ParamStr( Idx ) ))
            );
        }
    }
}
//---------------------------------------------------------------------------

void Validate( OptionType const * const Options, size_t OptionCount )
{
    for ( OptionType const * it = Options ; it != Options + OptionCount ; ++it ) {
        if ( IsMandatory( *it ) && IsTextEmpty( GetValue( *it ) ) ) {
            throw Exception(
                _D( "Option \'%s\' is mandatory" ),
                ARRAYOFCONST(( GetName( *it ) ))
            );
        }
    }
}
//---------------------------------------------------------------------------

String StripFileExt( String Val )
{
    regex_type re( _D( "^(.*\?).\\w+$" ) );
    cmatch_type m;
    if ( regex_match( Val.c_str(), m, re ) ) {
        return m[1].str().c_str();
    }
    return Val;
}
//---------------------------------------------------------------------------

String GetHelpText( OptionType const * const Options, size_t OptionCount )
{
    auto SB = make_unique<TStringBuilder>();

    size_t MaxParWidth = 0;
    SB->AppendLine( _D( "Usage:" ) );
    SB->Append( LowerCase( StripFileExt( ExtractFileName( String( ParamStr( 0 ) ) ) ) ) );
    for ( OptionType const * Opt = Options ; Opt != Options + OptionCount ; ++Opt ) {
        SB->Append( _D( ' ' ) );
        if ( !IsMandatory( *Opt ) ) {
            SB->Append( _D( '[' ) );
        }
        SB->Append( String( _D( "-" ) ) );
        SB->Append( GetName( *Opt ) );
        if ( IsValueRequired( *Opt ) )  {
            SB->AppendFormat( _D( "={%s}" ),
            ARRAYOFCONST(( GetValueDesc( *Opt ) )) );
        }
        if ( !IsMandatory( *Opt ) ) {
            SB->Append( _D( ']' ) );
        }
        MaxParWidth = max( MaxParWidth, GetTextLen( GetName( *Opt ) ) );
    }

    SB->AppendLine();
    SB->AppendLine();
    SB->AppendLine( _D( "Parameters:" ) );
    for ( OptionType const * Opt = Options ; Opt != Options + OptionCount ; ++Opt ) {
        SB->AppendFormat(
            _D( "  -%s" ), ARRAYOFCONST(( GetName( *Opt ) ))
        );

        if ( IsValueRequired( *Opt ) && !IsTextEmpty( GetValueDesc( *Opt ) ) ) {
            SB->AppendFormat(
                _D( " = %s" ), ARRAYOFCONST(( GetValueDesc( *Opt ) ))
            );
        }

        SB->AppendFormat( _D( " ; %s" ), ARRAYOFCONST(( GetDesc( *Opt ) )) );

        if ( !IsTextEmpty( GetValueDesc( *Opt ) ) ) {
            SB->AppendLine();
            SB->AppendFormat(
                _D( "        %s" ), ARRAYOFCONST(( GetValueDesc( *Opt ) ))
            );
            if( !IsTextEmpty( GetValueLongDesc( *Opt ) ) ) {
                SB->AppendFormat(
                    _D( ": %s" ), ARRAYOFCONST(( GetValueLongDesc( *Opt ) ))
                );
            }
        }

        if ( !IsTextEmpty( GetValue( *Opt ) ) ) {
            SB->AppendLine();
            SB->AppendFormat(
                _D( "        default=%s" ),
                ARRAYOFCONST(( GetValue( *Opt ) ))
            );
        }
        SB->AppendLine();
        SB->AppendLine();
    }

    return SB->ToString();
}

//---------------------------------------------------------------------------
}; // End of namespace CmdLineParser
//---------------------------------------------------------------------------
