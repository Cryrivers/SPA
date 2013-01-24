/*
 * FollowsTest.cpp
 *
 *  Created on: 30 Oct 2012
 *      Author: ray
 */

#include <iostream>
#include "Follows.h"

void printVector(vector<int> );

void f1()
{
	Follows f;

	//case 1a, should print false
	vector<int> va;
	vector<int> vb;
	cout << f.follows(&va, &vb, 0) << endl;

	f.addFollows(1, 2);
	f.addFollows(2, 3);
	f.addFollows(3, 4);
	f.addFollows(4, 5);
	f.addFollows(5, 6);
	f.addFollows(6, 7);
	f.addFollows(7, 15);
	f.addFollows(8, 9);
	f.addFollows(9, 14);
	f.addFollows(10, 11);
	f.addFollows(12, 13);

	/* Follow table is
	 * 1 2 3 4 5 6 7  8 9  10 12
	 * 2 3 4 5 6 7 15 9 14 11 13
	 *
	 */
	//case 1a, should print true
	cout << f.follows(&va, &vb, 0) << endl;

	//case 1b, should print false
	vb.push_back(8);
	cout << f.follows(&va, &vb, 0) << endl;
	//case 1b, should print false
	vb.push_back(10);
	cout << f.follows(&va, &vb, 0) << endl;
	//case 1b, should print true
	vb.push_back(5);
	cout << f.follows(&va, &vb, 0) << endl;

	vb.clear();

	//case 1c, should print false
	va.push_back(11);
	cout << f.follows(&va, &vb, 0) << endl;
	//case 1c, should print false
	va.push_back(13);
	cout << f.follows(&va, &vb, 0) << endl;
	//case 1c, should print true
	va.push_back(4);
	cout << f.follows(&va, &vb, 0) << endl;

	//case 1d, should print false
	vb.push_back(6);
	cout << f.follows(&va, &vb, 0) << endl;
	//case 1d, should print true
	vb.push_back(5);
	cout << f.follows(&va, &vb, 0) << endl;

	//should print 0 1 0 0 1 0 0 1 0 1
}


void f2()
{
	Follows f;

	f.addFollows(1, 2);
	f.addFollows(2, 3);
	f.addFollows(3, 4);
	f.addFollows(4, 5);
	f.addFollows(5, 6);
	f.addFollows(6, 7);
	f.addFollows(7, 15);
	f.addFollows(8, 9);
	f.addFollows(9, 14);
	f.addFollows(10, 11);
	f.addFollows(12, 13);

	/* Follow table is
	 * 1 2 3 4 5 6 7  8 9  10 12
	 * 2 3 4 5 6 7 15 9 14 11 13
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 2a, should print true\n2 3 4 5 6 7 15 9 14 11 13
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(11);
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);
	//case 2b, should print true\n 6
	va.clear();
	va.push_back(5);
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);

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
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);

	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(11);
	vb.push_back(1);
	vb.push_back(2);
	vb.push_back(3);
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);
	//case 2d, should print true\n 15
	va.clear();
	va.push_back(7);
	vb.push_back(5);
	vb.push_back(8);
	vb.push_back(15);
	cout << f.follows(&va, &vb, 1) << endl;
	printVector(vb);
}


void f3()
{
	Follows f;

	f.addFollows(1, 2);
	f.addFollows(2, 3);
	f.addFollows(3, 4);
	f.addFollows(4, 5);
	f.addFollows(5, 6);
	f.addFollows(6, 7);
	f.addFollows(7, 15);
	f.addFollows(8, 9);
	f.addFollows(9, 14);
	f.addFollows(10, 11);
	f.addFollows(12, 13);

	/* Follow table is
	 * 1 2 3 4 5 6 7  8 9  10 12
	 * 2 3 4 5 6 7 15 9 14 11 13
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 3a, should print true\n1 2 3 4 5 6 7 8 9 10 12
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(12);
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);
	//case 3b, should print true\n 10
	vb.clear();
	vb.push_back(11);
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);

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
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	vb.push_back(12);
	va.push_back(1);
	va.push_back(2);
	va.push_back(3);
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);
	//case 3d, should print true\n 6
	vb.clear();
	vb.push_back(7);
	va.push_back(6);
	va.push_back(8);
	va.push_back(15);
	cout << f.follows(&va, &vb, 2) << endl;
	printVector(va);
}


void f4()
{
	Follows f;

	f.addFollows(1, 2);
	f.addFollows(2, 3);
	f.addFollows(3, 4);
	f.addFollows(4, 5);
	f.addFollows(5, 6);
	f.addFollows(6, 7);
	f.addFollows(7, 15);
	f.addFollows(8, 9);
	f.addFollows(9, 14);
	f.addFollows(10, 11);
	f.addFollows(12, 13);

	/* Follow table is
	 * 1 2 3 4 5 6 7  8 9  10 12
	 * 2 3 4 5 6 7 15 9 14 11 13
	 *
	 */
	vector<int> va;
	vector<int> vb;

	//case 4a, should print true
	//1 2 3 4 5 6 7 8 9 10 12
	//2 3 4 5 6 7 15 9 14 11 13
	cout << f.follows(&va, &vb, 3) << endl;
	printVector(va);
	printVector(vb);

	//case 4b, should print true
	//7 8
	//15 9
	va.clear();
	vb.clear();
	vb.push_back(12);
	vb.push_back(15);
	vb.push_back(9);
	vb.push_back(16);
	cout << f.follows(&va, &vb, 3) << endl;
	printVector(va);
	printVector(vb);

	va.clear();
	vb.clear();

	//case 4c, should print true
	//2 12
	//3 13
	va.push_back(2);
	va.push_back(11);
	va.push_back(13);
	va.push_back(12);
	cout << f.follows(&va, &vb, 3) << endl;
	printVector(va);
	printVector(vb);

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(12);
	vb.push_back(12);
	va.push_back(1);
	va.push_back(3);
	cout << f.follows(&va, &vb, 3) << endl;
	printVector(va);
	printVector(vb);
	//case 3d, should print true
	//6 7
	//7 15
	vb.push_back(7);
	vb.push_back(7);
	vb.push_back(7);
	vb.push_back(15);
	va.push_back(6);
	va.push_back(8);
	va.push_back(15);
	va.push_back(7);
	cout << f.follows(&va, &vb, 3) << endl;
	printVector(va);
	printVector(vb);
}


int follows_main()
{
	//f1();
	//f2();
	//f3();
	f4();
	return(0);
}


void printVector(vector<int> v)
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
