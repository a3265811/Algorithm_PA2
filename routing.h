#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include <math.h>
#include <algorithm>

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
};

class router{
public:
	router(){}
	~router(){}

	void parser(int argc, char*argv[]);
	static	bool compare(net n1, net n2);
	void route();
	void output(int argc, char*argv[]);
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
		predecessor = pair<int,int>(-1,-1);
		int from_dir = -1;
	}
	~node(){}

	friend class router;
private:
	int distance;
	bool found;
	pair<int,int> predecessor;
	int from_dir;
};
