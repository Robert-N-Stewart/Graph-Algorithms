// ------------------------------------------------ graphm.cpp -------------------------------------------------------

// Programmer Name Course Section Number: Robert Stewart CSS 343 A

// Creation Date: 05/10/17

// Date of Last Modification: 05/10/17

// --------------------------------------------------------------------------------------------------------------------

// Purpose - this program acts as a graph that can perfom diskjati's path serach 

// -------------------------------------------------------------------------------------------------------------------- 

// --------------------------------------------------------------------------------------------------------------------

#include "graphm.h"

//------------------------- Default Constructor ---------------------------------
// creates a default graphm
// graphm will be empty
// Preconditions: None
// Postconditions: constructed empty graphm
GraphM::GraphM()
{
	size = 0;		//sets the default size of the graph to zero as there are no vertices

	modified = false;

	//for every from vertex, create nodedata to store in graph's nodedata array, create default distance matrix with distances set to infinity, and default table for visiting
	for (int from = 0; from < MAXNODESM; from++)
	{
		data[from] = NodeData();		//create default nodedata for each vertex

		//for every to vertex 
		for (int to = 0; to < MAXNODESM; to++)
		{

			//assigns the cost matrix to default values
			//default non existent edge cost is assigned to negative infinity
			//row 0 and column 0 are unused throughout this program
			C[from][to] = numeric_limits<int>::max();

			//assigns the cost matrix to default values
			//default non existent edge cost is assigned to infinity
			//row 0 and column 0 are unused throughout this program
			T[from][to] = TableType();

		}
	}

}

//------------------------- Copy Constructor ---------------------------------
// creates a graphm from passed graphm
// graphm will be a deep copy of passed graphm
// Preconditions: None
// Postconditions: constructed graphm that contains all deep copy of passed graph
GraphM::GraphM(const GraphM & passedGraph)
{
	size = passedGraph.size;		//sets the size of the implicit graph to the size of the passed graph

	modified = false;

	//for every from vertex, create nodedata to store in graph's nodedata array, create default distance matrix with distances set to infinity, and default table for visiting
	for (int from = 0; from < MAXNODESM; from++)
	{
		data[from] = NodeData(passedGraph.data[from]);		//create nodedata from passed graph's nodedata

		//for every to vertex. 
		for (int to = 0; to < MAXNODESM; to++)
		{

			//assigns the cost matrix to default values
			//default non existent edge cost is assigned to -1
			//row 0 and column 0 are unused throughout this program
			//leaves unused columns and rows unassigned as they will not be acessed
			C[from][to] = passedGraph.C[from][to];

			//assigns the cost matrix to default values
			//default non existent edge cost is assigned to -1
			//row 0 and column 0 are unused throughout this program
			//leaves unused columns and rows unassigned as they will not be acessed
			T[from][to] = TableType(passedGraph.T[from][to]);

		}
	}
}

//------------------------- buildGraph() ---------------------------------
// builds graph from ifstream of textfile
// Preconditions: text file of ifstream must be formatted in correctly
// Postconditions: the graphm will be constructed from text file
void GraphM::buildGraph(ifstream & infile)
{
	//if the implicit graph has already been built, reset data for new build
	if (size > 0) {
		//for every from vertex, create nodedata to store in graph's nodedata array, create default distance matrix with distances set to infinity, and default table for visiting
		for (int from = 1; from <= size; from++)
		{
			data[from] = NodeData();		//create default nodedata for each vertex

											//for every to vertex 
			for (int to = 1; to <= size; to++)
			{

				//assigns the cost matrix to default values
				//default non existent edge cost is assigned to negative infinity
				//row 0 and column 0 are unused throughout this program
				C[from][to] = numeric_limits<int>::max();

				//assigns the cost matrix to default values
				//default non existent edge cost is assigned to infinity
				//row 0 and column 0 are unused throughout this program
				T[from][to] = TableType();

			}
		}
	}

	if (infile.eof())		//if there the file is exhausted, exit build
		return;

	infile >> size;		//stores first line of graph text representing the number of nodedatas for the graph into size

	string s;
	getline(infile, s);

	if (size <= 0)		//if size of graph is zero, exit build
		return;


	//stores each line of the graph representing the nodedata into the data array
	//there is size number of nodedatas
	for (int i = 1; i <= size; i++) {
		data[i].setData(infile);
	}

	//assigns the cost matrix to the file values for edges
	//rows represent "from" nodes and columns represent "to" nodes for edges
	//row 0 and column 0 are unused throughout this program
	//leaves unused columns and rows unassigned as they will not be acessed
	int from, to, edge = 0;		//initializes from, to, and edge only for the purpose of initializing
	for(;;) {
		infile >> from;
		infile >> to;
		infile >> edge;
		if (from == 0)
			break;
		if (edge < 0)
			continue;
		C[from][to] = edge;
	}



}

//------------------------- insertEdge() ---------------------------------
// insertes an edge into the graph
// when displayed or the shortest paths functions are called, edge will be used
// Preconditions: passed edge must be valid
// Postconditions: the graphm will now contain the edge
bool GraphM::insertEdge(const int& edge, const int& to, const int& from)
{
	//if either of the passed representations of nodes do not exist
	//or if the passed edge is invalied (less than 0)
	//then return false
	if (to <= 0 || to > size || from <= 0 || from > size || edge < 0) {
		return false;
	}
	else {
		C[from][to] = edge;		//assign the edge value to the passed value
		modified = true;
		return true;
	}

}

//------------------------- removeEdge() ---------------------------------
// removes the passed edge from the graphm
// when displayed or the shortest paths functions are called, edge will not be used
// Preconditions: passed edge must be valid
// Postconditions: the passed edge will be removed from the graph
bool GraphM::removeEdge(const int& from, const int& to)
{
	//if either of the passed representations of nodes do not exist
	//or if the edge between the nodes already does not exist
	//then return false
	if (to <= 0 || to > size || from <= 0 || from > size) {
		return false;
	}
	else {
		C[from][to] = numeric_limits<int>::max();		//assign the edge to default nonexistent edge value
		modified = true;
		return true;
	}
}

//------------------------- findNext() ---------------------------------
// findShortestPath helper function for finding the next vertex in the shortest path 
// Preconditions: None
// Postconditions: the next vertex representation will be returned
int GraphM::findNext(const int& source )
{
	//keeps track of minimum distance at this point and the vertice with the minimum distance
	int next = 0;		//next vertice is initialized to zero, so that if all possible verticies have been visited the return value will be 0 (an indicator to break loop for finding more shortest paths for this source, as all possible paths exhausted)
	int min = numeric_limits<int>::max();		//initialized as Tabletype default value for skiping over verticies which do not have a distance at this point
	
	//for each possible next vertex from starting at this source
	for (int i = 1; i <= size; i++) {
		//
		if (T[source][i].visited == false && T[source][i].dist < min) {
			min = T[source][i].dist;		//
			next = i;		//sets next to the index of the vertex with the next minimum distance
		}
	}

	return next;
	
}

//------------------------- findShortestPath() ---------------------------------
// finds all the shortest paths for the graph, from each vertex to each vertex
// Preconditions: None
// Postconditions: shortest paths will be stored in graphm T
void GraphM::findShortestPath()
{
	//if the graph has been modified, resets table T for finding the new shortest path
	if (modified == true) {
		for (int from = 1; from <= size; from++) {
			for (int to = 1; to <= size; to++) {
				T[from][to].dist = numeric_limits<int>::max();
				T[from][to].path = 0;
				T[from][to].visited = false;
			}
		}
		modified = false;
	}

	int v = 0;		//initialize for only purpose of initialization
	
	//for every single vertex use it as a starting source to all of its possible paths 
	for (int source = 1; source <= size; source++) {

		T[source][source].dist = 0;		//initialize the distance from the source to the source to zero so it can be the intial v in the following loop

		for (int i = 1; i <= size; i++) {		//until all  vertices have been visited (unless all paths exhausted). Each loop visits a vertex

			v = findNext(source);		//find the next vertice to visit, shortest distance and not visited at this point 
			if (v == 0)		//if there are no more paths to find from source then all paths exhausted for this source
				break;

			T[source][v].visited = true;	//visit the next shortest distance and not visited vertice

			//for each of v's neighbors change distances if distnace at v plus distance from v to neighbor is less than neighbors distance at this point
			for (int w = 1; w <= size; w++) {
				//if there is path between the source and vertex and the neighbor vertex has not already been visited
				if (C[v][w] != numeric_limits<int>::max() && T[source][w].visited == false) {
					//if the distance from the source to the neighbor plus the distance to the source so far is less than the distance of the neighbor so far 
					if (T[source][v].dist + C[v][w] < T[source][w].dist) {
						T[source][w].dist = T[source][v].dist + C[v][w];		//sets distance of neighbor to distance of v plus distance from v to neighbor
						T[source][w].path = v;		//sets the path of neighbor to v 
					}
				}
			}
		}
	}
}

//------------------------- displayAll() ---------------------------------
// displays the graphm and all of its shortests paths, from each node to each vertex
// Preconditions: None
// Postconditions: all shortest paths will be displayed as a table to output
void GraphM::displayAll()
{
	if (modified == true) {
		findShortestPath();
	}

	cout << left;		//formats output to left aligned
	cout << setw(25) << "Description" << setw(12) << "From node" << setw(10) << "To node" << setw(15) << "Dijstra's" << "Path" << endl;		//outputs the output table header 

	//for each vertex find all all possible paths to other vertices in the graph
	for (int from = 1; from <= size; from++) {

		cout << data[from] << endl;		//ouput the from vertex name

		//for each possible vertex other then itself
		for (int to = 1; to <= size; to++) {
			
			//if the possible vertex is other than itself
			if (to != from) {

				cout << setw(25) << "";		//empty space for description column
				cout << setw(12) << from << setw(10) << to;		//output the from and to vertices, even if there is no path

				//if there is a path, then output the distance and the path 
				if (T[from][to].dist != numeric_limits<int>::max()) {

					cout << setw(15) << T[from][to].dist;		//outputs the distance of path

					outputPath(from, to);		//recursivly get to starting point (from) path and tail output path after
					cout << to;		//fencepost the final path 

				}
				else {
					cout << "-----";
				}
				cout << endl;		//time for next to vertex from source
			}
		}
	}

	cout << endl;
}

//------------------------- display() ---------------------------------
// displays a path from passed vertex to other passed vertex
// Preconditions: passed vertices must be valid
// Postconditions: displays a shortest path to output from passed vertices
void GraphM::display(const int& from, const int& to)
{

	//if the graph has 
	if (modified == true) {
		findShortestPath();
	}
	cout << left;		//formats output to left aligned
	cout << setw(8) << from << setw(8) << to;		//outputs the passed from and to vertices
	
	//if there is path between the two vertices
	if (T[from][to].dist != numeric_limits<int>::max()) {

		cout << setw(12) << T[from][to].dist;		//output the distance	
		outputPath(from, to);		//recursivly get to starting point (from) path and tail output path after
		cout << to;		//fencepost the final path 
		cout << endl;

		outputPathNames(from, to);		//recursivly get to starting point (from) path and tail output path name after
		cout << data[to] << endl;

	}
	else {
		cout << "----" << endl;
	}

	cout << endl;

}

//------------------------- outputPath() ---------------------------------
// recursive helper function for displaying paths from a vertex to another
// Preconditions: None
// Postconditions: path from passed vertex to other passed vertex will be displayed
void GraphM::outputPath(const int & from, const int & to)
{
	//recursivly get to starting point path (from) and tail output each path after
	if (from != to) {
		outputPath(from, T[from][to].path);
		cout << setw(4) << T[from][to].path;
	}
}

//------------------------- outputPathNames() ---------------------------------
// recursive helper function for displaying path names from a vertex to another
// Preconditions: None
// Postconditions: vertex names for shortest path from one vertex to another are displayed
void GraphM::outputPathNames(const int & from, const int & to)
{
	//recursivly get to starting point path (from) and tail output each path name after
	if (from != to) {
		outputPathNames(from, T[from][to].path);
		cout << data[T[from][to].path] << endl;;
	}

}



