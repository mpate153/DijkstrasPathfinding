#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <climits>
#include <cmath>

using namespace std;

class AdjacencyList {
private:
    istream& file; // Store a reference to file in a general input stream
    vector<vector<pair<int, int>>> adj;
    /*
    The above vector is the adjacency list. The index value will correspond to a specific vertex.
    The vector at that index will store the list of edges for the corresponding vertex
    First value in the edge pair is the destination vertex. Second value is the edge weight
    */
public:
    AdjacencyList(istream& f) : file(f) {
        string word;
        vector<string> temp; //temporarily stores the string values of vectexes. The constructor will assign each of these vertexes an int value
        file >> word;
        if (word == "directed") { //code for reading files in the format of canada.txt
            while (file >> word) {
                if (word == "vertex") {
                    file >> word;
                    bool duplicate = false; //for loop will check for duplicate vertexes
                    for (int i = 0; i < temp.size(); i++) {
                        if (temp[i] == word) {
                            duplicate = true;
                            break; //ends loop early if duplicate is found
                            cout << word << " is a duplicate vertex" << endl;
                        }
                    }
                    //will only add the new vertex if its not a duplicate
                    if (!duplicate) {
                        temp.push_back(word);
                        adj.push_back(vector<pair<int, int>>());
                    }
                }
                else if (word == "edge") {
                    file >> word;
                    int i;
                    for (i = 0; i < temp.size(); i++) { //find the index of this vertex
                        if (temp[i] == word) {
                            break;
                        }
                    }
                    
                    if (i < temp.size()) {
                        file >> word;
                        int j; //find the index of the destination vertex
                        for (j = 0; j < temp.size(); j++) {
                            if (temp[j] == word) {
                                break;
                            }
                        }

                        if (j < temp.size()) {
                            file >> word;
                            int n = stoi(word); //converts edge weight to int
                            adj[i].push_back({j, n});
                        }
                        else {
                            cout << "Error: destination vertex " << word << " is not a listed vertex" << endl;
                            file >> word;
                        }
                    }
                    else {
                        cout << "Error: " << word << " is not a listed vertex" << endl;
                        file >> word; //reads the remaining values in the current line, but ignores contents
                        file >> word;
                    }
                }
            }
            
        }
        else { //code for reading files in the format of input-graph.txt
            int vertices = stoi(word); //checks the listed number of vertices and edges at the top of the file
            file >> word;
            int edges = stoi(word);
            
            vector<pair<int, int>> coords; //vector containing coordinates of each vertex
            int i = 0;
            while (file >> word) {
                if (i < vertices) { //checks if all vertices have been added
                    i++;
                    bool duplicate = false; //for loop will check for duplicate vertexes
                    for (int j = 0; j < temp.size(); j++) {
                        if (temp[j] == word) {
                            duplicate = true;
                            break; //ends loop early if duplicate is found
                            cout << word << " is a duplicate vertex" << endl;
                        }
                    }
                    //will only add the new vertex if its not a duplicate
                    if (!duplicate) {
                        temp.push_back(word);
                        adj.push_back(vector<pair<int, int>>());

                        //adds the coordinates of the vertex to coords
                        file >> word;
                        int x = stoi(word);
                        file >> word;
                        int y = stoi(word);
                        coords.push_back({ x, y });
                    }
                }
                else {
                    string u = word;
                    file >> word;
                    string v = word;
                    bool validEdge = true;
                    int j;
                    int k;
                    for (j = 0; j < temp.size(); j++) { //checks if u is a valid vertex, and saves its index
                        if (u == temp[j]) {
                            break;
                        }
                    }
                    if (j >= temp.size()) {
                        cout << "Error: " << u << " is not a listed vertex" << endl;
                        validEdge = false;
                    }
                    for (k = 0; k < temp.size(); k++) { //checks if v is a valid vertex, and saves its index
                        if (v == temp[k]) {
                            break;
                        }
                    }
                    if (k >= temp.size()) {
                        cout << "Error: " << v << " is not a listed vertex" << endl;
                        validEdge = false;
                    }

                    if (validEdge) { //adds the edge if both vertices are valid
                        int dx = coords[k].first - coords[j].first;
                        int dy = coords[k].second - coords[j].second;
                        int c = sqrt((dx * dx) + (dy * dy)); //Uses pythagorean theorem to determine edge length

                        adj[j].push_back({k, c});
                        adj[k].push_back({j, c});
                    }
                }
            }
        }
    }

    void printContents() {
        for (int i = 0; i < adj.size(); i++) {
            cout << "Vertex: " << i << endl;
            cout << "Edges: " << endl;
            for (int j = 0; j < adj[i].size(); j++) {
                cout << i << " to " << adj[i][j].first << "; weight of " << adj[i][j].second << endl << endl;
            }
        }
    }

    int vertexCount() {
        return adj.size();
    }
    
    double dijkstra(int s, vector<int>& dist, vector<int>& prev) {
        auto start = chrono::steady_clock::now(); //records start time

        for (int i = 0; i < adj.size(); i++) {
            dist.push_back(INT_MAX);
            prev.push_back(-1);
        }
        dist[s] = 0;

        vector<pair<int, int>> queue;

        for (int i = 0; i < adj[s].size(); i++) {
            //cout << adj[s][i].first << endl;
            if (dist[s] + adj[s][i].second < dist[adj[s][i].first]) { //check if distance from s is less than current distance
                dist[adj[s][i].first] = dist[s] + adj[s][i].second;
                prev[adj[s][i].first] = s;
                queue.push_back({adj[s][i].first, dist[adj[s][i].first]}); //adds vertex to queue
            }
        }

        while (queue.size() > 0) {
            int temp = 0;
            for (int i = 1; i < queue.size(); i++) { //find smallest distance value in priority queue; default is first value
                if (queue[i].second < queue[temp].second) {
                    temp = i;
                }
            }

            //remove vertex from queue
            int u = queue[temp].first;
            int d = queue[temp].second;
            queue.erase(queue.begin() + temp);

            if (d <= dist[u]) { //check that a smaller distance for u hasn't been found
                int v;
                int edge;
                for (int i = 0; i < adj[u].size(); i++) { //for each neighbour of u, check if u provides a shorter path
                    v = adj[u][i].first;
                    edge = adj[u][i].second;
                    if (dist[v] > dist[u] + edge) {
                        dist[v] = dist[u] + edge;
                        prev[v] = u;
                        queue.push_back({v, dist[v]});
                    }
                }
            }

        }

        auto end = chrono::steady_clock::now(); //records end time

        chrono::duration<double, milli> ms = end - start; //calculates the elapsed time in milliseconds and returns that value as a double
        return ms.count();
    }
};