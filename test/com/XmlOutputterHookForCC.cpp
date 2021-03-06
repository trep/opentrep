// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// CPPUNIT
#include <cppunit/tools/XmlDocument.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/TestSuite.h>
// OPENTREP Test Common
#include <test/com/XmlOutputterHookForCC.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void XmlOutputterHookForCC::beginDocument(CppUnit::XmlDocument* document) {
    if (document == NULL) {
      return;
    }

    // Add the name of the test suite
    // CppUnit::XmlElement* suiteNameEl =
    //   new CppUnit::XmlElement ("SuiteName", _suiteName);

    //document->rootElement().addElement(suiteNameEl);
  };
		
  // //////////////////////////////////////////////////////////////////////
  void XmlOutputterHookForCC::failTestAdded (CppUnit::XmlDocument* document,
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

    // Add the name of the test suite
    // CppUnit::XmlElement* suiteNameEl =
    //   new CppUnit::XmlElement("SuiteName", test->getName());

    // testElement->addElement (suiteNameEl);
  }
			
}
