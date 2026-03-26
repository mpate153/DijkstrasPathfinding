#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include "adjacency-list.cpp"

using namespace std;


void adjList() {

    string fileName;
    cout << "Enter filename (.txt)" << endl;
    cin >> fileName;

    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    AdjacencyList adj(file);

    double totalListMS = 0.0;

    char showMat = 'n';
    cout << "Print the weighted adjacency table? (y/n): " << endl;
    cin >> showMat;
    if (showMat == 'y' || showMat == 'Y') {
        adj.printContents();
    }

    cout << "Enter source and destination (0 .. " << adj.vertexCount() - 1 << "), or -1 to quit." << endl;

    while (true) {

        int s;
        int t;
        cout << "Enter source and destination vertices" << endl;
        if (!(cin >> s >> t)) {
            break;
        }
        if (s == -1 && t == -1) {
            break;
        }

        if (s < 0 || t < 0 || s >= adj.vertexCount() || t >= adj.vertexCount()) {
            cout << "Invalid vertices. Please use values from the list." << endl;
            continue;
        }

        vector<int> dist;
        vector<int> prev;

        double ms = adj.dijkstra(s, dist, prev);

        if (dist[t] == INT_MAX) {
            cout << "No path from " << s << " to " << t << "." << endl;
        }
        else {
            cout << "Shortest distance: " << dist[t] << endl;

            int i = t;
            vector<int> path;
            path.push_back(i); //a vector will be made containing the shortest path from s to t
            do {
                i = prev[i];
                path.push_back(i);
            } while (i != s);

            //after making the vector, the shortest path is printed
            cout << "Path: ";
            for (int j = path.size() - 1; j >= 0; j--) {
                cout << path[j];
                if (j != 0) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }

        cout << "(adjacency list Dijkstra time this query: " << ms << " ms)\n\n"; //Prints in milliseconds the time the algorithm took

        totalListMS += ms;
    }

    cout << "\nTotal time in adjacency-list Dijkstra (all queries): " << totalListMS << " ms\n";


    file.close();
}

int main() {
    adjList();
    
    return 0;
}