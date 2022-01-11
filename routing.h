#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class net{
public:
	net(){}
	~net(){}
	friend class router;
private:
	string name;
	int x1;
	int y1;
	int x2;
	int y2;
};

class router{
public:
	router(){}
	~router(){}

	void parser(int argc, char*argv[]);
	void route();
	void output();
private:
	int grid_x;
	int grid_y;
	int capacity;
	vector<int> v_edge;
	vector<int> h_edge;
	vector<net> nets;
};
