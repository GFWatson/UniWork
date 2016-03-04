#pragma once
class dataArray
{
public:
	dataArray();
	~dataArray();
	void init();
	void setArray(int i, int j, int x);
	int getArray(int i, int j);
private:
	int myArray[100][100];
};

