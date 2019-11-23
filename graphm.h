#ifndef GRAPHM_H
#define GRAPHM_H

#include "nodedata.h"

#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

const int MAXNODESM = 101;

class GraphM {

public:

	GraphM();
	GraphM(const GraphM& passedGraph);

	void buildGraph(ifstream& infile);		// builds up graph node information and adjacency matrix of edges between each node reading from a data file.
	void findShortestPath();		//updates table T
	void displayAll();		//uses couts to demonstrate that the algorithm works properly. 
	void display(const int& from,const int& to);

	bool insertEdge(const int& edge, const int& nodeOne, const int& nodeTwo);		//insert an edge into graph between two given nodes
	bool removeEdge(const int& nodeOne, const int& nodeTwo);		//remove an edge between two given nodes

private:

	struct TableType {

		bool visited;          // whether node has been visited

		int dist;              // shortest distance from source known so far

		int path;              // previous node in path of min dist




		TableType(bool visited, int dist, int path) {
			this->visited = visited;
			this->dist = dist;
			this->path = path;
		}

		TableType() {
			this->visited = false;
			this->dist = numeric_limits<int>::max();
			this->path = 0;
		}

		TableType(const TableType& passedTableType) {
			this->visited = passedTableType.visited;
			this->dist = passedTableType.dist;
			this->path = passedTableType.path;
		}

	};

	NodeData data[MAXNODESM];              // data for graph nodes

	int C[MAXNODESM][MAXNODESM];            // Cost array, the adjacency matrix

	int size;                             // number of nodes in the graph
	
	bool modified;

	TableType T[MAXNODESM][MAXNODESM];      // stores visited, distance, path

	int findNext(const int& source);		//

	void outputPath(const int& from, const int& to);		//

	void outputPathNames(const int& from, const int& to);		//


};

#endif
