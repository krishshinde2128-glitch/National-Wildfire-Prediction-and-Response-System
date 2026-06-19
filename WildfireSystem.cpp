#include <iostream>
#include <vector> 
#include <queue>  
#include <string>

using namespace std;

// 1. DATA STRUCTURES 
// struct used to store multiple data types together
struct SensorReading {
    int regionID;
    double temperature;
    double windSpeed;
};

struct Region {
    int id;
    string name;
    double riskScore;
};

struct IncidentRecord {
    int incidentID;
    string date;
    string regionName;
};

struct Road {
    int toRegion;
    int travelTime;
};

// MAIN SYSTEM CLASS
class WildfireSystem {
private:
    queue<SensorReading> sensorQueue;          
    vector<Region> activeRegions;              // vector = for dynamic array 
    vector<IncidentRecord> incidentDatabase;   
    vector<vector<int>> forestGraph;           
    vector<vector<Road>> roadGraph;         

public:
    // Runs automatically to set up exact map sizes and prevent memory crashes
    WildfireSystem(int numRegions) {
        forestGraph.resize(numRegions);
        roadGraph.resize(numRegions);
    }

    // component 1: SENSOR DATA PROCESSING (Queue) 
    void receiveSensorData(int id, double temp, double wind) {
        // Pushes the new alert to the very back of the line
        sensorQueue.push({id, temp, wind});
    }

    void processNextReading() {
        if (sensorQueue.empty()) return; // does nothing if line is empty
        
        // .front() reads the oldest data and .pop() deletes it so the line moves forward and then next arlet is been showed 
        SensorReading current = sensorQueue.front();
        sensorQueue.pop(); 
        
        cout << "Processed Sensor -> Region: " << current.regionID 
             << " | Temp: " << current.temperature << "C\n";
    }


    // component 2: WILDFIRE RISK RANKING (Merge Sort)
    void addRegion(int id, string name, double risk) {
        activeRegions.push_back({id, name, risk}); // Adds unsorted regions to back of list
    }

private:
    // mraging them together in sorted order
    void merge(vector<Region>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<Region> L(n1), R(n2); // Temporary left and right workspaces

        // Copy raw data into workspaces
        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

        int i = 0, j = 0, k = left;
        
        // (>=) ensures hiighest risk goes to the top
        while (i < n1 && j < n2) {
            if (L[i].riskScore >= R[j].riskScore) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        // Sweeps up any remaining values
        while (i < n1) { arr[k] = L[i]; i++; k++; }
        while (j < n2) { arr[k] = R[j]; j++; k++; }
    }
    // chopping them up into smaller pieces
    void mergeSort(vector<Region>& arr, int left, int right) {
        if (left >= right) return; //  Stop chopping if size is 1
        
        int mid = (right + left)/2;  // Find middle term
        
        mergeSort(arr, left, mid);      // Chop Left
        mergeSort(arr, mid + 1, right); // Chop Right
        merge(arr, left, mid, right);   // Stitch together
    }

public:
    void rankRegionsByRisk() {
        if (activeRegions.empty()) return;
        mergeSort(activeRegions, 0, activeRegions.size() - 1); // starts the sorter
        cout << "\n--- High Risk Regions (Ranked) ---\n";
        for (const auto& r : activeRegions) {
            cout << r.name << " (ID: " << r.id << ") - Risk Score: " << r.riskScore << "\n";
        }
    }

    // component 3: INCIDENT SEARCH (binary search)
    void addIncidentRecord(int id, string date, string name) {
        incidentDatabase.push_back({id, date, name});
    }
    void searchIncident(int targetID) {
        int start = 0, end = incidentDatabase.size() - 1;
        
        while (start <= end) {
            int mid = (start + end) / 2; 
            
            if (incidentDatabase[mid].incidentID == targetID) {
                cout << "\n--- Incident Found ---\n";
                cout << "ID: " << incidentDatabase[mid].incidentID 
                     << " | Date: " << incidentDatabase[mid].date 
                     << " | Location: " << incidentDatabase[mid].regionName << "\n";
                return;
            }
            if (incidentDatabase[mid].incidentID < targetID) start = mid + 1;
            else end = mid - 1;
        }
        cout << "\nIncident " << targetID << " not found.\n";
    }

    // =================================================================
    // component 4: FIRE SPREAD PREDICTION (Graph & BFS)
    // =================================================================

    void addForestAdjacency(int u, int v) {
        forestGraph[u].push_back(v);
        forestGraph[v].push_back(u); // Undirected graph so if u touches v, v touches u
    }

    void predictFireSpread(int startNode) {
        cout << "\n--- Fire Spread Prediction (BFS) ---\n";
        vector<bool> visited(forestGraph.size(), false); // false = not burning 
        queue<int> q; // queue created to know which forest is going to burn next
        
        visited[startNode] = true;//Lightning strikes the startNode so mark it true 
        q.push(startNode);// push the start node into the queue

        int level = 0;
        while (!q.empty()) {
            int size = q.size(); //
            cout << "Level " << level << " (Immediate Danger): ";
            
            // reade exactly one ring/level at a time
            for (int i = 0; i < size; i++) {
                int curr = q.front();
                q.pop();
                cout << curr << " ";
                
                // Check all adjacent neighbors of the current burning forest
                for (int neighbor : forestGraph[curr]) {
                    if (!visited[neighbor]) { // If neighbor isn't burning yet...
                        visited[neighbor] = true; // Light it on fire
                        q.push(neighbor);         // Add to queue for the next level
                    }
                }
            }
            cout << "\n";
            level++;
        }
    }

    // component 5: EMERGENCY ROUTE PLANNING (Weighted Graph & Dijkstra)
    void addRoad(int u, int v, int time) {
        roadGraph[u].push_back({v, time}); // Add road from u to v with travel time
        roadGraph[v].push_back({u, time});
    }

    // Finds absolute mathematical shortest path using driving times
    void findFastestRoute(int start, int destination) {
        cout << "\n--- Emergency Routing (Dijkstra) ---\n";
        
        // set to a billon  Because you haven't driven the roads yet
        vector<int> minTime(roadGraph.size(), 1e9);
        
        // Priority Queue: Automatically sorts fastest routes to the front
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        minTime[start] = 0; // Starting point takes 0 minutes
        pq.push({0, start});

        while (!pq.empty()) {
            int currTime = pq.top().first;
            int currNode = pq.top().second;
            pq.pop();

            if (currNode == destination) break; // Reached the fire, stop calculating!

            // Edge Relaxation: Check all roads connected to current node
            for (auto edge : roadGraph[currNode]) {
                int nextNode = edge.toRegion;
                int travelTime = edge.travelTime;

                // If driving this road beats our previous speed record...
                if (currTime + travelTime < minTime[nextNode]) {
                    minTime[nextNode] = currTime + travelTime; // Overwrite the record
                    pq.push({minTime[nextNode], nextNode});    // Push faster path to VIP line
                }
            }
        }

        cout << "Fastest time from Station " << start << " to Zone " << destination 
             << " is " << minTime[destination] << " minutes.\n";
    }
};


// MAIN FUNCTION
int main() {
    // 1. Boot up 
    WildfireSystem system(6);

    // 2. Queue 
    system.receiveSensorData(1, 45.2, 22.0);
    system.receiveSensorData(2, 38.1, 10.5);
    system.receiveSensorData(3, 50.0, 30.0);
    system.receiveSensorData(4, 42.5, 15.0);
    system.processNextReading();

    // 3. Merge Sort
    system.addRegion(0, "Kharghar Hills", 85.5);
    system.addRegion(1, "Nerul Valley", 42.0);
    system.addRegion(2, "Lonavala Woods", 91.2);
    system.addRegion(3, "Karnala Forest", 67.8);
    system.addRegion(4, "parallel Forest", 75.3);
    system.rankRegionsByRisk(); 

    // 5. BFS 
    system.addForestAdjacency(0, 1); // drawing the Edges between the Nodes
    system.addForestAdjacency(0, 2);
    system.addForestAdjacency(1, 3);
    system.addForestAdjacency(2, 4);
    system.addForestAdjacency(3, 5);
    system.predictFireSpread(0);

    // 6. Dijkstra  
    system.addRoad(0, 1, 10);
    system.addRoad(1, 3, 15);
    system.addRoad(0, 2, 5);
    system.addRoad(2, 3, 2);
    system.addRoad(1, 4, 20);
    system.findFastestRoute(0, 4);

    // 4. Binary Search
    system.addIncidentRecord(1001, "2024-03-12", "Kharghar Hills");
    system.addIncidentRecord(1002, "2025-01-05", "Nerul Valley");
    system.addIncidentRecord(1005, "2026-05-18", "Lonavala Woods");
    system.addIncidentRecord(1007, "2027-07-22", "Karnala Forest");
    system.addIncidentRecord(1010, "2028-11-30", "parallel Forest");

    int requestedID;
    cout << "\n[COMMAND CENTER] Enter the Incident ID you want to search for(example 1001): ";
    cin >> requestedID; 
    system.searchIncident(requestedID);



    return 0; 
}