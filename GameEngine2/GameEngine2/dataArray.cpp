#include "dataArray.h"



dataArray::dataArray()
{
}


dataArray::~dataArray()
{
	delete[] myArray;
}

void dataArray::init() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			myArray[i][j] = 0;
		}
	}
}

void dataArray::setArray(int i, int j, int x) {
	myArray[i][j] = x;
}

int dataArray::getArray(int i, int j) {
	return myArray[i][j];
}