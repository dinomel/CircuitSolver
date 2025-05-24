//
//  Graph.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include "model/GridComponent.h"
#include <vector>
#include <queue>
#include <unordered_set>

class Graph
{
    std::map<CoordinatePoint, int> _nodeCoordinateIndexMap;

public:
    int edgesCount;
    int nodesCount = 0;
    std::vector<std::vector<int>> adj;
    std::vector<std::vector<GridComponent *>> adjC; // dimenzija VxV => NIJE simetricna!

    Graph(int edgesCount) : edgesCount(edgesCount)
    {
        adjC.resize(edgesCount);
        for (int i = 0; i < edgesCount; i++)
        {
            adjC[i].resize(edgesCount);
        }
    }

    void addEdge(GridComponent *component)
    {
        CoordinatePoint startCoordinate = component->getStartCoordinate();
        if (_nodeCoordinateIndexMap.count(startCoordinate) == 0)
        {
            _nodeCoordinateIndexMap[startCoordinate] = static_cast<int>(_nodeCoordinateIndexMap.size());
            adj.resize(_nodeCoordinateIndexMap.size());
            nodesCount++;
        }
        int startNodeIndex = _nodeCoordinateIndexMap[startCoordinate];

        CoordinatePoint endCoordinate = component->getEndCoordinate();
        if (_nodeCoordinateIndexMap.count(endCoordinate) == 0)
        {
            _nodeCoordinateIndexMap[endCoordinate] = static_cast<int>(_nodeCoordinateIndexMap.size());
            adj.resize(_nodeCoordinateIndexMap.size());
            nodesCount++;
        }
        int endNodeIndex = _nodeCoordinateIndexMap[endCoordinate];

        adj[startNodeIndex].push_back(endNodeIndex);
        adj[endNodeIndex].push_back(startNodeIndex);
        adjC[startNodeIndex][endNodeIndex] = component;
    }

    std::vector<std::pair<int, int>> bfsMST(int start)
    {
        std::vector<std::pair<int, int>> mst;
        std::vector<bool> visited(nodesCount, false);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            for (int neighbor : adj[node])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    mst.emplace_back(node, neighbor);
                    q.push(neighbor);
                }
            }
        }

        return mst;
    }
};
