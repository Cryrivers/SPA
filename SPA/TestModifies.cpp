/*
* TestModifies.cpp
*
*  Created on: 30 Oct 2012
*      Author: ray
*/

#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Modifies.h"
#include "TestModifies.h"
#include <iostream>
#include <string>

void 
	ModifiesTest::setUp()
{
}


void 
	ModifiesTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ModifiesTest ); // Note 4 

void ModifiesTest::f0() {
	Modifies f;

	//case 1a, should print false
	vector<int> va; vector<int> vb;
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);

	f.addModifies(1,0);
	f.addModifies(2,1);
	f.addModifies(3,2);
	f.addModifies(4,3);
	f.addModifies(5,3);
	f.addModifies(6,4);
	f.addModifies(7,4);
	f.addModifies(7,2);
	f.addModifies(7,5);
	f.addModifies(7,1);
	f.addModifies(8,4);
	f.addModifies(9,2);
	f.addModifies(9,5);
	f.addModifies(9,1);
	f.addModifies(10,2);
	f.addModifies(11,5);
	f.addModifies(12,1);
	f.addModifies(13,5);
	f.addModifies(14,1);
	/* Modifies table is
	* 1 2 3 4 5 6 7 7 7 7 8 9 9 9 10 11 12 13 14
	* 0 1 2 3 3 4 4 2 5 1 4 2 5 1 2  5  1  5  1
	*
	*/
	//case 1a, should print true
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == true);

	//case 1b, should print false
	vb.push_back(6);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);
	//case 1b, should print false
	vb.push_back(9);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);
	//case 1b, should print true
	vb.push_back(3);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == true);

	vb.clear();

	//case 1c, should print false
	va.push_back(15);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);
	//case 1c, should print false
	va.push_back(18);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);
	//case 1c, should print true
	va.push_back(7);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == true);

	//case 1d, should print false
	vb.push_back(10);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == false);
	//case 1d, should print true
	vb.push_back(4);
	CPPUNIT_ASSERT(f.modifies(&va,&vb,0) == true);

}

void ModifiesTest::f1() {
	Modifies f;
	f.addModifies(1,0);
	f.addModifies(2,1);
	f.addModifies(3,2);
	f.addModifies(4,3);
	f.addModifies(5,3);
	f.addModifies(6,4);
	f.addModifies(7,4);
	f.addModifies(7,2);
	f.addModifies(7,5);
	f.addModifies(7,1);
	f.addModifies(8,4);
	f.addModifies(9,2);
	f.addModifies(9,5);
	f.addModifies(9,1);
	f.addModifies(10,2);
	f.addModifies(11,5);
	f.addModifies(12,1);
	f.addModifies(13,5);
	f.addModifies(14,1);
	/* Modifies table is
	* 1 2 3 4 5 6 7 7 7 7 8 9 9 9 10 11 12 13 14
	* 0 1 2 3 3 4 4 2 5 1 4 2 5 1 2  5  1  5  1
	*
	*/
	vector<int> va; vector<int> vb;

	//case 2a, should print true\n 0 1 2 3 4 5
	int myints[] = {0, 1, 2, 3, 4, 5};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(15);

	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2b, should print true\n 2 5 1 4
	va.push_back(9);
	va.push_back(7);

	int myints2[] = {2, 5, 1, 4};
	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints2, myints2 + sizeof(myints2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 2c, should print true\n 2 3 4 5
	vb.push_back(2);
	vb.push_back(3);
	vb.push_back(4);
	vb.push_back(5);
	vb.push_back(12);
	vb.push_back(13);
	vb.push_back(16);

	int myint3[] = {2, 3, 4, 5};
	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint3, myint3 + sizeof(myint3) / sizeof(int)));


	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(9);
	vb.push_back(8);vb.push_back(3);vb.push_back(15);vb.push_back(14);

	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2d, should print true\n 1 5 4
	va.clear();
	va.push_back(7);va.push_back(9);va.push_back(12);va.push_back(14);
	vb.push_back(8);vb.push_back(1);vb.push_back(15);vb.push_back(14);
	vb.push_back(5);vb.push_back(9);vb.push_back(12);vb.push_back(3);vb.push_back(4);
	int myint4[] = {1, 5, 4};
	CPPUNIT_ASSERT(f.modifies(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint4, myint4 + sizeof(myint4) / sizeof(int)));
}

void ModifiesTest::f2() {
	Modifies f;
	f.addModifies(1,0);
	f.addModifies(2,1);
	f.addModifies(3,2);
	f.addModifies(4,3);
	f.addModifies(5,3);
	f.addModifies(6,4);
	f.addModifies(7,4);
	f.addModifies(7,2);
	f.addModifies(7,5);
	f.addModifies(7,1);
	f.addModifies(8,4);
	f.addModifies(9,2);
	f.addModifies(9,5);
	f.addModifies(9,1);
	f.addModifies(10,2);
	f.addModifies(11,5);
	f.addModifies(12,1);
	f.addModifies(13,5);
	f.addModifies(14,1);
	/* Modifies table is
	* 1 2 3 4 5 6 7 7 7 7 8 9 9 9 10 11 12 13 14
	* 0 1 2 3 3 4 4 2 5 1 4 2 5 1 2  5  1  5  1
	*
	*/
	vector<int> va; vector<int> vb;

	//case 3a, should print true\n1 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	int myints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(7);

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3b, should print true\n 2, 7, 9, 12, 14, 3, 10
	vb.push_back(1);vb.push_back(2);
	int myints2[] = {2, 7, 9, 12, 14, 3, 10};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints2, myints2 + sizeof(myints2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 3c, should print true\n 7, 1, 4, 12, 13
	va.push_back(7);
	va.push_back(1);
	va.push_back(4);
	va.push_back(15);
	va.push_back(12);
	va.push_back(13);
	va.push_back(16);

	int myint3[] = {7, 1, 4, 12, 13};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint3, myint3 + sizeof(myint3) / sizeof(int)));

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	vb.push_back(3);vb.push_back(4);vb.push_back(5);
	va.push_back(1);va.push_back(2);va.push_back(3);

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3d, should print true\n 9, 7, 13, 8
	vb.clear();
	va.push_back(9);va.push_back(7);va.push_back(14);va.push_back(13);va.push_back(5);va.push_back(8);
	vb.push_back(15);vb.push_back(2);vb.push_back(4);vb.push_back(5);
	int myint4[] = {9, 7, 13, 8};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint4, myint4 + sizeof(myint4) / sizeof(int)));
}

void ModifiesTest::f3() {
	Modifies f;
	f.addModifies(1,0);
	f.addModifies(2,1);
	f.addModifies(3,2);
	f.addModifies(4,3);
	f.addModifies(5,3);
	f.addModifies(6,4);
	f.addModifies(7,4);
	f.addModifies(7,2);
	f.addModifies(7,5);
	f.addModifies(7,1);
	f.addModifies(8,4);
	f.addModifies(9,2);
	f.addModifies(9,5);
	f.addModifies(9,1);
	f.addModifies(10,2);
	f.addModifies(11,5);
	f.addModifies(12,1);
	f.addModifies(13,5);
	f.addModifies(14,1);
	/* Modifies table is
	* 1 2 3 4 5 6 7 7 7 7 8 9 9 9 10 11 12 13 14
	* 0 1 2 3 3 4 4 2 5 1 4 2 5 1 2  5  1  5  1
	*
	*/
	vector<int> va; vector<int> vb;

	//case 4a, should print true
	// 1 2 3 4 5 6 7 7 7 7 8 9 9 9 10 11 12 13 14
	// 0 1 2 3 3 4 4 2 5 1 4 2 5 1 2  5  1  5  1
	int myintA[] = {1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 8, 9, 9, 9, 10, 11, 12, 13, 14};
	int myintB[] = {0, 1, 2, 3, 3, 4, 4, 2, 5, 1, 4, 2, 5, 1, 2,  5,  1,  5,  1};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA, myintA + sizeof(myintA) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB, myintB + sizeof(myintB) / sizeof(int)));

	//case 4b, should print true
	//1, 2, 7, 9, 12, 14, 6, 7, 8
	//0, 1, 1, 1, 1,  1,  4, 4, 4
	va.clear();
	vb.clear();
	vb.push_back(0); vb.push_back(1);vb.push_back(7);vb.push_back(4);vb.push_back(6);

	int myintA2[] = {1, 2, 7, 9, 12, 14, 6, 7, 8};
	int myintB2[] = {0, 1, 1, 1, 1,  1,  4, 4, 4};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA2, myintA2 + sizeof(myintA2) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB2, myintB2 + sizeof(myintB2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 4c, should print true
	//7 7 7 7 14 9 9 9 8
	//4 2 5 1 1  2 5 1 4
	va.push_back(7); va.push_back(15);va.push_back(14);va.push_back(16);va.push_back(9);va.push_back(8);
	int myintA3[] = {7, 7, 7, 7, 14, 9, 9, 9, 8};
	int myintB3[] = {4, 2, 5, 1, 1,  2, 5, 1, 4};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA3, myintA3 + sizeof(myintA3) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB3, myintB3 + sizeof(myintB3) / sizeof(int)));

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(8);vb.push_back(12);vb.push_back(16);
	va.push_back(9);va.push_back(7);va.push_back(6);

	CPPUNIT_ASSERT(f.modifies(&va,&vb,3) == false);
	CPPUNIT_ASSERT(va == vector<int>());
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 4d, should print true
	//9 7 7
	//2 5 1
	vb.push_back(2);vb.push_back(7);vb.push_back(7);vb.push_back(5);vb.push_back(1);
	va.push_back(9);va.push_back(8);va.push_back(15);va.push_back(7);va.push_back(7);

	int myintA4[] = {9, 7, 7};
	int myintB4[] = {2, 5, 1};

	CPPUNIT_ASSERT(f.modifies(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA4, myintA4 + sizeof(myintA4) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB4, myintB4 + sizeof(myintB4) / sizeof(int)));
}
