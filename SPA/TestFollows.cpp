#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Follows.h"
#include "TestFollows.h"
#include <iostream>
#include <string>

void 
	FollowsTest::setUp()
{
}


void 
	FollowsTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( FollowsTest ); // Note 4 

void FollowsTest::f0() {
	Follows f;

	//case 1a, should print false
	vector<int> va; vector<int> vb;
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);

	f.addFollows(1,2);
	f.addFollows(2,3);
	f.addFollows(3,4);
	f.addFollows(4,5);
	f.addFollows(5,6);
	f.addFollows(6,7);
	f.addFollows(7,15);
	f.addFollows(8,9);
	f.addFollows(9,14);
	f.addFollows(10,11);
	f.addFollows(12,13);
	/* Follow table is
	* 1 2 3 4 5 6 7  8 9  10 12
	* 2 3 4 5 6 7 15 9 14 11 13
	*
	*/
	//case 1a, should print true
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == true);

	//case 1b, should print false
	vb.push_back(8);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);
	//case 1b, should print false
	vb.push_back(10);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);
	//case 1b, should print true
	vb.push_back(5);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == true);

	vb.clear();

	//case 1c, should print false
	va.push_back(11);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);
	//case 1c, should print false
	va.push_back(13);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);
	//case 1c, should print true
	va.push_back(4);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == true);

	//case 1d, should print false
	vb.push_back(6);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == false);
	//case 1d, should print true
	vb.push_back(5);
	CPPUNIT_ASSERT(f.follows(&va,&vb,0) == true);

	//should print 0 1 0 0 1 0 0 1 0 1
}

void FollowsTest::f1() {
	Follows f;
	f.addFollows(1,2);
	f.addFollows(2,3);
	f.addFollows(3,4);
	f.addFollows(4,5);
	f.addFollows(5,6);
	f.addFollows(6,7);
	f.addFollows(7,15);
	f.addFollows(8,9);
	f.addFollows(9,14);
	f.addFollows(10,11);
	f.addFollows(12,13);
	/* Follow table is
	* 1 2 3 4 5 6 7  8 9  10 12
	* 2 3 4 5 6 7 15 9 14 11 13
	*
	*/
	vector<int> va; vector<int> vb;

	//case 2a, should print true\n 2 3 4 5 6 7 15 9 14 11 13
	int myints[] = {2, 3, 4, 5, 6, 7, 15, 9, 14, 11, 13};

	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(11);

	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2b, should print true\n 6
	va.clear();
	va.push_back(5);

	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(1,6));

	va.clear();
	vb.clear();

	//case 2c, should print true\n 2 4 15 13
	vb.push_back(2);
	vb.push_back(1);
	vb.push_back(4);
	vb.push_back(15);
	vb.push_back(12);
	vb.push_back(13);
	vb.push_back(16);

	int myint2[] = {2, 4, 15, 13};
	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint2, myint2 + sizeof(myint2) / sizeof(int)));


	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(11);
	vb.push_back(1);vb.push_back(2);vb.push_back(3);

	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2d, should print true\n 15
	va.clear();
	va.push_back(7);
	vb.push_back(5);vb.push_back(8);vb.push_back(15);

	CPPUNIT_ASSERT(f.follows(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(1,15));
}

void FollowsTest::f2() {
	Follows f;
	f.addFollows(1,2);
	f.addFollows(2,3);
	f.addFollows(3,4);
	f.addFollows(4,5);
	f.addFollows(5,6);
	f.addFollows(6,7);
	f.addFollows(7,15);
	f.addFollows(8,9);
	f.addFollows(9,14);
	f.addFollows(10,11);
	f.addFollows(12,13);
	/* Follow table is
	* 1 2 3 4 5 6 7  8 9  10 12
	* 2 3 4 5 6 7 15 9 14 11 13
	*
	*/
	vector<int> va; vector<int> vb;

	//case 3a, should print true\n1 2 3 4 5 6 7 8 9 10 12
	int myints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12};

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(12);

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3b, should print true\n 10
	vb.clear();
	vb.push_back(11);

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(1,10));

	va.clear();
	vb.clear();

	//case 3c, should print true\n 2 1 4 12
	va.push_back(2);
	va.push_back(1);
	va.push_back(4);
	va.push_back(15);
	va.push_back(12);
	va.push_back(13);
	va.push_back(16);

	int myint2[] = {2, 1, 4, 12};

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint2, myint2 + sizeof(myint2) / sizeof(int)));

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	vb.push_back(12);
	va.push_back(1);va.push_back(2);va.push_back(3);

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3d, should print true\n 6
	vb.clear();
	vb.push_back(7);
	va.push_back(6);va.push_back(8);va.push_back(15);

	CPPUNIT_ASSERT(f.follows(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(1,6));
}

void FollowsTest::f3() {
	Follows f;
	f.addFollows(1,2);
	f.addFollows(2,3);
	f.addFollows(3,4);
	f.addFollows(4,5);
	f.addFollows(5,6);
	f.addFollows(6,7);
	f.addFollows(7,15);
	f.addFollows(8,9);
	f.addFollows(9,14);
	f.addFollows(10,11);
	f.addFollows(12,13);
	/* Follow table is
	* 1 2 3 4 5 6 7  8 9  10 12
	* 2 3 4 5 6 7 15 9 14 11 13
	*
	*/
	vector<int> va; vector<int> vb;

	//case 4a, should print true
	//1 2 3 4 5 6 7 8 9 10 12
	//2 3 4 5 6 7 15 9 14 11 13
	int myintA[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12};
	int myintB[] = {2, 3, 4, 5, 6, 7, 15, 9, 14, 11, 13};

	CPPUNIT_ASSERT(f.follows(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA, myintA + sizeof(myintA) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB, myintB + sizeof(myintB) / sizeof(int)));

	//case 4b, should print true
	//7 8
	//15 9
	va.clear();
	vb.clear();
	vb.push_back(12); vb.push_back(15);vb.push_back(9);vb.push_back(16);

	int myintA2[] = {7, 8};
	int myintB2[] = {15, 9};

	CPPUNIT_ASSERT(f.follows(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA2, myintA2 + sizeof(myintA2) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB2, myintB2 + sizeof(myintB2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 4c, should print true
	//2 12
	//3 13
	va.push_back(2); va.push_back(11);va.push_back(13);va.push_back(12);

	int myintA3[] = {2, 12};
	int myintB3[] = {3, 13};

	CPPUNIT_ASSERT(f.follows(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA3, myintA3 + sizeof(myintA3) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB3, myintB3 + sizeof(myintB3) / sizeof(int)));

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(12);vb.push_back(12);
	va.push_back(1);va.push_back(3);

	CPPUNIT_ASSERT(f.follows(&va,&vb,3) == false);
	CPPUNIT_ASSERT(va == vector<int>());
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 3d, should print true
	//6 7
	//7 15
	vb.push_back(7);vb.push_back(7);vb.push_back(7);vb.push_back(15);
	va.push_back(6);va.push_back(8);va.push_back(15);va.push_back(7);

	int myintA4[] = {6, 7};
	int myintB4[] = {7, 15};

	CPPUNIT_ASSERT(f.follows(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA4, myintA4 + sizeof(myintA4) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB4, myintB4 + sizeof(myintB4) / sizeof(int)));
}
