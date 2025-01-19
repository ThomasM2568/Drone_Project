#ifndef NODE_H
#define NODE_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Node related methods
 **/

#include <QString>
#include <QList>
#include "vector2d.h"

/**
 * @brief The Node class represents a node in a graph, with a unique ID, position,
 * and a list of neighboring nodes (edges).
 *
 * This class has methodes to manage and access node properties, including its
 * position, connected edges, and unique identifier. It also supports basic operations
 * such as adding edges and checking for connections to other nodes.
 */
class Node {
public:
    /**
    * @brief Constructor to initialize a node with an ID and position.
     * @param id The unique identifier for the node.
     * @param x The x-coordinate of the node.
     * @param y The y-coordinate of the node.
     */
    Node(const QString& id, float x, float y);

    /**
     * @brief Returns the unique ID of the node.
     * @return The ID of the node.
     */
    QString getId() const;

    /**
     * @brief Adds an edge between this node and a neighbor node.
     * @param neighbor A pointer to the neighboring node to add an edge with.
     */
    void addEdge(Node* neighbor);

    /**
     * @brief Returns the list of neighboring nodes (edges).
     * @return A constant reference to a list of neighboring nodes.
     */
    const QList<Node*>& getEdges() const;

    /**
     * @brief Returns the position of the node.
     * @return A pointer to the Vector2D representing the position of the node.
     */
    const Vector2D* getPosition();

    /**
     * @brief Returns the x-coordinate of the node.
     * @return The x-coordinate of the node.
     */
    float getX() const;

    /**
     * @brief Returns the y-coordinate of the node.
     * @return The y-coordinate of the node.
     */
    float getY() const;

    /**
     * @brief Checks if there is an edge between this node and another node.
     * @param other A pointer to the other node to check for an edge.
     * @return True if there is an edge, false otherwise.
     */
    bool hasEdgeTo(Node* other) const;
    QList<Node*> edges;           ///< List of connected nodes (edges)


private:
    QString id;                   ///< Unique identifier for the node

    float x;                      ///< X coordinate
    float y;                      ///< Y coordinate
    //float transmissionDistance;   ///< Transmission range
    //float transmissionSpeed;      ///< Transmission speed
};

#endif // NODE_H
