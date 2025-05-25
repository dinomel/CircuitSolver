//
//  Graph.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include "../model/GridComponent.h"
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
    std::vector<std::vector<Component *>> adjC; // dimenzije (nodesCount x nodesCount) => NIJE simetricna!

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
        adjC[startNodeIndex][endNodeIndex] = component->getComponent();
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

    // bool dfs(int u, int par, std::vector<bool> &visited, std::vector<int> &parent, std::vector<int> &cycle)
    // {
    //     visited[u] = true;
    //     for (int v : adj[u])
    //     {
    //         if (!visited[v])
    //         {
    //             parent[v] = u;
    //             if (dfs(v, u, visited, parent, cycle))
    //                 return true;
    //         }
    //         else if (v != par)
    //         {
    //             // Found a back edge (cycle)
    //             cycle.push_back(v);
    //             int cur = u;
    //             while (cur != v)
    //             {
    //                 cycle.push_back(cur);
    //                 cur = parent[cur];
    //             }
    //             cycle.push_back(v); // close the cycle
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    // std::vector<int> findCycle()
    // {
    //     std::vector<bool> visited(nodesCount, false);
    //     std::vector<int> parent(nodesCount, -1);
    //     std::vector<int> cycle;
    //     for (int i = 0; i < nodesCount; ++i)
    //     {
    //         if (!visited[i])
    //         {
    //             if (dfs(i, -1, visited, parent, cycle))
    //                 break;
    //         }
    //     }

    //     return cycle;
    // }

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

    std::vector<int> dfsEdges(std::vector<Edge> edges, std::vector<int> prevPath, int nodeIndex)
    {
        std::vector<int> returnPath = std::vector<int>(0);
        std::vector<std::pair<Edge, int>> edgesAfter = findEdgesWithNodeIndex(edges, abs(prevPath[prevPath.size() - 1]), nodeIndex);

        for (int i = 0; i < edgesAfter.size(); i++)
        {
            std::vector<int> path = prevPath;
            path.push_back(edgesAfter[i].first.index * edgesAfter[i].second);

            if ((edgesAfter[i].second == 1 && edgesAfter[i].first.endNode == edges[0].startNode) || edgesAfter[i].first.startNode == edges[0].startNode)
                return path;

            returnPath = dfsEdges(edges, path, edgesAfter[i].second == 1 ? edgesAfter[i].first.endNode : edgesAfter[i].first.startNode);

            if (returnPath.size() != 0)
                return returnPath;
        }
        return returnPath;
    }

    // Vraca vektor edge indexa koji mogu biti negativni i ukoliko jeste, to znaci da je B[.][i] = -1
    std::vector<int> findCycle(std::vector<Edge> edges)
    {
        std::vector<int> cycle = dfsEdges(edges, {edges[0].index}, edges[0].endNode);

        return cycle;
    }

    // OVO DOLJE ZAKOMENTARISANO NISTA NE VALJA!! BOLJE GA NI NE GLEDATI NEGO ISPOCETKA! AL ZNACI STVAAARNO NE VALJA..

    // std::vector<Edge> findEdgesWithNodeIndex(std::vector<Edge> edges, int edgeIndex, int nodeIndex)
    // {
    //     std::vector<Edge> edgesWithNodeIndex = {};

    //     for (int i = 0; i < edges.size(); i++)
    //     {
    //         if (edges[i].index == edgeIndex)
    //             continue;
    //         if (edges[i].startNode == nodeIndex || edges[i].endNode == nodeIndex)
    //         {
    //             edgesWithNodeIndex.push_back(edges[i]);
    //         }
    //     }

    //     return edgesWithNodeIndex;
    // }

    // std::vector<int> rekurzivna(std::vector<Edge> edges, Edge edge, std::vector<int> prevPath)
    // {
    //     std::vector<Edge> adjacentEdges = findEdgesWithNodeIndex(edges, edge.index, edge.endNode);
    //     for(int i = 0; i < adjacentEdges.size(); i++)
    //     {
    //         std::vector<int> path = prevPath;
    //         path.push_back(adjacentEdges[i].index);
    //         if(adjacentEdges[i].startNode == edges[0].startNode || adjacentEdges[i].endNode == edges[0].startNode)
    //         {
    //             return path;
    //         }
    //         return rekurzivna(edges, adjacentEdges[i], path);
    //     }
    // }

    // std::vector<int> findCycle(std::vector<Edge> edges)
    // {

    //     std::vector<int> cycle = rekurzivna(edges, edges[0], {0});

    //     // for(int i = 0; i < edgesWithNodeIndex.size(); i++)
    //     // {
    //     //     Edge edge = edgesWithNodeIndex[0];

    //     //     std::vector<Edge> edgesWithNodeIndex = findEdgesWithNodeIndex(edges, edge.index, edge.endNode);

    //     // }

    //     return cycle;
    // }
};
