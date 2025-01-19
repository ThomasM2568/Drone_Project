#include "server.h"

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Server related methods
 **/

server::server(Vector2D* n_point):point(n_point){
    x=n_point->x;
    y=n_point->y;
    color=n_point->color;
    name=n_point->name;
};

void server::findShortestTransmissionPath(Graph& graph,  Node *startNode,  Node *endNode) {

    if (startNode && endNode) {
        QList<Node*> path;
        float minTime = graph.dijkstra(startNode, endNode, path);

        // Check if transmission time is infinity, which means there is no path
        if (minTime != std::numeric_limits<float>::infinity()) {
            /*qDebug() << "Minimal distance between" << startNode->getId() << "and" << endNode->getId() << "ist:" << minTime << "pixels";
            qDebug() << "Path:";*/
            /*for (Node* node : path) {
                qDebug() << "  " << node->getId();
            }*/
        } /*else {
            qDebug() << "There is no path between" << startNode->getId() << "and" << endNode->getId();
        }*/
    } else {
        qWarning() << "Nodes" << startNode->getId() << "or" << endNode->getId() << "do not exist in graph";
    }
    //qDebug() << " ";
}

QList<Node*> server::findNextHopToServer(Graph* graph,Graph* networkGraph,  Node *startNode,  Node *endNode) {
    //Function used for servers only
    // If end node is not a server, then we do nothing
    if(!networkGraph->getNode(endNode->getX(),endNode->getY())){
        //qDebug() << endNode->getId() << "is not a server";
        QList<Node*> newList;
        return newList;
    }

    if (startNode && endNode) {
        QList<Node*> path;
        QList<Node*> nextHopPath;
        float minTime = graph->dijkstra(startNode, endNode, path);

        // Check if transmission time is infinity, which means there is no path
        if (minTime != std::numeric_limits<float>::infinity()) {
            /*qDebug() << "Minimal distance between" << startNode->getId() << "and" << endNode->getId() << "is:" << minTime << "pixels";
            qDebug() << "Path:";*/
            //Instead of returning full path to end, we return only the route to next server
            //next wserver will compute the rest of the route to follow
            //Loop on all the nodes of the path
            int cpt=0;
            for (Node* node : path) {
                //qDebug() << "  " << node->getId();
                //Add each node to path
                nextHopPath.append(node);
                //If the node exists in networkGraph, it means that the node is a server,
                //so we can stop the loop and return the path
                if(networkGraph->getNode(node->getX(),node->getY())){
                    if(cpt==1)return nextHopPath;
                    cpt++;
                }

                //if(cpt==2)return nextHopPath;
                //else we just continue to loop until we reach the end or a server
            }
            return nextHopPath;
        } else {
            //qDebug() << "There is no path between" << startNode->getId() << "and" << endNode->getId();
            QList<Node*> newList;
            return newList;
        }
    } else {
        qWarning() << "Nodes" << startNode->getId() << "or" << endNode->getId() << "do not exist in graph";
        QList<Node*> newList;
        return newList;
    }
    //qDebug() << " ";
}
