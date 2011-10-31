#ifndef __EXTRACC_EXTRACPPUNIT_XMLOUTPUTTERHOOKFORCC_HPP
#define __EXTRACC_EXTRACPPUNIT_XMLOUTPUTTERHOOKFORCC_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <cppunit/XmlOutputterHook.h>
 
namespace EXTRACPPUNIT {
  
  /** Helper class to translate CPPUnit test output into something ANT can
      understand (for CruiseControl). */
  class XmlOutputterHookForCC : public CppUnit::XmlOutputterHook {
  public:
    /** Call-back for the beginning of a document. */
    virtual void beginDocument (CppUnit::XmlDocument*);
    
    /** Call-back for a failed test. */
    virtual void failTestAdded (CppUnit::XmlDocument*, CppUnit::XmlElement*,
				CppUnit::Test*, CppUnit::TestFailure*);
      
    /** Call-back for a successful test. */
    virtual void successfulTestAdded (CppUnit::XmlDocument*,
				      CppUnit::XmlElement*, CppUnit::Test*);
    
    /** Call-back for a new test. */
    virtual void testAdded (CppUnit::XmlDocument*, CppUnit::XmlElement*, 
			    CppUnit::Test*);

  public:
    /** Destructor .*/
    virtual ~XmlOutputterHookForCC() {}
  };
		
}
#endif // __EXTRACC_EXTRACPPUNIT_XMLOUTPUTTERHOOKFORCC_HPP
