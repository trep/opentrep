#ifndef __EXTRACC_EXTRACPPUNIT_CPPUNITCORE_HPP
#define __EXTRACC_EXTRACPPUNIT_CPPUNITCORE_HPP

#include <string>

// /////////////// MACROS /////////////////
#define CPPUNIT_MAIN_WITH_CLASS(CPPUNITCORE_CLASS) \
  int main (int argc, char* argv[]) { \
    std::string xmlOutputFile(__FILE__); \
    xmlOutputFile.resize(xmlOutputFile.length()-4); \
    xmlOutputFile.append("_results.xml"); \
    return CPPUNITCORE_CLASS(xmlOutputFile.c_str()).main(argc, argv); \
  }

#define CPPUNIT_MAIN_WITH_NAMESPACE(CPPUNITCORE_CLASS_NAMESPACE) CPPUNIT_MAIN_WITH_CLASS(CPPUNITCORE_CLASS_NAMESPACE::CppUnitCore)

#define CPPUNIT_MAIN() CPPUNIT_MAIN_WITH_NAMESPACE(EXTRACPPUNIT)
// /////////// (END OF)  MACROS /////////////

namespace EXTRACPPUNIT {
  
  /** Class to handle CPPUnit functions. */
  class CppUnitCore {
  public:
    /** Constructor .*/
    CppUnitCore (const char* resultFileName);

    /** Destructor .*/
    virtual ~CppUnitCore() {}
      
    /** The main to use for all CPPUnit test.*/
    int main (int argc, char* argv[]);
      
    /** To parse the argument, one has just to overwrite the
	maincore method with args. */
    virtual int maincore (int argc, char* argv[]);

  protected:
    const char* _resultFileName;
	
    /** This method should not be overwritten. */
    int maincore();
  };
    
}
#endif // __EXTRACC_EXTRACPPUNIT_CPPUNITCORE_HPP
