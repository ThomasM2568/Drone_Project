#ifndef GRAPH_H
#define GRAPH_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Graph related methods
 **/


#include "node.h"
#include <QList>

class Graph {
public:
    Graph();
    ~Graph();

    /**
 * @brief Adds a node to the system with a specified ID and position.
 * @param id The unique identifier for the node.
 * @param x The x-coordinate of the node.
 * @param y The y-coordinate of the node.
 */
    void addNode(const QString& id, float x, float y);

    /**
 * @brief Adds a node to the system with a specified position and returns its index.
 * @param x The x-coordinate of the node.
 * @param y The y-coordinate of the node.
 * @return The index of the node.
 */
    int addNode2(float x, float y);

    /**
 * @brief Prints the adjacency matrix representing the graph's structure.
 */
    void printAdjacencyMatrix();

    /**
 * @brief Retrieves a node by its unique identifier.
 * @param id The unique identifier of the node.
 * @return A pointer to the node corresponding to the given ID.
 */
    Node* getNode(const QString& id) const;

    /**
 * @brief Retrieves all nodes in the graph.
 * @return A reference to the list of nodes in the graph.
 */
    const QList<Node*>& getNodes() const;

    /**
 * @brief Finds the articulation points in the graph.
 * @return A set of nodes that are articulation points in the graph.
 */
    QSet<Node*> findArticulationPoints();

    /**
 * @brief Finds clusters (connected components) in the graph.
 * @return A list of lists, each containing a set of connected nodes (a cluster).
 */
    QList<QList<Node*>> findClusters();

    /**
 * @brief Finds the shortest path between two nodes using Dijkstra's algorithm.
 * @param startNode The starting node for the path.
 * @param endNode The destination node for the path.
 * @param path A list that will hold the nodes along the shortest path.
 * @return The total cost of the shortest path.
 */
    float dijkstra(Node* startNode, Node* endNode, QList<Node*>& path);

    /**
 * @brief Adds an edge between two nodes in the graph.
 * @param node1 A pointer to the first node.
 * @param node2 A pointer to the second node.
 */
    void addEdge(Node *node1, Node *node2);

    /**
 * @brief Adds an edge between two nodes identified by their IDs.
 * @param nodeID1 The ID of the first node.
 * @param nodeID2 The ID of the second node.
 */
    void addEdge(int nodeID1, int nodeID2);

    /**
 * @brief Retrieves a node by its coordinates.
 * @param x The x-coordinate of the node.
 * @param y The y-coordinate of the node.
 * @return A pointer to the node at the specified coordinates.
 */
    Node* getNode(const int x, const int y) const;

    /**
 * @brief Retrieves the next available node ID.
 * @return The next node ID.
 */
    const int getNextId() const;


private:
    QList<Node*> nodes; ///< List of all nodes in the graph, stores all nodes that are part of the graph.

    /**
 * @brief Performs a depth-first search (DFS) on the graph starting from the given node.
 * @param node The starting node for the DFS traversal.
 * @param visited A set to track the visited nodes during the search.
 * @param cluster A list to store the nodes found in the current cluster (connected component).
 */
    void DFS(Node* node, QSet<Node*>& visited, QList<Node*>& cluster);

    /**
 * @brief Performs a depth-first search (DFS) to find articulation points in the graph.
 * @param node The starting node for the DFS traversal.
 * @param visited A map to track whether each node has been visited during the search.
 * @param discoveryTime A map to store the discovery time for each node during DFS.
 * @param lowTime A map to store the low time of each node during DFS.
 * @param parent A map to store the parent node for each node during DFS traversal.
 * @param articulationPoints A set to store the articulation points found during DFS.
 * @param time The current time used to assign discovery times to nodes.
 */
    void DFSArticulationPoints(Node* node, QMap<Node*, bool>& visited, QMap<Node*, int>& discoveryTime,
                               QMap<Node*, int>& lowTime, QMap<Node*, Node*>& parent, QSet<Node*>& articulationPoints, int& time);

};

#endif // GRAPH_H
