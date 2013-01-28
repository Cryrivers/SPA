#ifndef TestQueryPreprocessor_h
#define TestQueryPreprocessor_h

#include <cppunit/extensions/HelperMacros.h>

class QueryPreprocessorTest : public CPPUNIT_NS::TestFixture  
{ 
	CPPUNIT_TEST_SUITE( QueryPreprocessorTest ); 
	CPPUNIT_TEST( f0 );
	//CPPUNIT_TEST( f1 );
	//CPPUNIT_TEST( f2 );
	//CPPUNIT_TEST( f3 );
	CPPUNIT_TEST_SUITE_END();

public:
	//default
	void setUp();
	void tearDown();
	//test cases
	void f0();
	//void f1();
	//void f2();
	//void f3();
};
#endif