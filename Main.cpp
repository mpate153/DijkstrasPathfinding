#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include "adjacency-list.cpp"

using namespace std;

int main() {

    string fileName;
    cout << "Input filename.txt" << endl;
    cin >> fileName;

    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    AdjacencyList adj(file);

    double totalListMS = 0.0;

    while (true) {
        adj.printContents();

        int s;
        int t;
        cout << "Please enter source and destination vertices" << endl;
        if (!(cin >> s >> t)) {
            break;
        }
        if (s == -1 && t == -1) {
            break;
        }

        if (s < 0 || t < 0 || s > adj.numVertices() || t > adj.numVertices()) {
            cout << "Invalid vertices. Please use values from the list." << endl;
            continue;
        }

        vector<int> dist;
        vector<int> prev;

        double ms = adj.dijkstra(s, dist, prev);

        cout << "Query time for adjacency list: " << ms << endl; //Prints in milliseconds the time the algorithm took
        totalListMS += ms;
        cout << "Total time for all adjacency list queries: " << totalListMS << endl;

        //prints the shortest distance from s to each vertext
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i] == INT_MAX) { //if dist still equals INT_MAX, then no path to s was found
                cout << "Vertex " << i << " has no path from " << s << endl;
            }
            else {
                cout << "Vertex " << i << "'s shortest distance from " << s << " is " << dist[i] << endl;
            }
        }

        if (dist[t] != INT_MAX) { //will print the path to t if one was found
            int i = t;
            vector<int> path;
            path.push_back(i); //a vector will be made containing the shortest path from s to t
            do {
                i = prev[i];
                path.push_back(i);
            } while (i != s);

            //after making the vector, the shortest path is printed
            cout << "The shortest path from " << s << " to " << t << " is: ";
            for (int j = path.size() - 1; j >= 0; j--) {
                cout << path[j];
                if (j != 0) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
        else {
            cout << "A path from " << s << " to " << t << " could not be found" << endl;
        }
    }


    file.close();

	return 0;
}