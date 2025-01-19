#include "graph.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "node.h"
#include <QSet>
#include <cmath>
#include <QMap>
#include <QQueue>
#include <limits>
#include <QList>
#include <QStack>

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Graph related methods
 **/

Graph::Graph() {}


Graph::~Graph() {
    qDeleteAll(nodes);  // Clean up nodes to avoid memory leaks
}

void Graph::addNode(const QString& id, float x, float y) {
    int i = 0;
    bool found = false;

    // Using a while loop to check for the node
    while (i < nodes.size() && !found) {
        if (nodes[i]->getX() == x && nodes[i]->getY() == y) {
            found = true; // Mark as found
        } else {
            ++i; // Move to the next node
        }
    }

    if (found) {
       //qDebug()<< "Node with coordinates (" << x << ", " << y << ") already exists.\n";
    } else {
        //qDebug()<< "No such node found. Adding a new node.\n";
        Node* newNode = new Node(id, x, y); // Create a new node object
        nodes.append(newNode);  // Add the new node to the graph
    }

}

int Graph::addNode2(float x, float y) {
    int i = 0;
    int nodeID = 0;
    bool found = false;

    // Using a while loop to check for the node
    while (i < nodes.size() && !found) {
        if (nodes[i]->getX() == x && nodes[i]->getY() == y) {
            nodeID=nodes[i]->getId().toInt();
            found = true; // Mark as found
        } else {
            ++i; // Move to the next node
        }
    }

    if (found) {
        //qDebug()<< "Node with coordinates (" << x << ", " << y << ") already exists.\n";
        return nodeID;
    } else {
        //qDebug()<< "No such node found. Adding a new node.\n";
        Node* newNode = new Node(QString::number(nodes.size()), x, y); // Convertir nodes.size() en QString
        nodes.append(newNode);  // Add the new node to the graph
        return nodes.size();
    }

}

void Graph::addEdge(Node *node1, Node *node2){
   // qDebug()<<"Adding edge from"<<node1<<"to"<<node2;
    node1->addEdge(node2);node2->addEdge(node1);
}

void Graph::addEdge(int nodeID1, int nodeID2){
    Node* node1 = this->getNode(QString(QChar(nodeID1)));
    Node* node2 = this->getNode(QString(QChar(nodeID2)));
    if(node1!=nullptr && node2){
       // qDebug()<<"Adding edge from"<<node1<<"to"<<node2;
        node1->addEdge(node2);node2->addEdge(node1);
    }
}


Node* Graph::getNode(const QString& id) const {
    for (Node* node : nodes) {
        if (node->getId() == id) {
            return node;  // Return the node with the matching ID
        }
    }
    return nullptr;  // Return nullptr if the node is not found
}

Node* Graph::getNode(const int x, const int y) const {
    //qDebug()<<"Looking for"<<x<<","<<y;
    for (Node* node : nodes) {
        //qDebug()<<"Have "<<node->getX()<<","<<node->getY();
        if (node->getX() == x && node->getY() == y) {
            return node;  // Return the node with the matching ID
        }
    }
    return nullptr;  // Return nullptr if the node is not found
}

const QList<Node*>& Graph::getNodes() const {
    return nodes;  // Return the list of nodes in the graph
}

const int Graph::getNextId() const {
    return nodes.size();  // Return the list of nodes in the graph
}



void Graph::DFS(Node* startNode, QSet<Node*>& visited, QList<Node*>& cluster) {
    // Stack = List
    QStack<Node*> stack;
    stack.push(startNode);

    // While the stack is not empty
    while (!stack.isEmpty()) {
        // Remove the last element and put in currentNode
        Node* currentNode = stack.pop();

        // If the current visited node list does not contain the current node
        if (!visited.contains(currentNode)) {
            // Add the node to both visited and cluster
            visited.insert(currentNode);
            cluster.append(currentNode);

            // Add the neighbors to the stack
            for (Node* neighbor : currentNode->getEdges()) {
                if (!visited.contains(neighbor)) {
                    stack.push(neighbor);
                }
            }
        }
    }
}

QList<QList<Node*>> Graph::findClusters() {
    QStack<Node*> stack;
    QSet<Node*> visited;  // All visited nodes
    QList<QList<Node*>> clusters;  // List of found clusters

    // Step 1 : Start a explicit DFS to sort the nodes
    for (Node* node : nodes) {
        if (!visited.contains(node)) {
            QList<Node*> nodesToVisit;  // stack to simulate natural DFS
            nodesToVisit.append(node);

            while (!nodesToVisit.isEmpty()) {
                Node* currentNode = nodesToVisit.last();

                if (!visited.contains(currentNode)) {
                    visited.insert(currentNode);
                    bool allNeighborsVisited = true;

                    // Add all the non visited neighbors
                    for (Node* neighbor : currentNode->getEdges()) {
                        if (!visited.contains(neighbor)) {
                            nodesToVisit.append(neighbor);
                            allNeighborsVisited = false;
                        }
                    }

                    // If all the neighbors have already been visited, add to the stack
                    if (allNeighborsVisited) {
                        nodesToVisit.removeLast();
                        stack.push(currentNode);
                    }
                } else {
                    nodesToVisit.removeLast();
                }
            }
        }
    }

    // Step 2 : Use the stack to do a depth search on the graph with inverted edges
    visited.clear();  // Empty the visited node list

    // Browse each node in the stack
    while (!stack.isEmpty()) {
        Node* node = stack.pop();

        if (!visited.contains(node)) {
            QList<Node*> cluster;
            QList<Node*> nodesToVisit;
            nodesToVisit.append(node);

            while (!nodesToVisit.isEmpty()) {
                Node* currentNode = nodesToVisit.takeLast();

                if (!visited.contains(currentNode)) {
                    visited.insert(currentNode);
                    cluster.append(currentNode);

                    // Browse each node in order to find those who are pointing to currentNode
                    for (Node* otherNode : nodes) {
                        if (otherNode->getEdges().contains(currentNode) && !visited.contains(otherNode)) {
                            nodesToVisit.append(otherNode);
                        }
                    }
                }
            }

            clusters.append(cluster);
        }
    }

    return clusters;  // Return clusters list
}





QSet<Node*> Graph::findArticulationPoints() {
    QMap<Node*, bool> visited;
    QMap<Node*, int> discoveryTime;
    QMap<Node*, int> lowTime;
    QMap<Node*, Node*> parent;
    QSet<Node*> articulationPoints;
    int time = 0;

    // Initialize all data
    for (Node* node : nodes) {
        visited[node] = false;
        parent[node] = nullptr;  // No parent at first
    }

    // Apply DFS for each non visited nodes
    for (Node* node : nodes) {
        if (!visited[node]) {
            DFSArticulationPoints(node, visited, discoveryTime, lowTime, parent, articulationPoints, time);
        }
    }

    return articulationPoints;  // Return all articulation points
}

void Graph::DFSArticulationPoints(Node* node, QMap<Node*, bool>& visited, QMap<Node*, int>& discoveryTime,
                                  QMap<Node*, int>& lowTime, QMap<Node*, Node*>& parent, QSet<Node*>& articulationPoints, int& time) {
    visited[node] = true;
    discoveryTime[node] = lowTime[node] = ++time; // Initialize the discovery time
    int children = 0;  // Number of childs in the DFS tree

    for (Node* neighbor : node->getEdges()) {
        if (!visited[neighbor]) {
            // not already processed Neighbor
            parent[neighbor] = node;
            children++;

            DFSArticulationPoints(neighbor, visited, discoveryTime, lowTime, parent, articulationPoints, time);

            // Update the vale low of a node according to DFS result
            lowTime[node] = std::min(lowTime[node], lowTime[neighbor]);

            // Check if the node is an articulation point
            if (parent[node] == nullptr && children > 1) {
                articulationPoints.insert(node);  // Case for root
            }

            if (parent[node] != nullptr && lowTime[neighbor] >= discoveryTime[node]) {
                articulationPoints.insert(node);
            }

        } else if (neighbor != parent[node]) {
            // Update the low value if the already visited neighbor is found
            lowTime[node] = std::min(lowTime[node], discoveryTime[neighbor]);
        }
    }
}

void Graph::printAdjacencyMatrix() {
    int n = nodes.size();  // Number of nodes in the graph

    // Create a n * n matrice initialized at 0
    QVector<QVector<int>> adjacencyMatrix(n, QVector<int>(n, 0));

    // Create a map to associate index to each node
    QMap<Node*, int> nodeIndex;
    QMap<int, QString> indexToNode;
    for (int i = 0; i < n; ++i) {
        nodeIndex[nodes[i]] = i;
        indexToNode[i] = nodes[i]->getId();  // Store nodes name for printing
    }

    // Fill the adjacency matrice
    for (Node* node : nodes) {
        int i = nodeIndex[node];  // Get the node index in the node
        for (Node* neighbor : node->getEdges()) {
            int j = nodeIndex[neighbor];  // Get the neighbor index
            adjacencyMatrix[i][j] = 1;  // Connection from node to neighbor
        }
    }

    // Header
    QString header = "  ";  // Spacing between rows
    for (int i = 0; i < n; ++i) {
        header += indexToNode[i] + " ";  // Add the node name to the header
    }
    qDebug() << header;

    // Print the adjacency matrice
    for (int i = 0; i < n; ++i) {
        QString row = indexToNode[i] + " ";
        for (int j = 0; j < n; ++j) {
            row += QString::number(adjacencyMatrix[i][j]) + " ";
        }
        qDebug() << row;
    }
}


float Graph::dijkstra(Node* startNode, Node* endNode, QList<Node*>& path) {
    // Initialize the distances with infinity
    QMap<Node*, float> distances;
    QMap<Node*, Node*> previousNodes;
    for (Node* node : nodes) {
        distances[node] = std::numeric_limits<float>::infinity();
    }
    distances[startNode] = 0;  // The distance to starting node is 0

    QQueue<Node*> queue;
    queue.enqueue(startNode);

    // While there is still nodes to browse
    while (!queue.isEmpty()) {
        Node* currentNode = queue.dequeue();

        // If we reach the final node we can stop
        if (currentNode == endNode) {
            break;
        }

        // Browse all neighbors of the current Node
        for (Node* neighbor : currentNode->getEdges()) {
            // Calculate the distance to neighbor
            float distance = sqrt(pow(neighbor->getX() - currentNode->getX(), 2) +
                                  pow(neighbor->getY() - currentNode->getY(), 2));
            float transmissionTime = distance ; // Time based on transmission speed

            // If a shorter path is found
            if (distances[currentNode] + transmissionTime < distances[neighbor]) {
                distances[neighbor] = distances[currentNode] + transmissionTime;
                previousNodes[neighbor] = currentNode;
                queue.enqueue(neighbor);  // Add to the priority list
            }
        }
    }

    // Build the path by browsing previous nodes
    Node* currentNode = endNode;
    while (currentNode) {
        path.prepend(currentNode);
        currentNode = previousNodes.value(currentNode, nullptr);
    }

    // Return minimal transmission time
    return distances[endNode];
}
