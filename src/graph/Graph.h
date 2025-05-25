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
        edges.push_back(Edge(nodesCount - 1, startNodeIndex, endNodeIndex, component));
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

    std::vector<int> findCycle(std::vector<Edge> edges)
    {
        return {};
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
