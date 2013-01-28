#ifndef TestParent_h
#define TestParent_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParentTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( ParentTest ); // Note 3 
	CPPUNIT_TEST( f0 );
	CPPUNIT_TEST( f1 );
	CPPUNIT_TEST( f2 );
	CPPUNIT_TEST( f3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void f0();
	void f1();
	void f2();
	void f3();

};
#endif
