// STL
#include <sstream>
// CPPUNIT
#include <cppunit/extensions/HelperMacros.h>

class IndexBuildingTestSuite : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE (IndexBuildingTestSuite);
  CPPUNIT_TEST (testIndexBuilding);
  //  CPPUNIT_TEST (errorCase);
  CPPUNIT_TEST_SUITE_END ();
public:

  /** Test the Index Building functionality. */
  void testIndexBuilding ();

  /** Test some error detection functionalities. */
  //  void errorCase ();

  /** Constructor. */
  IndexBuildingTestSuite ();
  
protected:
  std::stringstream _describeKey;
};
CPPUNIT_TEST_SUITE_REGISTRATION (IndexBuildingTestSuite);
