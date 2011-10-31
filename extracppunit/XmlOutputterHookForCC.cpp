// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// CPPUNIT
#include <cppunit/tools/XmlDocument.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/TestSuite.h>
// ExtraCppUnit
#include <extracppunit/XmlOutputterHookForCC.hpp>

namespace EXTRACPPUNIT {

      // //////////////////////////////////////////////////////////////////////
      void XmlOutputterHookForCC::
      beginDocument (CppUnit::XmlDocument* document) {
        if (document == NULL) {
          return;
        }
      }
		
      // //////////////////////////////////////////////////////////////////////
      void XmlOutputterHookForCC::
      failTestAdded (CppUnit::XmlDocument* document,
                     CppUnit::XmlElement* testElement,
                     CppUnit::Test* test,
                     CppUnit::TestFailure* failure) {
        testAdded (document, testElement, test);
      }
		
      // //////////////////////////////////////////////////////////////////////
      void XmlOutputterHookForCC::
      successfulTestAdded (CppUnit::XmlDocument* document,
                           CppUnit::XmlElement* testElement,
                           CppUnit::Test* test) {
        testAdded (document, testElement, test);
      }
      
      // //////////////////////////////////////////////////////////////////////
      void XmlOutputterHookForCC::testAdded (CppUnit::XmlDocument* document,
                                             CppUnit::XmlElement* testElement,
                                             CppUnit::Test* test) {
        if (testElement == NULL) {
          return;
        }        
      }
			
}
