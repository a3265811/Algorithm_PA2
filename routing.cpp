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

bool router::compare(net n1, net n2){
	int MD1 = abs(n1.x2 - n1.x1) + abs(n1.y2 - n1.y1);
	int MD2 = abs(n2.x2 - n2.x1) + abs(n2.y2 - n2.y1);
	return MD1 > MD2;
}

void router::route(){
	sort(nets.begin(),nets.end(),router::compare);
	// BFS
	queue<pair<int,int>> waiting;
	for(int i = 0; i < nets.size(); i++){
		pair<int,int> start = pair<int,int>(nets[i].y1,nets[i].x1);
		pair<int,int> target = pair<int,int>(nets[i].y2,nets[i].x2);
		waiting.push(start);
		vector<vector<node>> path;
		for(int j = 0; j < grid_y; j++){
			vector<node>temp(grid_x);
			path.push_back(temp);
		}
		path[start.first][start.second].distance = 0;
		path[start.first][start.second].found = true;
		while(!waiting.empty()){
			pair<int,int> cur = waiting.front();
			waiting.pop();
			int counter = 0;
			if(cur.second - 1 >= 0 && !path[cur.first][cur.second-1].found){ // left grid search
				if(h_edge[cur.first][cur.second-1] > 0){
					waiting.push(pair<int,int>(cur.first, cur.second-1));
					path[cur.first][cur.second-1].distance = path[cur.first][cur.second].distance + 1;
					path[cur.first][cur.second-1].found = true;
					path[cur.first][cur.second-1].predecessor = cur;
					path[cur.first][cur.second-1].from_dir = 1;
					counter++;
				}
			}
			if(cur.second + 1 < grid_x && !path[cur.first][cur.second+1].found){ // right grid search
				if(h_edge[cur.first][cur.second] > 0){
					waiting.push(pair<int,int>(cur.first, cur.second+1));
					path[cur.first][cur.second+1].distance = path[cur.first][cur.second].distance + 1;
					path[cur.first][cur.second+1].found = true;
					path[cur.first][cur.second+1].predecessor = cur;
					path[cur.first][cur.second+1].from_dir = 0;
					counter++;
				}
			}
			if(cur.first - 1 >= 0 && !path[cur.first-1][cur.second].found){ // up grid search
				if(v_edge[cur.first-1][cur.second] > 0){
					waiting.push(pair<int,int>(cur.first-1, cur.second));
					path[cur.first-1][cur.second].distance = path[cur.first][cur.second].distance + 1;
					path[cur.first-1][cur.second].found = true;
					path[cur.first-1][cur.second].predecessor = cur;
					path[cur.first-1][cur.second].from_dir = 3;
					counter++;
				}
			}
			if(cur.first + 1 < grid_y && !path[cur.first+1][cur.second].found){ // down grid search
				if(v_edge[cur.first][cur.second] > 0){
					waiting.push(pair<int,int>(cur.first+1, cur.second));
					path[cur.first+1][cur.second].distance = path[cur.first][cur.second].distance + 1;
					path[cur.first+1][cur.second].found = true;
					path[cur.first+1][cur.second].predecessor = cur;
					path[cur.first+1][cur.second].from_dir = 2;
					counter++;
				}
			}

			if(counter == 0 && (cur == start || path[target.first][target.second].predecessor == pair<int,int>(-1,-1))){
				waiting.push(cur);
				if(0 <= cur.first && cur.first < h_edge.size() && 0 <= cur.second-1 && cur.second-1 < h_edge.size())
					h_edge[cur.first][cur.second-1]++;
				if(0 <= cur.first && cur.first < h_edge.size() && 0 <= cur.second && cur.second < h_edge.size())
					h_edge[cur.first][cur.second]++;
				if(0 <= cur.first-1 && cur.first-1 < v_edge.size() && 0 <= cur.second && cur.second < v_edge.size())
					v_edge[cur.first-1][cur.second]++;
				if(0 <= cur.first && cur.first < v_edge.size() && 0 <= cur.second && cur.second < v_edge.size())
					v_edge[cur.first][cur.second]++;
			}
		}

		//backtrace ver 2
		pair<int,int> finder = target;
		while(finder != start){
			int dir = path[finder.first][finder.second].from_dir;
			if(dir == 0){ //left track
				if(h_edge[finder.first][finder.second-1] > 0){
					h_edge[finder.first][finder.second-1]--;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first));
					finder = path[finder.first][finder.second].predecessor;
				}
			}
			else if(dir == 1){ //right track
				if(h_edge[finder.first][finder.second] > 0){
					h_edge[finder.first][finder.second]--;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first));
					finder = path[finder.first][finder.second].predecessor;
				}
			}
			else if(dir == 2){ // up track
				if(v_edge[finder.first-1][finder.second] > 0){
					v_edge[finder.first-1][finder.second]--;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first));
					finder = path[finder.first][finder.second].predecessor;
				}
			}
			else if(dir == 3){ // down track
				if(v_edge[finder.first][finder.second] > 0){
					v_edge[finder.first][finder.second]--;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first));
					finder = path[finder.first][finder.second].predecessor;
				}
			}
		}
		nets[i].road.push_back(pair<int,int>(nets[i].x1,nets[i].y1));

	/*	
		//backtrace ver1
		pair<int,int> finder = pair<int,int>(nets[i].y2,nets[i].x2);
		pair<int,int> last_pos;
		nets[i].road.push_back(pair<int,int>(nets[i].x1,nets[i].y1));
		vector<int> path_dir; // (side) 0:left 1:right 2:up 3:down
		bool privilege = false;
		set<int> forbiden;
		cout << "------------------------------------------" << endl;
		cout << "finder: (" << finder.first << "," << finder.second << ")" << endl;
		while(finder != start){
			cout << "finder:(" << finder.first << "," << finder.second << ")" << endl;
			if(( privilege || finder.second > start.second) && forbiden.find(0) == forbiden.end()){
				if(h_edge[finder.first][finder.second-1] > 0){
					cout << "go:" << 0 << endl;
					nets[i].road.push_back(pair<int,int>(finder.second-1,finder.first));
					h_edge[finder.first][finder.second-1]--;
					finder.second--;
					path_dir.push_back(0);
					forbiden.clear();
					forbiden.insert(1);
					privilege = false;
				}
				else
					forbiden.insert(0);
			}
			else if(( privilege || finder.first > start.first) && forbiden.find(2) == forbiden.end()){
				if(v_edge[finder.first-1][finder.second] > 0){
					cout << "go:" << 2 << endl;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first-1));
					v_edge[finder.first-1][finder.second]--;
					finder.first--;
					path_dir.push_back(2);
					forbiden.clear();
					forbiden.insert(3);
					privilege = false;
				}
				else
					forbiden.insert(2);
			}
			else if((privilege || finder.second < start.second) && forbiden.find(1) == forbiden.end()){
				if(h_edge[finder.first][finder.second] > 0){
					cout << "go:" << 1 << endl;
					nets[i].road.push_back(pair<int,int>(finder.second+1,finder.first));
					h_edge[finder.first][finder.second]--;
					finder.second++;
					path_dir.push_back(1);
					forbiden.clear();
					forbiden.insert(0);
					privilege = false;
				}
				else
					forbiden.insert(1);
			}
			else if((privilege || finder.first < start.first) && forbiden.find(3) == forbiden.end()){
				if(v_edge[finder.first][finder.second] > 0){
					cout << "go:" << 3 << endl;
					nets[i].road.push_back(pair<int,int>(finder.second,finder.first+1));
					v_edge[finder.first][finder.second]--;
					finder.first++;
					path_dir.push_back(3);
					forbiden.clear();
					forbiden.insert(2);
					privilege = false;
				}
				else
					forbiden.insert(3);
			}
			else{
				//cout << "start:" << nets[i].x1 << "," << nets[i].y1 << endl;
				//cout << "target:" << nets[i].x2 << "," << nets[i].y2 << endl;
				cout << "path_dir size: " << path_dir.size() << endl;
				cout << "no road to find (" << finder.first << "," << finder.second << ")" << endl;
				for(int i = 0; i < path_dir.size(); i++){
					cout << path_dir[i] << " ";
				}
				cout << endl;
				if(path_dir.size() == 0){
					privilege = true;
				}
				else{
					privilege = true;
					forbiden.insert(path_dir.back());
					if(path_dir.back() == 0){
						h_edge[finder.first][finder.second]++;
						finder.second++;
						forbiden.insert(0);
						cout << "back:" << 0 << endl;

					}
					else if(path_dir.back() == 1){
						h_edge[finder.first][finder.second-1]++;
						finder.second--;
						forbiden.insert(1);
						cout << "back:" << 1 << endl;
					}
					else if(path_dir.back() == 2){
						v_edge[finder.first][finder.second]++;
						finder.first++;
						forbiden.insert(2);
						cout << "back:" << 2 << endl;
					}
					else if(path_dir.back() == 3){
						v_edge[finder.first-1][finder.second]++;
						finder.first--;
						forbiden.insert(3);
						cout << "back:" << 3 << endl;
					}
					path_dir.pop_back();
					nets[i].road.pop_back();

					int counter = 0;
					if(finder.first == 0){
						forbiden.insert(2);
						counter++;
					}
					if(finder.first == grid_y - 1){
						forbiden.insert(3);
						counter++;
					}
					if(finder.second == 0){
						forbiden.insert(0);
						counter++;
					}
					if(finder.second == grid_x - 1){
						forbiden.insert(1);
						counter++;
					}
				}
				cout << "move back finder:(" << finder.first << "," << finder.second << ")" << endl;
			}
			cout << last << endl;
			cout << "finder: (" << finder.first << "," << finder.second << ")" << endl;
		}
//		cout << "------------------------------------------" << endl;
		
		nets[i].road.push_back(pair<int,int>(nets[i].x2,nets[i].y2));*/
		
	}
}

void router::output(int argc, char*argv[]){
	fstream fout;
	fout.open(argv[2],ios::out);
	if(!fout){
		cout << "Can't open file:" << argv[2] << endl;
		return;
	}
	int counter = 0;
	for(int i = 0; i < nets.size(); i++){
		fout << nets[i].name << " " << nets[i].road.size()-1 << endl;
		for(int j = nets[i].road.size()-1; j > 0; j--){
			fout << nets[i].road[j].first << " " << nets[i].road[j].second << " " << nets[i].road[j-1].first << " " << nets[i].road[j-1].second << endl;
		}
		if(nets[i].road.size() == 0){
			counter++;
		}
	}
	if(counter != 0)
		cout << "miss connection" << endl;
}
