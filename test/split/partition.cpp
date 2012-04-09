// /////////////////////////////////////////////////////////////////////////
//
// String partition algorithm
//
// Author: Denis Arnaud
// Date:   April 2012
//
// /////////////////////////////////////////////////////////////////////////
// STL
#include <iostream>
#include <string>
#include <list>

// //////////////// Type definitions //////////////////
typedef std::list<std::string> StringSet_T;
typedef std::list<StringSet_T> StringPartition_T;


// /////////// M A I N ////////////////
int main (int argc, char* argv[]) {

  const std::string lLax1Str = "los angeles";
  const std::string lLax2Str = "lso angeles";
  const std::string lRio1Str = "rio de janeiro";
  const std::string lRio2Str = "rio de janero";
  const std::string lRek1Str = "reikjavik";
  const std::string lRek2Str = "rekyavik";
  const std::string lSfoRio1Str = "san francisco rio de janeiro";
  const std::string lSfoRio2Str = "san francicso rio de janero";

  return 0;
}
