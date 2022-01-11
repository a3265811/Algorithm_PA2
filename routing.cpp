#include "routing.h"

void router::parser(int argc, char*argv[]){
	cout << "hi" << endl;
	fstream fin;
	fin.open(argv[1],ios::in);
	if(!fin){
		cout << "Can't open the input file" << endl;
		return;
	}

	string str;
	fin >> str >> grid_x >> grid_y;
	fin >> str >> capacity;
	fin >> str >> str >> str;
	while(getline(fin,str)){
		if(str == "")
			continue;
		net temp_n;
		stringstream ss;
		ss << str;
		ss >> temp_n.name >> temp_n.x1 >> temp_n.y1 >> temp_n.x2 >> temp_n.y2;
		nets.push_back(temp_n);
	}
	fin.close();
	for(int i = 0; i < grid_x - 1; i++){
		vector temp_v(grid_y,0);
		h_edge.push_back(temp_v);
	}
	for(int i = 0; i < grid_y - 1; i++){
		vector temp_v(grid_x,0);
		v_edge.push_back(temp_v);
	}
}

void router::route(){
}

void router::output(){
}
