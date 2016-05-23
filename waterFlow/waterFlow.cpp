#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <list>

using namespace std;

ofstream outfile("output.txt",ofstream::out);

class Offtime
{
public:
	int off[100];
	Offtime();
};
Offtime::Offtime()
{
	for(int i=0; i<24; i++)
	{
		off[i] = 1000025;
	}
}

class Node
{
public:
	string node_name; 
	int node_weight; 
	Node(string node_name, int node_weight);
};

Node::Node(string node_name, int node_weight)
{
	this->node_name = node_name;
	this->node_weight = node_weight; 
}



struct intOrder{
	bool operator() (Node Node1, Node Node2) 
	{
		return  Node1.node_name < Node2.node_name;
	}
}intorder;


struct intOrder2{ 
	bool operator() (Node Node1, Node Node2)
	{
		return  Node1.node_weight < Node2.node_weight;
	}
}intorder2;


class Graph
{
private:
	int node_num;
	map<string,vector<Node> > adj; 
	map<string,map<string,int> > distance;
	map<string,map<string,int> > distance_bfs_dfs; 
	map<string,int> weight; 
	map<string,int> weight1; 
	map<string,map<string,Offtime> > off_pipe; 
	
public:
	Graph(int node_num);
	void addEdge(Node Node1, Node Node2, int weight, int *off_time);
	void BFS(Node start_node, vector<string> goal_nodes, int time);
	void DFS(Node start_node, vector<string> goal_nodes, int time);
	void UCS(Node start_node, vector<string> goal_nodes, int time);
};

Graph::Graph(int node_num) 
{
	this->node_num = node_num;
}

void Graph::addEdge(Node NodeParent, Node NodeChild, int w, int *off_time)
{
	weight1[NodeParent.node_name] = 100023;
	weight1[NodeChild.node_name] = 100023; 

	distance[NodeParent.node_name][NodeChild.node_name] = w; 
	distance_bfs_dfs[NodeParent.node_name][NodeChild.node_name] = 1; 
		
	for(int i=0; i<24; i++) 
		off_pipe[NodeParent.node_name][NodeChild.node_name].off[i] = off_time[i];


	adj[NodeParent.node_name].push_back(NodeChild); 
	sort(adj[NodeParent.node_name].begin(), adj[NodeParent.node_name].end(), intorder); 

}

void Graph::BFS(Node start_node, vector<string> goal_nodes, int time)
{
	Node closed_node = start_node;
	
	queue<Node> qu;

	map<string,int> check_B; 
	check_B[closed_node.node_name] = 0;

	weight[start_node.node_name] = time; 
	start_node.node_weight = time; 

	qu.push(start_node);

	printf("BFS : ");
	while(true)
	{
		if(qu.empty()) 
		{
			printf("Fail to find the node \n");
			outfile << "None";
			outfile << "\n";
			break;
		}

		closed_node = qu.front();

		if(goal_nodes.end() != 
			find(goal_nodes.begin(), goal_nodes.end(), closed_node.node_name)) 
		{
			cout << endl;
			cout << endl;
			cout << "node : " << closed_node.node_name << "("<< closed_node.node_weight << ")" <<  endl;
			cout << "times : " << closed_node.node_weight % 24 << "seconds" << endl;
			outfile << closed_node.node_name;
			outfile << " ";
			outfile << closed_node.node_weight % 24;
			outfile << "\n";
		
			break;
		}

		cout << closed_node.node_name << " ";
		check_B[closed_node.node_name] = 1; 
			
		qu.pop();
			
		for(int i=0; i<adj[closed_node.node_name].size(); i++)
		{
			if(check_B[adj[closed_node.node_name][i].node_name] == 0)
			{
				weight[adj[closed_node.node_name][i].node_name] 
				= weight[closed_node.node_name] 
				+ distance_bfs_dfs[closed_node.node_name][adj[closed_node.node_name][i].node_name];
				
				adj[closed_node.node_name][i].node_weight 
					= weight[adj[closed_node.node_name][i].node_name];

				qu.push(adj[closed_node.node_name][i]);
				check_B[adj[closed_node.node_name][i].node_name] = 1;
			}
		}
	}
	printf("\n");
}


void Graph::DFS(Node start_node, vector<string> goal_nodes, int time)
{
	Node closed_node = start_node; 
	
	stack<Node> st; 

	map<string,int> check_D; 
	check_D[closed_node.node_name] = 0; 

	weight[start_node.node_name] = time; 
	start_node.node_weight = time; 

	st.push(start_node); 

	printf("DFS : ");
	while(true)
	{
		if(st.empty()) 
		{
			printf("fail to find the node \n");
			outfile << "None";
			outfile << "\n";
			break;
		}

		closed_node = st.top();

		if(goal_nodes.end() != 
			find(goal_nodes.begin(), goal_nodes.end(), closed_node.node_name)) 
		{
			cout << endl;
			cout << endl;
			cout << "node : " << closed_node.node_name << "("<< closed_node.node_weight << ")" << endl;
			cout << "times : " << closed_node.node_weight % 24 << "seconds" << endl;
			outfile << closed_node.node_name;
			outfile << " ";
			outfile << closed_node.node_weight % 24;
			outfile << "\n";
		
			break;
		}
		cout << closed_node.node_name << " ";
		check_D[closed_node.node_name] = 1;
		
		st.pop(); 

		for(int i=adj[closed_node.node_name].size()-1; i>=0; i--) 
		{
			if(check_D[adj[closed_node.node_name][i].node_name] == 0) 
			{
				weight[adj[closed_node.node_name][i].node_name] 
				= weight[closed_node.node_name] 
				+ distance_bfs_dfs[closed_node.node_name][adj[closed_node.node_name][i].node_name]; 
				
				adj[closed_node.node_name][i].node_weight 
					= weight[adj[closed_node.node_name][i].node_name]; 

				st.push(adj[closed_node.node_name][i]); 
				check_D[adj[closed_node.node_name][i].node_name] = 1;
			}
		}
	}
	printf("\n");
}


void Graph::UCS(Node start_node, vector<string> goal_nodes, int time)
{
	Node closed_node = start_node; 
	
	list<Node> open_queue1;
	
	map<string,int> check_U;
	check_U[closed_node.node_name] = 0;
	
	weight[start_node.node_name] = time; 
	start_node.node_weight = time;

	open_queue1.push_back(start_node);

	vector<Node> save_goal_nodes; 

	printf("UCS : "); 
	while(!open_queue1.empty())
	{
		open_queue1.sort(intorder2); 

		closed_node = open_queue1.front();
		
		if(goal_nodes.end() != 
			find(goal_nodes.begin(), goal_nodes.end(), closed_node.node_name)) 
		{
			save_goal_nodes.push_back(closed_node); 
			sort(save_goal_nodes.begin(), save_goal_nodes.end(), intorder2); 
		}
		
		cout << closed_node.node_name << "(" << closed_node.node_weight << ") ";
		check_U[closed_node.node_name] = 1; 
	
		open_queue1.pop_front();
			
		for(int i=0; i<adj[closed_node.node_name].size(); i++) 
		{

			if(check_U[adj[closed_node.node_name][i].node_name] == 0) 
			{
				int time = weight[closed_node.node_name] % 24; 
				bool a = true;
				for(int start_time=0; start_time<24; start_time++) {
					if(time
					== off_pipe[closed_node.node_name][adj[closed_node.node_name][i].node_name].off[start_time]) {

						a = false;			
					}
				}
				if(a == true) 
				{ 
					if(weight1[adj[closed_node.node_name][i].node_name] >= 
						weight[closed_node.node_name] 
						+ distance[closed_node.node_name][adj[closed_node.node_name][i].node_name]) 
					{ 
						weight[adj[closed_node.node_name][i].node_name]
						= weight[closed_node.node_name]
						+ distance[closed_node.node_name][adj[closed_node.node_name][i].node_name];

						weight1[adj[closed_node.node_name][i].node_name]
						= weight[adj[closed_node.node_name][i].node_name];

						adj[closed_node.node_name][i].node_weight
						= weight1[adj[closed_node.node_name][i].node_name]; 
					}
					else{
						adj[closed_node.node_name][i].node_weight
						= weight1[adj[closed_node.node_name][i].node_name]; 
					}
					open_queue1.push_back(adj[closed_node.node_name][i]);
				}
			}
		}
	}
	printf("\n");

	if(!save_goal_nodes.empty()) 
	{
			
			Node goal_node = save_goal_nodes[0];

			cout << endl;
			cout << endl;
			cout << "node : " << goal_node.node_name << "("<< goal_node.node_weight << ")" << endl;
			cout << "times : " << goal_node.node_weight % 24 << "seconds" << endl;
			outfile << goal_node.node_name;
			outfile << " ";
			outfile << goal_node.node_weight % 24;
			outfile << "\n";

			cout << endl;
	}
	else {
		printf("fail to find the node \n");
		outfile << "None";
		outfile << "\n";
		cout << endl;
	}
}

int main(int argc, const char *argv[])
{
	string space;
	char line;

	int number_of_test;
	string test_case;
	string start_node;
	string goal_node; 
	string middle_node; 
	int number_of_graph; 
	string connection;
	string parent_node; 
	string child_node; 
	int distance;
	int number_of_offpipe; 
	int start_offtime; 
	int end_offtime; 
	int initial_node_weight; 

	ifstream infile(argv[2],ifstream::in);
  
	
	infile >> number_of_test; 
	cout << number_of_test << endl;

	getline(infile,space); 

	for(int i=0; i<number_of_test; i++) 
	{
		vector<string> goal_nodes; 
		Graph g(100); 

		getline(infile,test_case); 
      
      	if (test_case[test_case.size() - 1] == '\r')
			test_case.resize(test_case.size() - 1);

		cout << test_case << endl; 
		
		getline(infile,start_node); 
        
      	if (start_node[start_node.size() - 1] == '\r')
			start_node.resize(start_node.size() - 1);
      
		cout << start_node << endl;
		Node(start_node,0);
	
		string temp;
		getline(infile,temp);
      
        if (temp[temp.size() - 1] == '\r')
			temp.resize(temp.size() - 1);
      
		istringstream ss(temp);
		while(!ss.eof()) {
			ss >> temp;
			goal_nodes.push_back(temp);
		}
		for(vector<string>::iterator it = goal_nodes.begin(); 
			it != goal_nodes.end(); ++it)
				cout << *it << " ";
		
		cout << endl;

		getline(infile,middle_node); 
      
        if (middle_node[middle_node.size() - 1] == '\r')
			middle_node.resize(middle_node.size() - 1);
      
		cout << middle_node << endl; 
	
		infile >> number_of_graph; 
		cout << number_of_graph << endl;

		getline(infile,space); 

		int off_time[24]; 
		int num = 0;
	
		for(int i=0; i<24; i++)
			off_time[i] = 100013;
	
		for(int i=0; i<number_of_graph; i++) 
		{
			string connection;
			getline(infile,connection); 
                      
       		if (connection[connection.size() - 1] == '\r')
				connection.resize(connection.size() - 1);
          
			istringstream ss(connection);
		
			ss >> parent_node;
			ss >> child_node;
			ss >> distance;
			ss >> number_of_offpipe;
			cout << parent_node << " " << child_node << " " << distance << " " << number_of_offpipe << " ";

				for(int k=0; k<number_of_offpipe; k++) 
				{
					ss >> start_offtime;
					ss >> line;
					ss >> end_offtime;

					for(int num=start_offtime; num<=end_offtime; num++) 
					{
						off_time[num] = num; 
					}
					cout << start_offtime << line << end_offtime << " ";
				}
				cout << endl;

				g.addEdge(Node(parent_node,0), Node(child_node,0), distance, off_time); 

				for(int a=0; a<24; a++) 
					off_time[a] = 100013;
		}

		infile >> initial_node_weight;
		cout << initial_node_weight << endl;

		getline(infile,space); 
		getline(infile,space); 

		cout << endl;

		if(test_case == "BFS")
		{
			g.BFS(Node(start_node,0), goal_nodes, initial_node_weight);
		}
		
		else if(test_case == "DFS")
		{
			g.DFS(Node(start_node,0), goal_nodes,initial_node_weight); 
		}

		else if(test_case == "UCS")
		{
			g.UCS(Node(start_node,0), goal_nodes,initial_node_weight); 
		}
	}

	cout << endl;
	cout << endl;

	infile.close();
	outfile.close();

	return 0;
}

