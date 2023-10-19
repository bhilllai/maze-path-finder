#include<stdio.h>
#include<iostream>
#include<set>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<cstdlib>
#include<list>
#include<stack>
 
 
 
 
using namespace std;
vector<int> solution;
 

class Graph{
	public:
		int V;
		vector<vector<int> > adj;//vector of nodes , each node is a vector of its adjacents
		int* no_of_adj;
	public:
		Graph(int V);//constructor
		void insert_edge(int v, int w);//adds new edge b/w v and w nodes
		vector<int> get_Adjacents(int V);//adjacency list for a given node
};
 
 
Graph::Graph(int v){
	V = v;
	no_of_adj = new int[V];//counts the no. of adjacents for a given node
	adj.resize(V, vector<int>(V,0));
	//resizes the upper vector to contain V elements and each initialised subvector to also conatain v elements initialised to 0
	for ( int i = 0 ; i < V ; i++ ){
		adj[i].resize(V);
		no_of_adj[i] = 0;//initialise all to 0
	}
}
 
void Graph::insert_edge(int v, int w){
 
	vector<int> temp_vector = adj[v];
	int ix = no_of_adj[v];
	for(int i = 0;i<ix;i++){//traversing the adjacency list of v node and if w is part of that,then exit the fn.
		if(w==temp_vector[i])
			return;
	}
	no_of_adj[v]++;//inc no of adjacent of v(no w to v)
	adj[v][no_of_adj[v]-1] = w;//if its not a part add w to adjacency list of v
 
}
 
vector<int> Graph::get_Adjacents(int V){
	vector<int> temp_vector = adj[V];
	temp_vector.erase(temp_vector.begin()+no_of_adj[V],temp_vector.begin()+temp_vector.size());
	//resize the adjacency list to no.of adjacents and return it as a vector
	return temp_vector;
}
 
 
class Maze{
	public:
		Graph* m_g;
		int HEIGHT;
		int WIDTH;
		int no_of_vertex;
		int *spanningTree_graph;
		int **cells;//double ppointer 
	public:
		Maze(int w,int h);
		void inMaze();
		void maze_solveDFS();
		void make_mazegrid();//draws the maze
		void test_for_n();
		int unvisited_nb(bool* visit,vector<int> adj_list);
};
 
 
Maze::Maze(int w,int h){
	WIDTH = w;
	HEIGHT = h;
	m_g = new Graph(w*h);
	no_of_vertex = HEIGHT*WIDTH;
	spanningTree_graph = new int[no_of_vertex];//array of int size of no. of cells allocated on heap
	cells = new int*[HEIGHT];//array of int pointers of size HEIGHT allocated on heap
 
	inMaze();
	maze_solveDFS();
 
	for(int i = 0; i < HEIGHT; i++){
		cells[i] = new int[WIDTH];//intialise each array entry as a pointer  dynamic array of size 'WIDTH'
		for(int column = 0;column<WIDTH;column++){
			cells[i][column] = 0xF; // initialise each entry of array of size WIDTH to 0xf
		}
	}
	make_mazegrid();
}
 
int Maze::unvisited_nb(bool* visited,vector<int> adj_list){
	int s = adj_list.size();
 
	set<int> rand_set;
	while(rand_set.size()<s){
		int r = rand()%s;//random member from adj list 
		rand_set.insert(r);
		if(!visited[adj_list[r]]){//if  visited is false return the rth element of adj_list 
			return adj_list[r];
		}
	}
	return -9;//else return -9
}
 
void Maze::test_for_n(){
	for(int i = 0;i<no_of_vertex;i++){
		m_g->get_Adjacents(i);
	}
}
 
void Maze::inMaze(){
	for(int row = 0;row<HEIGHT;row++){
		for(int column = 0;column<WIDTH;column++){//iterate through 
			int v = row*WIDTH+column;
			if(row>0){
				int v_top = (row-1)*WIDTH+(column);
				m_g->insert_edge(v,v_top);
				m_g->insert_edge(v_top,v);
			}
			if(column>0){
				int v_left = row*WIDTH+(column-1);
				m_g->insert_edge(v,v_left);
				m_g->insert_edge(v_left,v);
			}
			if(row<HEIGHT-1){
				int v_bottom = (row+1)*WIDTH+(column);
				m_g->insert_edge(v,v_bottom);
				m_g->insert_edge(v_bottom,v);
 
			}
			if(column<WIDTH-1){
				int v_right = (row)*WIDTH+(column+1);
				m_g->insert_edge(v,v_right);
				m_g->insert_edge(v_right,v);
			}
		}
	}
	//connecting cell with v= 0 to the cells its left and below.i.e making entriesin adjacency list of 0
	int v_temp = (0+1)*WIDTH+(0);
	m_g->insert_edge(0,v_temp);
	m_g->insert_edge(v_temp,0);
	//making entries into adjacency list of cells v=1 and v=width
	v_temp = (0)*WIDTH+(0+1);
	m_g->insert_edge(0,v_temp);
	m_g->insert_edge(v_temp,0);
}
 
void Maze::maze_solveDFS(){
	int s_v = 0;//current node
	stack<int> temp;
	 
	int no_of_vertex = no_of_vertex;
	bool visited[no_of_vertex];
	int count_visits = 1;
	int i;
	for(i = 0;i<no_of_vertex;i++){
		visited[i] = false;
	}
	visited[s_v] = true;
	spanningTree_graph[s_v] = -1;
	while(count_visits<no_of_vertex){
		vector<int> adj_list = m_g->get_Adjacents(s_v);
		random_shuffle ( adj_list.begin(), adj_list.end() );
		int count_adjacents = m_g->no_of_adj[s_v];
		int next_cell = unvisited_nb(visited,adj_list);
		
		if(next_cell!=-9){
			int w = next_cell;
			visited[w] = true;
			spanningTree_graph[w] = s_v;
			temp.push(w);
			s_v = w;
			count_visits++;
		}else{
			if(!temp.empty()){
				s_v = temp.top();
				temp.pop();
			}
		}
	}
}
 
void Maze::make_mazegrid(){// displays the maze grid on screen
	int v,r0,c0,v0;
	int RIGHT = 1; //0001 
	int UP = 2;    //0010 
	int LEFT = 4;  //0100 
	int DOWN = 8;  //1000 
	for(int row = 0;row<HEIGHT;row++){
		for(int column = 0;column<WIDTH;column++){
			v = row*WIDTH+column;//index of current node
			int w = spanningTree_graph[v]; //index of previous node
			if(w>=0){
				r0 = w/WIDTH;
				c0 = w%WIDTH;
			}
			v0 = r0*WIDTH+c0;
			if(column+1==c0){
				cells[row][column] &= ~RIGHT;//stores 1110
				cells[r0][c0] &= ~LEFT;		 //stores 1011
			}
			if(row+1==r0){
				cells[row][column] &= ~DOWN;//stores 0111
				cells[r0][c0] &= ~UP;		//stores 1101
			}
			if(row-1==r0){
				cells[row][column] &= ~UP;
				cells[r0][c0] &= ~DOWN;
			}
			if(column-1==c0){
				cells[row][column] &= ~LEFT;
				cells[r0][c0] &= ~RIGHT;
			}
		}
	}
 
	cells[0][0] &= ~LEFT;//keep open on left
	cells[HEIGHT-1][WIDTH-1] &= ~RIGHT;//keep open on right
 
//	for(int i = 0; i < HEIGHT; i++){
//		for(int column = 0;column<WIDTH;column++){
			//cells[i][column] = 0xF; //-> 1111
			// cout << cells[i][column]<<" ";
//		}
//		cout << endl;
//	}
}
 
void show_mazegrid(int **cells,int y,int x,int solve_action,int vx) {//y = height, x is width.i.e y is no. of rows and x is no.of columns
	/// if solve_action = 1 then prints the maze with solution if solution exists/user solves the maze
	/// if solve_action = 0 just prints the maze
	
		// black=0,
		// dark_blue=1,
		// dark_green=2,
		// dark_aqua,dark_cyan=3,
		// dark_red=4,
		// dark_purple=5,dark_pink=5,dark_magenta=5,
		// dark_yellow=6,
		// dark_white=7,
		// gray=8,
		// blue=9,
		// green=10,
		// aqua=11,cyan=11,
		// red=12,
		// purple=13,pink=13,magenta=13,
		// yellow=14,
		// white=15
	int color_cursor =12;
	int color_wall = 15;
	int i,j,v;
	cout << endl << endl;
	for (i = 0; i < y; i++) {
		/// draw the UP edge
		for (j = 0; j < x; j++) {
			v = x*i+j;
			if(solve_action==1){
				v = x*i+j;
				if(std::find(solution.begin(), solution.end(), v) != solution.end()) {
					if(((cells[i][j])&2)==0){//i,e cells contain 0010 or UP print "# +  " 
						cout <<"+ ";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), color_cursor|0);
						cout <<"+";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|color_wall);
						cout <<" ";
 
					}else{
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|color_wall);
						cout << "#===";
					}
				} else {
					SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|color_wall);
					cout << ((cells[i][j] & 2) == 0 ?"#   ": "#===");//if cell contains 0010[UP] print 1st else 2nd
				}
			}else{
				cout << ((cells[i][j] & 2) == 0 ?"#   ": "#===");
			}
		}
		cout << "#" << endl;
		/// draw the LEFT edge
		for (j = 0; j < x; j++) {
			v = x*i+j;
			if(solve_action==1){
				v = x*i+j;
				if(std::find(solution.begin(), solution.end(), v) != solution.end()) {
					if((cells[i][j] & 4) == 0){
						cout <<" ";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), color_cursor|0);
						cout <<"+";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|color_wall);
						cout <<"  ";
					}else{
						cout <<"| ";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), color_cursor|0);
						cout <<"+";
						SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|color_wall);
						cout <<" ";
 
					}
				} else {
					cout << ((cells[i][j] & 4) == 0 ?"    ": "|   ");
				}
			}else{
				cout << ((cells[i][j] & 4) == 0 ?"    ": "|   ");
			}
		}
 
		cout << ((cells[i][j-1] & 1)==0?"    ":"|");
		cout << endl;
	}
	for (int j = 0; j < x; j++) {
		cout << "#===";
	}
	cout << "+" << endl;
	SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|15);
 
}
void show_vector(vector<int> temp_vector){
	for (vector<int>::iterator iter = temp_vector.begin(); iter != temp_vector.end(); ++iter) {
		cout << *iter <<",";
	}
	cout << endl;
}
 
 
 
void maze_print(int **cells,int w,int h){
	for(int i = 0; i < h; i++){
		for(int column = 0;column<w;column++){
			cout << cells[i][column]<<" ";
		}
		cout << endl;
	}
}
 
 
 
 
int Get_unvisited_nodes(bool **visited,int x,int y,int WIDTH,int HEIGHT,int **cells){
	int v = 999;
	vector<int> a;
	if(x<HEIGHT-1){
		if((visited[x+1][y]==false)){//if cell below the current cell is not visited
			if((cells[x][y]&8)==0){//if the cell contains 0111 i.e to draw down
				v = (x+1)*WIDTH+y;//index of the cell below the current cell
				return v;
			}
		}
	}
	if(x>0){
		if(visited[x-1][y]==false){//if cell above the current cell is not visited
			if((cells[x][y]&2)==0){//if the cell contains 1101 i.e to draw up
				v = (x-1)*WIDTH+y;//index of th cell above curr cell
				return v;
			}
		}
	}
	if(y<WIDTH-1){
		if(visited[x][y+1]==false){
			if((cells[x][y]&1)==0){
				v = x*WIDTH+(y+1);
				return v;
			}
		}
	}
	if(y>0){
		if(visited[x][y-1]==false){
			if((cells[x][y]&4)==0){
				v = x*WIDTH+(y-1);
				return v;
			}
		}
	}
	return 999;
}
 
vector<int> Solving_algo(int **cells,int x1,int y1,int x2,int y2,int WIDTH,int HEIGHT){//x1,y1 starting point and x2,y2 ending point
	bool **visited;
	int row,column;
	visited = new bool*[HEIGHT];
	int i,j;
	for(i = 0; i < HEIGHT; i++){
		visited[i] = new bool[WIDTH];
		for(j = 0;j<WIDTH;j++){
			visited[i][j] = false;
		}
	}
	visited[x1][y1] = true;
	stack<int> stk;
	vector<int> soln_vector;
	int v = y1*WIDTH+x1;
	stk.push(v);
	while(!stk.empty()){
		int u_n = Get_unvisited_nodes(visited,x1,y1,WIDTH,HEIGHT,cells);
		if(u_n!=999){
			row = u_n/WIDTH;
			column = u_n%WIDTH;
			visited[row][column] = true;
			soln_vector.push_back(u_n);
			stk.push(u_n);
			if(row==x2 && column==y2){
				//cout <<"------------------------------------------>soln_vector FOUND"<<endl;
				//for(vector<int>::iterator it = soln_vector.begin();it!=soln_vector.end();it++){
					//cout <<*it<<" ,";
				//}
				return soln_vector;
			}
		}
		int t_v = stk.top();
 
 
		if(t_v==0){
			soln_vector.pop_back();
			int c = soln_vector.back();
			stk.push(c);
			x1 = c/WIDTH;
			y1 = c%WIDTH;
		}else{
			stk.pop();
			x1 = t_v/WIDTH;
			y1 = t_v%WIDTH;
		}
	}
	return soln_vector;
}
 
 
 
// bool myfunction (int i, int j) {
// 	return (i==j);
// }
 
vector<int> convert_stack_to_vector(stack<int> a){
	stack<int> b = a;
	vector<int> vec;
	while(!b.empty()){
		vec.push_back(b.top());
		b.pop();
	}
	return vec;
}
 
int turn_remove(char previous,char current){ // no turn if backtrack
	int turn_remove_status = 0;
	if(previous=='w'||previous=='W'){
		if(current == 's'||current=='S'){
			turn_remove_status = 1;
		}
	}
	if(previous=='s'||previous=='S'){
		if(current == 'W'||current=='w'){
			turn_remove_status = 1;
		}
	}
	if(previous=='a'||previous=='A'){
		if(current == 'D'||current=='d'){
			turn_remove_status = 1;
		}
	}
	if(previous=='D'||previous=='d'){
		if(current == 'a'||current=='A'){
			turn_remove_status = 1;
		}
	}
	return turn_remove_status;
}
int test_collision(int **cells,int x,int y,char direction){
	int proceed = 0;
	if(direction == 'w' || direction == 'W'){
		if((cells[y][x]&2)==2){
			proceed = 1;
		}
	}
	if(direction == 's' || direction == 'S'){
		if((cells[y][x]&8)==8){
			proceed = 1;
		}
 
	}
	if(direction == 'a' || direction == 'A'){
		if((cells[y][x]&4)==4){
			proceed = 1;
		}
	}
	if(direction == 'd' || direction == 'D'){
		if((cells[y][x]&1)==1){
			proceed = 1;
		}
	}
	return proceed;
}
void Maze_gameplay(Maze m){
	stack<int> turns;
	stack<char> t;
	int x = 0;
	int y = 0;
	while(true){
		system("cls");
		cout << "X: "<<x<<"Y: "<<y<<endl;
		if(x==m.HEIGHT-1&&y==m.WIDTH-1){
			SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|10);
			cout << "Game Over, You WON!!!\n";
			SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|15);
			cout << "Type 'X' to EXIT"<<endl;
		}
		show_mazegrid(m.cells,m.WIDTH,m.HEIGHT,1,0);
		cout << "Use w, a, s, d to play the game." << endl;
		cout << "Enter x to exit." << endl;
		char c;
		cin >> c;
		bool move_possible = true;
		if(c=='x')
			return;
		 if(c == 'w' || c == 'W'){
            if(y>0){
						if(test_collision(m.cells,x,y,c)==0){
							y = y-1;
						}else{
							move_possible = false;
						}
					}else{
						move_possible = false;
					}
        }
        else if(c == 's' || c == 'S'){
            if(y<m.HEIGHT-1){
						if(test_collision(m.cells,x,y,c)==0){
							y = y+1;
						}else{
							move_possible = false;
						}
					}else{
						move_possible = false;
					}
        }
        else if(c == 'a' || c == 'A'){
            if(x>0){
						if(test_collision(m.cells,x,y,c)==0){
							x = x-1;
						}else{
							move_possible = false;
						}
					}else{
						move_possible = false;
					}
        }
        else if(c == 'd' || c == 'D'){
            if(x<m.WIDTH-1){
						if(test_collision(m.cells,x,y,c)==0){
							x = x+1;
						}else{
							move_possible = false;
						}
					}else{
						move_possible = false;
					}
        }
        else{
				cout << "Enter a valid key!" << endl;
        }
		int v = m.WIDTH*y+x;
		if(t.empty() && turns.empty() && move_possible){
			t.push(c);
			turns.push(v);
		}else{
			if(move_possible){
				char check = t.top();
				if(turn_remove(check,c)==1){
					t.pop();
					turns.pop();
				}else{
					t.push(c);
					turns.push(v);
				}
			}
		}
		solution = convert_stack_to_vector(turns);
 
		//show_mazegrid(m.cells,m.WIDTH,m.HEIGHT,1,0);
	}
	//solution = convert_stack_to_vector(turns);
	//show_mazegrid(m.cells,m.WIDTH,m.HEIGHT,1,5,5,0);
}
void console_msg_1(){
	
	cout << "\tA.Find solution to the maze." << endl;
	cout << "\tB.Play the maze." << endl;
	cout << "\tPress C to Restart the game or Exit" << endl;
}

int main(){
	
	srand(time(NULL));
	
	cout<<"Enter size of the square matrix ";
	char c;
	char input;
	int exit_status  = 0;
	bool gameStatus = true;
	while(gameStatus){
		
		int width;
		int height;
		//console_msg_2();
		bool grid_size_viability = true;
		while(grid_size_viability){
			
			cin >> width ;
			height = width;
			if(width>2){
				grid_size_viability=false; //break out of loop
			}else{
				cout << "Press a  valid key!" << endl;
			}
		}
		Maze mg(height,width);
		
		SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|14);
		cout << "\t. . . . .  Creating MAZE . . . . . \n";
		
		SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|15);
		show_mazegrid(mg.cells,height,width,0,0);
		cout << endl;
		while(exit_status <1){
			console_msg_1();
			cin >> input;
			
         	if(input == 'a' || input == 'A'){
                solution = Solving_algo(mg.cells,0,0,height-1,width-1,height,width);
					Sleep(100);
					show_mazegrid(mg.cells,height,width,1,0);
            }
            else if(input == 'b' || input == 'B'){
                system("cls");
					show_mazegrid(mg.cells,height,width,0,0);
					solution.clear();
					Maze_gameplay(mg);
            }
            else if(input == 'c' || input == 'C'){
                exit_status += 1;
            }
		}
		exit_status  = 0;
 
		cout << "Press x to exit or press ANY other key to continue." << endl;
		cin >> c;
        
        if(c == 'x' || c == 'X'){
            gameStatus = false;
        }
        else
            cout<<"Loading Maze\n\n";
	}
 
}
