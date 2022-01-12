#include "routing.h"

void router::parser(int argc, char*argv[]){
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
	for(int i = 0; i < grid_y; i++){
		vector<int> temp_v;
		for(int j = 0; j < grid_x - 1; j++)
			temp_v.push_back(capacity);
		h_edge.push_back(temp_v);
	}
	for(int i = 0; i < grid_y - 1; i++){
		vector<int> temp_v;
		for(int j = 0; j < grid_x; j++){
			temp_v.push_back(capacity);
		}
		v_edge.push_back(temp_v);
	}
}

void router::route(){
	cout << "h_edge:" << "[" << h_edge.size() << "," << h_edge[0].size() << "]" << endl;
	cout << "v_edge:" << "[" << v_edge.size() << "," << v_edge[0].size() << "]" << endl;
	// BFS
	queue<pair<int,int>> waiting;
	for(int i = 0; i < nets.size(); i++){
		cout << "new route" << endl;
		pair<int,int> start = pair<int,int>(nets[i].y1,nets[i].x1);
		waiting.push(start);
		vector<vector<node>> path;
		int step = 0;
		for(int j = 0; j < grid_y; j++){
			vector<node>temp(grid_x);
			path.push_back(temp);
		}
		path[start.first][start.second].distance = 0;
		path[start.first][start.second].found = true;
		while(!waiting.empty()){
			step++;
			pair<int,int> cur = waiting.front();
			waiting.pop();
			if(cur.second - 1 >= 0 && !path[cur.first][cur.second-1].found){ // left grid search
				waiting.push(pair<int,int>(cur.first, cur.second-1));
				path[cur.first][cur.second-1].distance = step;
				path[cur.first][cur.second-1].found = true;
			}
			if(cur.second + 1 < grid_x && !path[cur.first][cur.second+1].found){ // right grid search
				waiting.push(pair<int,int>(cur.first, cur.second+1));
				path[cur.first][cur.second+1].distance = step;
				path[cur.first][cur.second+1].found = true;
			}
			if(cur.first - 1 >= 0 && !path[cur.first-1][cur.second].found){ // up grid search
				waiting.push(pair<int,int>(cur.first-1, cur.second));
				path[cur.first-1][cur.second].distance = step;
				path[cur.first-1][cur.second].found = true;
			}
			if(cur.first + 1 < grid_y && !path[cur.first+1][cur.second].found){ // down grid search
				waiting.push(pair<int,int>(cur.first+1, cur.second));
				path[cur.first+1][cur.second].distance = step;
				path[cur.first+1][cur.second].found = true;
			}
		}
		/*for(int j = 0; j < grid_y; j++){
			for(int k = 0; k < grid_x; k++)
				cout << path[j][k].distance << " ";
			cout << endl;
		}*/
		
		
		//backtrace
		pair<int,int> finder = pair<int,int>(nets[i].y2,nets[i].x2);
		nets[i].road.push_back(pair<int,int>(nets[i].x2,nets[i].y2));
		/*cout << "------------------------------------------" << endl;
		cout << "finder: (" << finder.first << "," << finder.second << ")" << endl;*/
		while(finder != start){
	/*		cout << last << endl;
			cout << "finder: (" << finder.first << "," << finder.second << ")" << endl;*/
		}
//		cout << "------------------------------------------" << endl;
		
	}
}

void backtrace(pair<int,int>finder, pair<int,int>start, int mode){
	string last = ""; // (side) up down left right
	if(mode == 0 && h_edge[finder.first][finder.second-1] > 0){
		nets[i].road.push_back(pair<int,int>(finder.second-1,finder.first));
		h_edge[finder.first][finder.second-1]--;
		finder.second--;
		last = "r";
	}
	else if(mode == 1 && v_edge[finder.first-1][finder.second] > 0){
		nets[i].road.push_back(pair<int,int>(finder.second,finder.first-1));
		v_edge[finder.first-1][finder.second]--;
		finder.first--;
		last = "d";
	}
	else if(mode == 2 && h_edge[finder.first][finder.second] > 0){
		nets[i].road.push_back(pair<int,int>(finder.second+1,finder.first));
		h_edge[finder.first][finder.second]--;
		finder.second++;
		last = "l";
	}
	else if(mode == 3 && v_edge[finder.first][finder.second] > 0){
		nets[i].road.push_back(pair<int,int>(finder.second,finder.first+1));
		v_edge[finder.first][finder.second]--;
		finder.first++;
		last = "u";
	}
	else{
		cout << "no road to find (" << finder.first << "," << finder.second << ")" << endl;
	}
}

void router::output(){
}
