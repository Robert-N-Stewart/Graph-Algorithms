// ------------------------------------------------ graphl.cpp -------------------------------------------------------

// Programmer Name Course Section Number: Robert Stewart CSS 343 A

// Creation Date: 05/10/17

// Date of Last Modification: 05/10/17

// --------------------------------------------------------------------------------------------------------------------

// Purpose - this program acts as a graph that can perfom depth first searches 

// -------------------------------------------------------------------------------------------------------------------- 

// --------------------------------------------------------------------------------------------------------------------

#include "graphl.h"

//------------------------- Default Constructor ---------------------------------
// creates an empty graphl
// Preconditions: None
// Postconditions: empty graphl will be constructed 
GraphL::GraphL()
{
	size = 0;		//sets the default size of the graph to zero

	//fills graph's array of vertices with empty graphnodes for future building
	for (int i = 0; i < MAXNODES; i++) {
		vertices[i] = GraphNode();
	}

}

//------------------------- Copy Constructor ---------------------------------
// creates a graphl from passed graphm
// graphm will be a deep copy of passed graphm
// Preconditions: None
// Postconditions: constructed graphl that is a deep copy of passed graph
GraphL::GraphL(const GraphL & passedGraphL)
{
	size = passedGraphL.size;		//sets implicit graph size to the passed graph size as they will have the same size after construction

	//for each graph vertex of the passed graph make deep copies of vertices and their EdgeNodes
	//empty graohnodes are also copied
	for (int i = 0; i < MAXNODES; i++)
	{
		vertices[i] = GraphNode();		//create a graphnode for every possible vertex
		
		//if the passed graph's graphnode has data
		if(passedGraphL.vertices[i].data != NULL)
			vertices[i].data = new NodeData(*passedGraphL.vertices[i].data);		//sets implicit graphnode's data pointer to point to a new NodeData that has the same data as the passed graphs parallel graphnode's data pointer

		const EdgeNode* passedCurrent = passedGraphL.vertices[i].edgeHead;		//parallel pointer to passed graph's parallel graphnode's edgenodes
		EdgeNode* implicitCurrent = vertices[i].edgeHead;		//parallel pointer to implicit graph's parallel graphnode's edgenodes

		//while the passed graph's parallel graphnode's edgenodes are not NULL set the implicit graph's parallel graphnode's edgenodes to deep copies
		while (passedCurrent != NULL) {
			implicitCurrent = new EdgeNode(passedCurrent->adjGraphNode);		//make a deep copy of the passed graph's parallel graphnode's parallel edgenode
			passedCurrent = passedCurrent->nextEdge;		//increment passed graph's edgepointer
			implicitCurrent = implicitCurrent->nextEdge;		//increment mplicit graphs edgepointer
		}

		implicitCurrent = NULL;		//now that the passed graph's parallel graphnode's edgenodes are exhausted set implicitCurrent at this point to NULL  
	}
}

//------------------------- destructor() ---------------------------------
// deallocates memory assigned to this graphl
// Preconditions: None
// Postconditions: all dynamically created memory for nodedata and edgenodes will be deleted
GraphL::~GraphL()
{
	//for everysingle graph node created 
	for (int i = 1; i <= size; i++) {
		deleteEdgeNodeList(vertices[i].edgeHead);		//recursicely deletes all edgenodes of this graphnode
		delete vertices[i].data;		//deletes this graphnode's data 
		vertices[i].data = NULL;		//sets the pointer to this graphnode's data to NULL for avoid dangling pointer
	}
}

//------------------------- buildGraph() ---------------------------------
// builds graph from ifstream of textfile
// Preconditions: text file of ifstream must be formatted in correctly
// Postconditions: the graphm will be constructed from text file
void GraphL::buildGraph(ifstream & infile)
{

	//if the implicit graph already has been built, then delete the built data 
	for (int i = 1; i <= size; i++) {
		deleteEdgeNodeList(vertices[i].edgeHead);		//recursicely deletes all edgenodes of this graphnode
		delete vertices[i].data;		//deletes this graphnode's data 
		vertices[i].data = NULL;		//sets the pointer to this graphnode's data to NULL for avoid dangling pointer
	}

	if (infile.eof())		//if the file is already exhausted, exit build
		return;

	infile >> size;		//stores first line at this point in the file as the graphs size representing the number of vertices for the graph

	if (size <= 0)		//if size of graph is zero, exit build
		return;

	string s;
	getline(infile, s);

	//for every the size of the graph to build, get each of the vertex names from the file
	//each name will be on its own sequential line
	for (int i = 1; i <= size; i++) {
		vertices[i].data = new NodeData();
		vertices[i].data->setData(infile);
	}

	
	int from, to = 0;		//initializes from, to, only for the purpose of initializing
	EdgeNode* edgeNodeToInsert;		//edgenode pointer for insertation
	//for every edgenode representation in the file, store edgenode into the correct graphnode's edgenode list 
	for (;;) {
		infile >> from;		//gets the vertex number of the from vertex for edge
		infile >> to;		//gets the vertex number of the to vertex for edge

		//indicator that at the end of this file's graph data, therefore break build
		if (from == 0)
			break;

		//assumes edge nodes representations stored in file are sequintialy from greatest to smallest
		edgeNodeToInsert = new EdgeNode(to);		//dynamically creates edgenode with the to vertex to insert in from graphnode's edge node adjacent list
		edgeNodeToInsert->nextEdge = vertices[from].edgeHead;		//points the created edgenode's next edgenode to the graphnode's first edgenode
		vertices[from].edgeHead = edgeNodeToInsert;		//points the graphnodes first edgenode to the created edgenode, so smallest to greates order of adjacent edgenodes is maintained
	}

}

//------------------------- displayGraph() ---------------------------------
// displays the graphl and all of its nodes and connections
// Preconditions: None
// Postconditions: displays the graphl and all of its nodes and connections
void GraphL::displayGraph() const
{
	cout << left;		//sets the format of output to left aligned for producing table output 
	cout << "Graph:" << endl;		//tabe title output
	
	//for every one of this graph's valid graph nodes
	for (int i = 1; i <= size; i++) {
		cout << "Node" << setw(10) << i << *vertices[i].data << endl;		//outputs the vertex number and vertex name
		const EdgeNode* current = vertices[i].edgeHead;		//edge node pointer assigned to the begining of the graphnode's adjacent edge nodes
		
		//while not at the end of this graph node's edge node adjacent list
		while (current != NULL) {
			cout << "  edge " << i << " " << current->adjGraphNode << endl;		//output the graphnode's vertex number and the edgenodes vertex number, representing the path
			current = current->nextEdge;		//go to the next edgenode in this graph nodes adjacent edgenode list
		}
	}
	cout << endl;
}

//------------------------- depthFirstSearch() ---------------------------------
// outputs the depthfirst search of this graph
// uses recursive helper functions
// Preconditions: None
// Postconditions: outputs the depthfirst search of this graph
void GraphL::depthFirstSearch()
{
	cout << "Depth-fisrt ordering:";
	//for every graphnode that is valid, visit if the graphnode has not already been visited and dfs through its adjacent edgenodes
	for (int i = 1; i <= size; i++) {
		if (vertices[i].visited == false)
			dfsHelper(i);
	}

	cout << endl << endl;

	//resets all of the valid graph nodes to not visited for future depth first searches
	for (int i = 1; i <= size; i++) {
		vertices[i].visited = false;
	}

}

//------------------------- deleteEdgeNodes() ---------------------------------
// recursive helper function for deleting all edgenodes for a graph node
// Preconditions: None
// Postconditions: all edgenodes will be deleted for used graphnode
void GraphL::deleteEdgeNodeList(EdgeNode *& current)
{
	//if not at the end of the graphnode's edgenodes
	if (current != NULL) {
		deleteEdgeNodeList(current->nextEdge);		//recursively get to the end of the edgelists for this graph node before deleting 
		delete current;		//deletes edge node folding back 
		current = NULL;		//sets dangiling pointer to NULL
	}
}

//------------------------- dfsHelper() ---------------------------------
// recursive helper function for displaying depthfirst search
// Preconditions: None
// Postconditions: depth first search will be outputed 
void GraphL::dfsHelper(const int& vertex)
{
	cout << "  " << vertex;		//graphnode has not been visited, therfore print out the graphnode number
	vertices[vertex].visited = true;		//and visit
	EdgeNode* current = vertices[vertex].edgeHead;		//sets a pointer to the begining of its adjacent edges
	
	//goes through all of its adjacent edges and recursively depth first searches through any graphnode that has not been visited
	while (current != NULL) {
		if (vertices[current->adjGraphNode].visited == false) {
			dfsHelper(current->adjGraphNode);		//if the edge node has not been visited, visit and recursively continue its depth first search
		}
		current = current->nextEdge;		//goes to the next edgenode for this graphnode
	}

}
