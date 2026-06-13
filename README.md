# 🔥 Wildfire System

A comprehensive C++ application demonstrating advanced data structures and algorithms for wildfire prediction, risk assessment, and emergency response coordination.

## Overview

The Wildfire System is an integrated platform designed to monitor forest regions, predict fire spread, rank risk levels, and coordinate emergency evacuation routes. It combines multiple algorithmic approaches to create a practical emergency management system.

## Features

### 1. **Sensor Data Processing** (Queue)
- Receives and processes real-time temperature and wind speed readings from forest regions
- FIFO queue-based system for managing sensor inputs in order
- Tracks readings from multiple sensor stations across regions

### 2. **Wildfire Risk Ranking** (Merge Sort)
- Ranks forest regions by risk score in descending order
- Uses merge sort algorithm for efficient O(n log n) sorting
- Helps prioritize monitoring and prevention efforts

### 3. **Incident Search** (Binary Search)
- Quickly searches historical incident database by incident ID
- Maintains sorted incident records for fast retrieval
- O(log n) lookup time for incident information

### 4. **Fire Spread Prediction** (Graph & BFS)
- Predicts fire spread patterns using breadth-first search
- Models forest regions as a graph with adjacency connections
- Displays threat levels by distance from fire origin
- Helps identify endangered areas in real-time

### 5. **Emergency Route Planning** (Dijkstra's Algorithm)
- Calculates fastest evacuation routes between stations
- Optimizes travel time based on road network topology
- Finds shortest path from emergency stations to disaster zones
- Critical for coordinating evacuation efforts

## Data Structures Used

| Component | Data Structure | Purpose |
|-----------|----------------|---------|
| Sensor Management | Queue | FIFO processing of sensor readings |
| Risk Assessment | Vector | Storage & sorting of region data |
| Incident Tracking | Vector | Sorted database of incidents |
| Fire Spread | Graph (Adjacency List) | Forest connectivity |
| Route Planning | Weighted Graph | Road network with travel times |

## Algorithms Implemented

| Algorithm | Use Case | Time Complexity |
|-----------|----------|-----------------|
| Merge Sort | Risk ranking | O(n log n) |
| Binary Search | Incident lookup | O(log n) |
| BFS | Fire spread prediction | O(V + E) |
| Dijkstra's Algorithm | Optimal routing | O((V + E) log V) |

## Example Usage

```cpp
// Initialize system with 6 regions
WildfireSystem system(6);

// Add sensor readings
system.receiveSensorData(1, 45.2, 22.0);
system.processNextReading();

// Add and rank regions by risk
system.addRegion(0, "Kharghar Hills", 85.5);
system.rankRegionsByRisk();

// Search incident database
system.addIncidentRecord(1005, "2026-05-18", "Lonavala Woods");
system.searchIncident(1005);

// Predict fire spread
system.addForestAdjacency(0, 1);
system.predictFireSpread(0);

// Find emergency routes
system.addRoad(0, 1, 10);
system.findFastestRoute(0, 3);
```

## Compilation & Execution

### Compile:
```bash
g++ -std=c++11 WildfireSystem.cpp -o wildfire
```

### Run:
```bash
./wildfire
```

## Project Structure

```
WildfireSystem/
├── WildfireSystem.cpp    # Main implementation file
└── README.md             # Project documentation
```

## Sample Output

The program outputs information about:
- Processed sensor readings
- Ranked high-risk regions
- Found incident records
- Fire spread patterns (by threat level)
- Optimal evacuation routes with travel times

## Key Classes

### `WildfireSystem`
Main system class containing:
- `receiveSensorData()` - Add sensor reading to queue
- `processNextReading()` - Process next sensor data
- `rankRegionsByRisk()` - Sort regions by risk score
- `searchIncident()` - Binary search for incident
- `predictFireSpread()` - BFS fire propagation
- `findFastestRoute()` - Dijkstra's shortest path

## Real-World Applications

- 🏕️ Forest fire detection and management
- 🚨 Emergency response coordination
- 🛣️ Evacuation route optimization
- 📊 Wildfire risk assessment
- 🌲 Forest monitoring systems

## Learning Outcomes

This project demonstrates proficiency in:
- ✅ Queue data structure operations
- ✅ Efficient sorting algorithms (Merge Sort)
- ✅ Search optimization (Binary Search)
- ✅ Graph traversal (BFS)
- ✅ Shortest path algorithms (Dijkstra)
- ✅ Object-oriented C++ design

## Author

**Name**: Krish Shinde  
**Batch**: Larry Page  
**Course**: B.Tech CSE 


## License

Open for educational purposes
