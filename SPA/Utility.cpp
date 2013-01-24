#include "stdafx.h"
#include "Utility.h"

int indexOf(vector<int> list, int val)
{
	for (int i = 0; i < list.size(); i++) {
		if (list.at(i) == val) {
			return(i);
		}
	}
	return(-1);
}
