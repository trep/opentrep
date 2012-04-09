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

/**
 * Class holding a set of strings, e.g., {"rio", "de", "janeiro"}
 */
struct StringSet {
  // //////////////// Type definitions //////////////////
  typedef std::list<std::string> StringSet_T;

  // /////////////////////////////////////////////////////////////////////////
  void display() const {
    //
    std::cout << "  {";

    short idx_string = 0;
    for (StringSet_T::const_iterator itString = _set.begin();
         itString != _set.end(); ++itString, ++idx_string) {
      //
      if (idx_string != 0) {
        std::cout << ", ";
      }

      //
      const std::string& lString = *itString;

      //
      std::cout << "\"" << lString << "\"";
    }

    //
    std::cout << "}";
  }

  // /////////////////////////////////////////////////////////////////////////
  void push_back (const std::string& iString) {
    _set.push_back (iString);
  }
  
  // /////////////////////////////////////////////////////////////////////////
  void clear() {
    _set.clear();
  }
  
  // //////////////// Attributes ///////////////
  /**
   * String set, i.e., a list of strings
   */
  StringSet_T _set;
};

/**
 * Class holding a string partition.
 *
 * A string partition contains all the ways to combine, serially,
 * all the words of a given string.
 *
 * For instance, "rio de janeiro" will give:
 * <ul>
 *   <li>{</li>
 *   <li><ul>
 *     <li>{"rio", "de", "janeiro"}</li>
 *     <li>{"rio", "de janeiro"}</li>
 *     <li>{"rio de", "janeiro"}</li>
 *     <li>{"rio de janeiro"}</li>
 *   </ul></li>
 *   <li>}</li>
 * </ul>
 */
struct StringPartition {
  // //////////////// Type definitions //////////////////
  typedef std::list<StringSet> StringPartition_T;
  
  // /////////////////////////////////////////////////////////////////////////
  void display() const {
    //
    std::cout << "{" << std::endl;

    short idx_sublist = 0;
    for (StringPartition_T::const_iterator itSet = _partition.begin();
         itSet != _partition.end(); ++itSet, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        std::cout << "," << std::endl;
      }
      
      //
      const StringSet& lStringSet = *itSet;

      //
      lStringSet.display();
    }

    //
    std::cout << std::endl << "}" << std::endl;
  }

  // /////////////////////////////////////////////////////////////////////////
  void push_back (const StringSet& iStringSet) {
    _partition.push_back (iStringSet);
  }
  
  // /////////////////////////////////////////////////////////////////////////
  void clear() {
    _partition.clear();
  }
  
  // /////////////////////////////////////////////////////////////////////////
  StringPartition_T& init (const std::string& iString) {
    //
    StringSet oStringSet;
    StringPartition_T oStringPartition;

    //
    oStringSet.push_back (iString);
    _partition.push_back (oStringSet);
  
    //
    return _partition;
  }

  // //////////////// Attributes ///////////////
  /**
   * Partition, i.e., a list containing sub-lists of strings
   */
  StringPartition_T _partition;
};

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

  //
  StringPartition lStringPartition;
  lStringPartition.init (lRio2Str);
  lStringPartition.display();
  
  return 0;
}
