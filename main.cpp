#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
#include <limits>

using namespace std;

struct edge {

	enum edgeStatus { UNEXPLORE, EXPLORE };
	int source, dst;
	string direction;
	edgeStatus status;

};

struct node {

	enum nodeStatus { WHITE, GRAY, BLACK };
	nodeStatus status;
	std::vector<edge> adj;

};

namespace maze {

	vector<edge> edges;
	map<int, node> graph;
	std::vector<int> path;
	int startNode = -1;
	int endNode = -1;

}

void commandLineErrCheck(int argc) {

	if(argc < 4) {
		std::cerr << "Usage: solveMaze input_file start_node end_node\n";
		exit(1);
	}

	return;

}

std::string chop(std::string& s,char a) {

	std::string final;
	int start = 0, end;

	for(int i = 0; i < s.size(); i++) {
		if(s[i] == a) {
			end = i;
			break;
		}
		if(i+1 == s.size()) {
			return s;
		}
	}

	final = s.substr(start, end);
	s = s.substr(end+1, s.size());

	return final;

}

vector<edge> getEdges(std::string fileName) {

	std::ifstream file;
	std::vector<edge> edges;
	edge currentEdge;
	string line;

	file.open(fileName.c_str());

	if(!file) {
		std::cerr << "Error: Cannot open " << fileName << std::endl;
		exit(1);
	}

	// Ignore the first line of the input file.
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while(!file.eof()) {
		getline(file, line);
		if(!line.empty()) {
			currentEdge.source = atoi(chop(line, ' ').c_str());
			currentEdge.dst = atoi(chop(line, ' ').c_str());
			currentEdge.direction = line;
			currentEdge.status = edge::UNEXPLORE;
			edges.push_back(currentEdge);
		}
	}

	file.close();

	return edges;

}

void createGraph() {

	vector<edge>::iterator currentEdge;
	node currentNode;
	edge modEdge;

	for(currentEdge = maze::edges.begin(); currentEdge != maze::edges.end(); ++currentEdge) {
		currentNode = maze::graph[currentEdge->source];
		currentNode.status = node::WHITE;
		currentNode.adj.push_back(*currentEdge);
		maze::graph[currentEdge->source] = currentNode;

		currentNode = maze::graph[currentEdge->dst];;
		currentNode.status = node::WHITE;
		modEdge.source = currentEdge->dst;
		modEdge.dst = currentEdge->source;
		modEdge.direction = currentEdge->direction;
		modEdge.status = edge::UNEXPLORE;
		currentNode.adj.push_back(modEdge);
		maze::graph[currentEdge->dst] = currentNode;
	}

	return;

}

void initialize() {

	maze::path.push_back(maze::startNode);
	maze::graph[maze::startNode].status = node::GRAY;

	return;

}

edge getNext(int currentNode) {

	edge nextEdge;
	nextEdge.dst = -1;
	vector<edge>::iterator adj;

	for(adj = (maze::graph[currentNode].adj).begin(); adj != (maze::graph[currentNode].adj).end(); adj++) {
		if(adj->status == edge::UNEXPLORE) {
			adj->status = edge::EXPLORE;
			nextEdge = *adj;
			break;
		}
	}

	// All adjacent node is discovered
	if (nextEdge.dst == -1) {
		maze::graph[currentNode].status = node::BLACK;
	}

	return nextEdge;

}

edge getNext(int currentNode, int previousNode, string direction) {

	edge nextEdge;
	nextEdge.dst = -1;
	vector<edge>::iterator adj;

	for(adj = (maze::graph[currentNode].adj).begin(); adj != (maze::graph[currentNode].adj).end(); adj++) {
		if((adj->direction.compare(direction)) == 0 && adj->dst != previousNode) {
			nextEdge = *adj;
			break;
		}
	}

	return nextEdge;

}

void findPath() {

	int currentNode, previousNode;
	edge currentEdge;

	// Starting node
	currentNode = maze::path.back();

	while(currentNode != maze::endNode) {

		// First node from starting position
		currentEdge = getNext(currentNode);
		previousNode = currentNode;
		currentNode = currentEdge.dst;

		// Check if current node has visited all the adj neighbor
		if (currentNode == -1) {
			maze::path.pop_back();
			currentNode = maze::path.back();
			continue;
		}

		// Second node from starting position
		currentEdge = getNext(currentNode, previousNode, currentEdge.direction);
		previousNode = currentNode;
		currentNode = currentEdge.dst;

		// Check if there is a next node in the same direction
		if(currentNode == -1) {
			currentNode = maze::path.back();
			continue;
		}

		// Final node
		currentEdge = getNext(currentNode, previousNode, currentEdge.direction);
		currentNode = currentEdge.dst;

		// Check if there is a next node in the same direction
		if(currentNode == -1) {
			currentNode = maze::path.back();
			continue;
		}

		// Check the current node if it has been visited
		if(maze::graph[currentNode].status == node::GRAY || maze::graph[currentNode].status == node::BLACK) {
			currentNode = maze::path.back();
			continue;
		}

		// Found the next node to add to path
		maze::graph[currentNode].status = node::GRAY;
		maze::path.push_back(currentNode);
	}

	return;

}

void displayPath() {

	cout << "Path: ";
	for(int i = 0; i < maze::path.size(); i++) {
		cout << maze::path[i] << ',' << ' ';
	}
	cout << endl;

	return;

}

int main(int argc, char* argv[]) {

	commandLineErrCheck(argc);
	maze::startNode = atoi(argv[2]);
	maze::endNode = atoi(argv[3]);

	maze::edges = getEdges(std::string(argv[1]));
	createGraph();
	initialize();
	findPath();
	displayPath();

	return 0;

};
