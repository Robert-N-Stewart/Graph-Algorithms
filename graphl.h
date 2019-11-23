#ifndef GRAPHL_H
#define GRAPHL_H

#include "nodedata.h"

#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

const int MAXNODES = 101;

class GraphL {

public:

	GraphL();
	GraphL(const GraphL& passedGraph);
	~GraphL();

	void buildGraph(ifstream& infile);		//  builds up graph node information and adjacency list of edges between each node reading from a data file
	void displayGraph() const;		// Displays each node information and edge in the graph
	void depthFirstSearch();		//Makes a depth-first search and displays each node in depth-first order

private:

	//
	struct EdgeNode {

		int adjGraphNode;  // subscript of the adjacent graph node

		EdgeNode* nextEdge;

		EdgeNode(int vertex) {
			adjGraphNode = vertex;
		}

	};

	//
	struct GraphNode {    // structs used for simplicity, use classes if desired

		EdgeNode* edgeHead; // head of the list of edges

		NodeData* data;     // data information about each node

		bool visited;

		//
		GraphNode() {
			edgeHead = NULL;
			data = NULL;
			visited = false;
		}

		
	};

	GraphNode vertices[MAXNODES];

	int size;

	void deleteEdgeNodeList(EdgeNode*& current);

	void dfsHelper(const int& vertex);


};

#endif
