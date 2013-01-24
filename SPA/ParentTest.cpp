/*
 * ParentTest.cpp
 *
 *  Created on: 30 Oct 2012
 *      Author: ray
 */


#include <iostream>
#include "Parent.h"

void printVect(vector<int> );

void p1()
{
	Parent f;

	//case 1a, should print false
	vector<int> va;
	vector<int> vb;
	cout << f.parent(&va, &vb, 0) << endl;

	f.addParent(7, 8);
	f.addParent(7, 9);
	f.addParent(7, 15);
	f.addParent(9, 10);
	f.addParent(9, 11);
	f.addParent(9, 12);
	f.addParent(9, 13);
	f.addParent(13, 14);

	/* Parent table is
	 * 7 7 7  9  9  9  9  13
	 * 8 9 15 10 11 12 13 14
	 *
	 */
	//case 1a, should print true
	cout << f.parent(&va, &vb, 0) << endl;

	//case 1b, should print false
	vb.push_back(3);
	cout << f.parent(&va, &vb, 0) << endl;
	//case 1b, should print false
	vb.push_back(5);
	cout << f.parent(&va, &vb, 0) << endl;
	//case 1b, should print true
	vb.push_back(8);
	cout << f.parent(&va, &vb, 0) << endl;

	vb.clear();

	//case 1c, should print false
	va.push_back(11);
	cout << f.parent(&va, &vb, 0) << endl;
	//case 1c, should print false
	va.push_back(18);
	cout << f.parent(&va, &vb, 0) << endl;
	//case 1c, should print true
	va.push_back(7);
	cout << f.parent(&va, &vb, 0) << endl;

	//case 1d, should print false
	vb.push_back(10);
	cout << f.parent(&va, &vb, 0) << endl;
	//case 1d, should print true
	vb.push_back(8);
	cout << f.parent(&va, &vb, 0) << endl;

	//should print 0 1 0 0 1 0 0 1 0 1
}


void p2()
{
	Parent f;

	f.addParent(7, 8);
	f.addParent(7, 9);
	f.addParent(7, 15);
	f.addParent(9, 10);
	f.addParent(9, 11);
	f.addParent(9, 12);
	f.addParent(9, 13);
	f.addParent(13, 14);

	/* Parent table is
	 * 7 7 7  9  9  9  9  13
	 * 8 9 15 10 11 12 13 14
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 2a, should print true\n 8 9 15 10 11 12 13 14
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(11);
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);
	//case 2b, should print true\n 10 11 12 13
	va.clear();
	va.push_back(9);
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);

	va.clear();
	vb.clear();

	//case 2c, should print true\n 8 9 15 12 13
	vb.push_back(2);
	vb.push_back(8);
	vb.push_back(9);
	vb.push_back(15);
	vb.push_back(12);
	vb.push_back(13);
	vb.push_back(16);
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);

	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(9);
	vb.push_back(8);
	vb.push_back(1);
	vb.push_back(15);
	vb.push_back(14);
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);
	//case 2d, should print true\n 8 15 9
	va.clear();
	va.push_back(7);
	vb.push_back(8);
	vb.push_back(1);
	vb.push_back(15);
	vb.push_back(14);
	vb.push_back(5);
	vb.push_back(9);
	vb.push_back(12);
	cout << f.parent(&va, &vb, 1) << endl;
	printVect(vb);
}


void p3()
{
	Parent f;

	f.addParent(7, 8);
	f.addParent(7, 9);
	f.addParent(7, 15);
	f.addParent(9, 10);
	f.addParent(9, 11);
	f.addParent(9, 12);
	f.addParent(9, 13);
	f.addParent(13, 14);

	/* Parent table is
	 * 7 7 7  9  9  9  9  13
	 * 8 9 15 10 11 12 13 14
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 3a, should print true\n 7 9 13
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(7);
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);
	//case 3b, should print true\n 9
	vb.clear();
	vb.push_back(11);
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);

	va.clear();
	vb.clear();

	//case 3c, should print true\n 7 13
	va.push_back(7);
	va.push_back(1);
	va.push_back(4);
	va.push_back(15);
	va.push_back(12);
	va.push_back(13);
	va.push_back(16);
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	vb.push_back(6);
	va.push_back(1);
	va.push_back(2);
	va.push_back(3);
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);
	//case 3d, should print true\n 7
	vb.clear();
	vb.push_back(15);
	va.push_back(9);
	va.push_back(7);
	va.push_back(13);
	cout << f.parent(&va, &vb, 2) << endl;
	printVect(va);
}


void p4()
{
	Parent f;

	f.addParent(7, 8);
	f.addParent(7, 9);
	f.addParent(7, 15);
	f.addParent(9, 10);
	f.addParent(9, 11);
	f.addParent(9, 12);
	f.addParent(9, 13);
	f.addParent(13, 14);

	/* Parent table is
	 * 7 7 7  9  9  9  9  13
	 * 8 9 15 10 11 12 13 14
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 4a, should print true
	// 7 7 7  9  9  9  9  13
	// 8 9 15 10 11 12 13 14
	cout << f.parent(&va, &vb, 3) << endl;
	printVect(va);
	printVect(vb);

	//case 4b, should print true
	//9 7 7
	//12 15 9
	va.clear();
	vb.clear();
	vb.push_back(12);
	vb.push_back(15);
	vb.push_back(9);
	vb.push_back(16);
	cout << f.parent(&va, &vb, 3) << endl;
	printVect(va);
	printVect(vb);

	va.clear();
	vb.clear();

	//case 4c, should print true
	//7 7 7 13 9 9 9 9
	//8 9 15 14 10 11 12 13
	va.push_back(7);
	va.push_back(6);
	va.push_back(13);
	va.push_back(9);
	va.push_back(19);
	cout << f.parent(&va, &vb, 3) << endl;
	printVect(va);
	printVect(vb);

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(8);
	vb.push_back(12);
	vb.push_back(16);
	va.push_back(9);
	va.push_back(7);
	va.push_back(6);
	cout << f.parent(&va, &vb, 3) << endl;
	printVect(va);
	printVect(vb);
	//case 3d, should print true
	//9 7
	//12 9
	vb.push_back(12);
	vb.push_back(7);
	vb.push_back(7);
	vb.push_back(9);
	va.push_back(9);
	va.push_back(8);
	va.push_back(15);
	va.push_back(7);
	cout << f.parent(&va, &vb, 3) << endl;
	printVect(va);
	printVect(vb);
}


int parent_main()
{
	//p1();
	//p2();
	p3();
	//p4();
	getchar();
	return(0);
}


void printVect(vector<int> v)
{
	int size = v.size();

	if (size == 0) {
		cout << "no element" << endl;
	}else {
		for (int i = 0; i < v.size(); i++) {
			cout << v.at(i) << " ";
		}
		cout << endl;
	}
}
