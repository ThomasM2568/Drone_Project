#ifndef SERVER_H
#define SERVER_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Server related methods
 **/

#include "vector2d.h"
#include "graph.h"
#include "node.h"

/**
 * @brief The server class represents a server in a 2D plane and its associated properties.
 *
 * The server class provides position, color, and name and some methods
 * to compute and find the shortest transmission paths and the next hop in a network graph.
 */
class server
{
public:
    float x,y; ///< The x and y components of the vector
    Vector2D* point; ///< Equivalent to server as Vector2D
    QColor color = QColor("#0eeb0e"); ///< Default color, changed later
    QString name=""; ///< Name of the server

    /**
     * @brief Constructs a Vector2D object with the given x and y components.
     * @param p_x The x component of the vector
     * @param p_y The y component of the vector
     */
    server(Vector2D* n_point);
    server(float _x,float _y, QColor _color, QString _name){
        x=_x;
        y=_y;
        color=_color;
        name=_name;
    }

    /**
 * @brief Finds the shortest transmission path between two nodes in the graph.
 * @param graph The graph containing the nodes.
 * @param startNode The node where the path starts.
 * @param endNode The node where the path ends.
 */
    void findShortestTransmissionPath(Graph& graph,  Node *startNode,  Node *endNode);

    /**
 * @brief Finds the next hop to the server in the transmission path.
 * @param graph The graph containing the nodes.
 * @param networkGraph The network graph used for routing.
 * @param startNode The node where the path starts.
 * @param endNode The node where the path ends.
 * @return A list of nodes representing the next hop towards the destination.
 */
    QList<Node*> findNextHopToServer(Graph* graph, Graph* networkGraph,  Node *startNode,  Node *endNode);


};

#endif // SERVER_H
