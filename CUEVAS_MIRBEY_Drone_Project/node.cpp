#include "node.h"
#include "qdebug.h"

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Node related methods
 **/

/*
Node::Node(const QString& id, float x, float y, float transmissionDistance, float transmissionSpeed)
    : id(id), x(x), y(y), transmissionDistance(transmissionDistance), transmissionSpeed(transmissionSpeed) {}*/
Node::Node(const QString& id, float x, float y)
    : id(id), x(x), y(y) {}

QString Node::getId() const {
    return id;
}

void Node::addEdge(Node* neighbor) {
    edges.append(neighbor); // Add a connection to another node
}

const QList<Node*>& Node::getEdges() const {
    return edges; // Return the list of connected nodes
}
const Vector2D* Node::getPosition(){
    return new Vector2D(x,y);
}


// Getters for the new attributes
float Node::getX() const {
    return x;
}

float Node::getY() const {
    return y;
}

/*float Node::getTransmissionDistance() const {
    return transmissionDistance;
}

float Node::getTransmissionSpeed() const {
    return transmissionSpeed;
}*/
bool Node::hasEdgeTo(Node* other) const {
    return edges.contains(other);
}
