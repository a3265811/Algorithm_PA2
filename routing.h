#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

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
	vector<pair<int,int>> road;
	vector<int> order;
};

class router{
public:
	router(){}
	~router(){}

	void parser(int argc, char*argv[]);
	void route();
	void backtrace();
	void output();
private:
	int grid_x;
	int grid_y;
	int capacity;
	vector<vector<int>> v_edge;
	vector<vector<int>> h_edge;
	vector<net> nets;
};

class node{
public:
	node(){
		distance = -1;
		found = false;
	}
	~node(){}

	friend class router;
private:
	int distance;
	bool found;
};
