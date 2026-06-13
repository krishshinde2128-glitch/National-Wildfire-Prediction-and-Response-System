#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// =====================================================================
// DATA STRUCTURES (Blueprints for the system)
// =====================================================================

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

// =====================================================================
// MAIN SYSTEM CLASS (Contains all 5 core components)
// =====================================================================

class WildfireSystem {
private:
    // Storage containers for our system
    queue<SensorReading> sensorQueue;
    vector<Region> activeRegions;
    vector<IncidentRecord> incidentDatabase;
    vector<vector<int>> forestGraph;       
    vector<vector<Road>> roadGraph;        

public:
    // Constructor to set up the map sizes
    WildfireSystem(int numRegions) {
        forestGraph.resize(numRegions);
        roadGraph.resize(numRegions);
    }

    // =================================================================
    // COMPONENT 1: SENSOR DATA PROCESSING (Queue)
    // =================================================================

    void receiveSensorData(int id, double temp, double wind) {
        sensorQueue.push({id, temp, wind});
    }

    void processNextReading() {
        if (sensorQueue.empty()) return;
        SensorReading current = sensorQueue.front();
        sensorQueue.pop();
        cout << "Processed Sensor -> Region: " << current.regionID 
             << " | Temp: " << current.temperature << "C\n";
    }

    // =================================================================
    // COMPONENT 2: WILDFIRE RISK RANKING (Merge Sort)
    // =================================================================

    void addRegion(int id, string name, double risk) {
        activeRegions.push_back({id, name, risk});
    }

private:
    void merge(vector<Region>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<Region> L(n1), R(n2);

        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

        int i = 0, j = 0, k = left;
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
        while (i < n1) { arr[k] = L[i]; i++; k++; }
        while (j < n2) { arr[k] = R[j]; j++; k++; }
    }

    void mergeSort(vector<Region>& arr, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

public:
    void rankRegionsByRisk() {
        if (activeRegions.empty()) return;
        mergeSort(activeRegions, 0, activeRegions.size() - 1);
        cout << "\n--- High Risk Regions (Ranked) ---\n";
        for (const auto& r : activeRegions) {
            cout << r.name << " (ID: " << r.id << ") - Risk Score: " << r.riskScore << "\n";
        }
    }

    // =================================================================
    // COMPONENT 3: INCIDENT SEARCH (Binary Search)
    // =================================================================

    void addIncidentRecord(int id, string date, string name) {
        incidentDatabase.push_back({id, date, name});
    }

    void searchIncident(int targetID) {
        int left = 0, right = incidentDatabase.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (incidentDatabase[mid].incidentID == targetID) {
                cout << "\n--- Incident Found ---\n";
                cout << "ID: " << incidentDatabase[mid].incidentID 
                     << " | Date: " << incidentDatabase[mid].date 
                     << " | Location: " << incidentDatabase[mid].regionName << "\n";
                return;
            }
            if (incidentDatabase[mid].incidentID < targetID) left = mid + 1;
            else right = mid - 1;
        }
        cout << "\nIncident " << targetID << " not found.\n";
    }

    // =================================================================
    // COMPONENT 4: FIRE SPREAD PREDICTION (Graph & BFS)
    // =================================================================

    void addForestAdjacency(int u, int v) {
        forestGraph[u].push_back(v);
        forestGraph[v].push_back(u);
    }

    void predictFireSpread(int startNode) {
        cout << "\n--- Fire Spread Prediction (BFS) ---\n";
        vector<bool> visited(forestGraph.size(), false);
        queue<int> q;
        
        visited[startNode] = true;
        q.push(startNode);

        int level = 0;
        while (!q.empty()) {
            int size = q.size();
            cout << "Level " << level << " (Immediate Danger): ";
            for (int i = 0; i < size; i++) {
                int curr = q.front();
                q.pop();
                cout << curr << " ";
                
                for (int neighbor : forestGraph[curr]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            cout << "\n";
            level++;
        }
    }

    // =================================================================
    // COMPONENT 5: EMERGENCY ROUTE PLANNING (Dijkstra)
    // =================================================================

    void addRoad(int u, int v, int time) {
        roadGraph[u].push_back({v, time});
        roadGraph[v].push_back({u, time});
    }

    void findFastestRoute(int start, int destination) {
        cout << "\n--- Emergency Routing (Dijkstra) ---\n";
        vector<int> minTime(roadGraph.size(), 1e9);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        minTime[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int currTime = pq.top().first;
            int currNode = pq.top().second;
            pq.pop();

            if (currNode == destination) break;

            for (auto edge : roadGraph[currNode]) {
                int nextNode = edge.toRegion;
                int travelTime = edge.travelTime;

                if (currTime + travelTime < minTime[nextNode]) {
                    minTime[nextNode] = currTime + travelTime;
                    pq.push({minTime[nextNode], nextNode});
                }
            }
        }

        cout << "Fastest time from Station " << start << " to Zone " << destination 
             << " is " << minTime[destination] << " minutes.\n";
    }
};

// =====================================================================
// MAIN FUNCTION
// =====================================================================

int main() {
    // Initialize system with 6 regions (Nodes 0 to 5)
    WildfireSystem system(6);

    // 1. Queue Testing
    system.receiveSensorData(1, 45.2, 22.0);
    system.receiveSensorData(2, 38.1, 10.5);
    system.processNextReading();

    // 2. Merge Sort Testing
    system.addRegion(0, "Kharghar Hills", 85.5);
    system.addRegion(1, "Nerul Valley", 42.0);
    system.addRegion(2, "Lonavala Woods", 91.2);
    system.addRegion(3, "Karnala Forest", 67.8);
    system.rankRegionsByRisk();

    // 3. Binary Search Testing (IDs must be sorted)
    system.addIncidentRecord(1001, "2024-03-12", "Kharghar Hills");
    system.addIncidentRecord(1002, "2025-01-05", "Nerul Valley");
    system.addIncidentRecord(1005, "2026-05-18", "Lonavala Woods");
    system.searchIncident(1005);

    // 4. BFS Testing (Graph edges)
    system.addForestAdjacency(0, 1);
    system.addForestAdjacency(0, 2);
    system.addForestAdjacency(1, 3);
    system.addForestAdjacency(2, 4);
    system.predictFireSpread(0);

    // 5. Dijkstra Testing (Road network)
    system.addRoad(0, 1, 10);
    system.addRoad(1, 3, 15);
    system.addRoad(0, 2, 5);
    system.addRoad(2, 3, 2);
    system.findFastestRoute(0, 3);

    return 0;
}
