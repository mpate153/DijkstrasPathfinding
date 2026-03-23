/*
 * Assignment 5: Graphs
 *
 * Mishaal Patel, Hariharan Vallath
 * Lakehead University
 * COMP-4433 Algorithm Design and Analysis
 * Dr. Saad Bin Ahmed
 * March 23, 2026
 */

#include "adjacency-matrix.cpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CityMap : public AdjacencyMatrixGraph {
public:
    void run();

private:
    bool loadGraphFile();
};

bool CityMap::loadGraphFile() {
    string fileName;
    cout << "Enter file name (.txt): ";
    if (!(cin >> fileName)) {
        return false;
    }

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Could not open file: " << fileName << '\n';
        return false;
    }

    string loadError;
    if (!loadFromStream(inputFile, loadError)) {
        cerr << "Could not read graph: " << loadError << '\n';
        return false;
    }

    return true;
}

void CityMap::run() {
    cout << fixed << setprecision(2);
    cout << "Shortest path (adjacency matrix)\n";

    if (!loadGraphFile()) {
        return;
    }

    printVertexList();

    char showMat = 'n';
    cout << "Print the weighted adjacency table? (y/n): ";
    cin >> showMat;
    if (showMat == 'y' || showMat == 'Y') {
        printAdjacencyMatrix();
    }

    double totalMatrixMs = 0.0;
    cout << "Enter source and destination (0 .. " << vertexCount() - 1 << "), or -1 to quit.\n";

    while (true) {
        int s, t;
        cout << "Enter source and destination vertices: ";
        if (!(cin >> s >> t)) {
            break;
        }
        if (s == -1 && t == -1) {
            break;
        }

        if (source < 0 || destination < 0 ||
            source >= vertexCount() || destination >= vertexCount()) {
            cout << "Invalid vertices. Please use values from the list.\n";
            continue;
        }

        vector<int> path;
        double dist = 0.0;
        double ms = dijkstra(s, t, path, dist);

        totalMatrixMs += ms;

        if (dist == kGraphInf) {
            cout << "No path from " << s << " to " << t << ".\n";
        } else {
            cout << "Shortest distance: " << dist << "\nPath: ";
            for (size_t i = 0; i < path.size(); ++i) {
                if (i) {
                    cout << " -> ";
                }
                cout << path[i];
            }
            cout << '\n';
        }
        cout << "(adjacency matrix Dijkstra time this query: " << ms << " ms)\n\n";
    }

    cout << "\nTotal time in adjacency-matrix Dijkstra (all queries): " << totalMatrixMs
         << " ms\n";
}

int main() {
    CityMap app;
    app.run();
    return 0;
}
