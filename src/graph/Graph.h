//
//  Graph.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include "Edge.h"
#include <vector>
#include <queue>
#include <unordered_set>

class Graph
{
    std::map<CoordinatePoint, int> _nodeCoordinateIndexMap;

public:
    int edgesCount;
    int nodesCount = 0;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;

private:
    Edge _getEdgeByNodes(int node1, int node2)
    {
        for (int i = 0; i < edgesCount; i++)
        {
            if ((node1 == edges[i].startNode && node2 == edges[i].endNode) || (node2 == edges[i].startNode && node1 == edges[i].endNode))
            {
                return edges[i];
            }
        }
    }

public:
    Graph(int edgesCount) : edgesCount(edgesCount) {}

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
        edges.push_back(Edge(static_cast<int>(edges.size()), startNodeIndex, endNodeIndex, component));
    }

    std::vector<Edge> bfsMST(int start)
    {
        std::vector<Edge> mst;
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
                    mst.emplace_back(_getEdgeByNodes(node, neighbor));
                    q.push(neighbor);
                }
            }
        }

        return mst;
    }

    std::vector<std::pair<Edge, int>> findEdgesWithNodeIndex(std::vector<Edge> edges, int edgeIndex, int nodeIndex)
    {
        std::vector<std::pair<Edge, int>> edgesWithNodeIndex = {};

        for (int i = 0; i < edges.size(); i++)
        {
            if (edges[i].index == edgeIndex)
                continue;
            if (edges[i].startNode == nodeIndex)
            {
                edgesWithNodeIndex.push_back(std::pair(edges[i], 1));
            }
            else if (edges[i].endNode == nodeIndex)
            {
                edgesWithNodeIndex.push_back(std::pair(edges[i], -1));
            }
        }

        return edgesWithNodeIndex;
    }

    std::vector<std::pair<int, int>> dfsEdges(std::vector<Edge> edges, std::vector<std::pair<int, int>> prevPath, int nodeIndex)
    {
        std::vector<std::pair<int, int>> returnPath = std::vector<std::pair<int, int>>(0);
        std::vector<std::pair<Edge, int>> edgesAfter = findEdgesWithNodeIndex(edges, prevPath[prevPath.size() - 1].first, nodeIndex);

        for (int i = 0; i < edgesAfter.size(); i++)
        {
            std::vector<std::pair<int, int>> path = prevPath;
            path.push_back(std::pair<int, int>(edgesAfter[i].first.index, edgesAfter[i].second));

            if ((edgesAfter[i].second == 1 && edgesAfter[i].first.endNode == edges[0].startNode) || edgesAfter[i].first.startNode == edges[0].startNode)
                return path;

            returnPath = dfsEdges(edges, path, edgesAfter[i].second == 1 ? edgesAfter[i].first.endNode : edgesAfter[i].first.startNode);

            if (returnPath.size() != 0)
                return returnPath;
        }
        return returnPath;
    }

    // Vraca vektor parova edge indexa i smjera grane (1 -> isti smjer, -1 -> suprotan)
    std::vector<std::pair<int, int>> findCycle(std::vector<Edge> edges)
    {
        return dfsEdges(edges, {std::pair<int, int>(edges[0].index, 1)}, edges[0].endNode);
    }
};
