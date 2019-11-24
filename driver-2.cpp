// Purpose - This is a driver to test the graphl and graphm classes  

//---------------------------------------------------------------------------
// This code tests the basic functionality of the classes to perform 
// Dijkstra's algorihms and depth-first search for CSS 343 Lab 3. 
// It is not meant to exhaustively test the class. 
// // Assumptions: 
//   -- students can follow the directions to interface with this file 
//   -- text files "data31.txt" and "data32.txt" are formatted as described 
//   -- Data file data3uwb provides an additional data set for part 1; 
//      it must be edited, as it starts with a description how to use it 
//---------------------------------------------------------------------------

#include <iostream> 
#include <fstream> 

#include "graphl.h" 
#include "graphm.h" 

using namespace std;


int main() {

	// part 1 tests with the dataUWB.txt not data31
	ifstream infile1("dataUWB.txt");
	if (!infile1) {
		cout << "File could not be opened." << endl; 
		return 1;
	} 

	//for each graph, find the shortest path from every node to all other nodes
	for (;;) {
		
		GraphM G; 	//builds graph that should have default values
		G.buildGraph(infile1);		//graph should now have values after
		if (infile1.eof()) 
			break; 
		G.findShortestPath(); 
		G.displayAll();              // display shortest distance, path to cout
		G.removeEdge(4, 2);		//tests deleting two edges between vertices
		G.removeEdge(1, 12);
		G.insertEdge(1, 2, 3);		//tests inserting an edge
		G.removeEdge(0, 111111);		//tests removing a bad edge
		G.findShortestPath();		//finds the shortest path after removal
		G.displayAll();		//displays the graph after removal

	}

	// part 2 
	ifstream infile2("data32.txt"); 
	if (!infile2) {
		cout << "File could not be opened." << endl; return 1;
	}



	//for each graph, find the depth-first search ordering 
	for (;;) {
		GraphL G; 		//graph should be built with default values no dynamic mem
		G.buildGraph(infile2); 		//graph should now be built with text values
		if (infile2.eof()) 
			break; 
		G.displayGraph(); 		//display the graph
		G.depthFirstSearch();    // find and display depth-first ordering to cout 
	}
	
	cout << endl;

	return 0;
}
