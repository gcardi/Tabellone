//---------------------------------------------------------------------------

#ifndef RegexDefsH
#define RegexDefsH

#include <boost/regex.hpp>
#include <string>

#if !defined(_UNICODE)
  using regex_type = boost::regex;
  using cmatch_type = boost::cmatch;
  using std_string = std::string;
#else
  using regex_type = boost::wregex;
  using cmatch_type = boost::wcmatch;
  using std_string = std::wstring;
#endif

//---------------------------------------------------------------------------
#endif
