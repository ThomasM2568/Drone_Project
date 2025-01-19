#include "canvas.h"
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include "myPolygon.h"
#include <QThread>
#include "server.h"
#include "drone.h"
#include <QDir>

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Canvas related methods
 **/

// Constructor to initialize the canvas
Canvas::Canvas(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true); // Enable mouse tracking for continuous mouse events

    // Load all the drone images and resize them
    droneImgRed.load("../../droneRed.png");
    droneImgBlue.load("../../droneBlue.png");
    droneImgYellow.load("../../droneYellow.png");
    droneImgGreen.load("../../droneGreen.png");
    float droneScaleRatio = 0.2;

    droneImgRed = droneImgRed.scaled(droneImgRed.width() * droneScaleRatio, droneImgRed.height() * droneScaleRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    droneImgBlue = droneImgBlue.scaled(droneImgBlue.width() * droneScaleRatio, droneImgBlue.height() * droneScaleRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    droneImgYellow = droneImgYellow.scaled(droneImgYellow.width() * droneScaleRatio, droneImgYellow.height() * droneScaleRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    droneImgGreen = droneImgGreen.scaled(droneImgGreen.width() * droneScaleRatio, droneImgGreen.height() * droneScaleRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage droneImg;

    // Load the server image and resize it
    float serverScaleRatio = 0.15;
    serverImg.load("../../server.png");
    serverImg = serverImg.scaled(serverImg.width() * serverScaleRatio, serverImg.height() * serverScaleRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

// Destructor to clean up resources
Canvas::~Canvas() {
    clear();
    for (Triangle* tri : triangles) {
        delete tri;  // Free memory for each Triangle
    }
    for (MyPolygon* pol : polygons) {
        delete pol;  // Free memory for each MyPolygon
    }
}

// Clear the vectors holding triangles, vertices, and polygons
void Canvas::clear() {
    triangles.clear();
    vertices.clear();
    polygons.clear();
}

// Add points to the canvas, duplicate the points to ensure they are permanent
void Canvas::addPoints(const QVector<Vector2D> &tab) {
    for (auto &pt:tab) {
        vertices.push_back(Vector2D(pt)); // Add point to vertices
    }
    reScale(); // Recalculate scaling
    update(); // Trigger a repaint of the canvas
}

// Add a triangle with vertices identified by their indices
void Canvas::addTriangle(int id0, int id1, int id2) {
    triangles.push_back(new Triangle(&vertices[id0],&vertices[id1],&vertices[id2]));
}

// Add a triangle with vertices and a specified color
void Canvas::addTriangle(int id0, int id1, int id2,const QColor &color) {
    triangles.push_back(new Triangle(&vertices[id0],&vertices[id1],&vertices[id2],color));
}

// Paint event to handle custom drawing on the canvas
void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0,0,width(),height(),whiteBrush); // Fill the background with white

    // Draw the axes
    QPointF points[7]={{0,-2},{80,-2},{80,-10},{100,0},{80,10},{80,2},{0,2}};
    painter.save();
    painter.translate(20,height()-20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawPolygon(points,7);
    painter.save();
    painter.setBrush(Qt::green);
    painter.rotate(-90);
    painter.drawPolygon(points,7);
    painter.restore();
    painter.restore();

    painter.save();
    painter.translate(10,height()-10);
    painter.scale(scale,-scale);
    painter.translate(-origin.x(),-origin.y());

    // Draw triangles
    if (showTriangles) {
        for (auto &tri:triangles) {
            tri->draw(painter); // Draw each triangle
        }
    }

    // Draw circles around highlighted triangles
    if (showCircles) {
        for (auto &tri:triangles) {
            if (tri->isHighlighted()) tri->drawCircle(painter);
        }
    }

    // Draw the center points of the triangles
    if (showCenters) {
        painter.setPen(QPen(Qt::black,3));
        for (auto &tri:triangles) {
            auto pt = tri->getCircleCenter();
            painter.drawLine(pt.x-15,pt.y-15,pt.x+15,pt.y+15);
            painter.drawLine(pt.x-15,pt.y+15,pt.x+15,pt.y-15);
        }
    }
    for (MyPolygon* polygon : polygons) {
        Vector2D center = polygon->calculateCentroid();
        painter.setPen(QPen(Qt::black,3));

        polygon->draw(painter,false); // Draw each polygon

        painter.setPen(QPen(Qt::white, 8));

        // Traverse the shared sides
        for (auto it = polygon->sharedSides.begin(); it != polygon->sharedSides.end(); ++it) {
            QPair<Vector2D, Vector2D> side = it.value();

            // If the flag is on, show the shared sides of the voronoi polygon
            if(showSharedSides){
                painter.setPen(QPen(Qt::red, 8));
                painter.drawLine(QPointF(side.first.x, side.first.y), QPointF(side.second.x, side.second.y));
            }
        }

    }

    // Show routes (white lines) using the created graph
    if(showRoutes){
        painter.setPen(QPen(Qt::white, 5));
        for (Node* node:graph.getNodes()){
            for (Node* edge:node->getEdges()){
                painter.drawLine(QPointF(node->getX(), node->getY()), QPointF(edge->getX(),edge->getY()));
            }
        }
    }
    int s=width()/100;
    QFont font("Times",s,QFont::Normal);
    painter.setFont(font);
    const QRect rect(-3*s,-2.5*s,3*s,2.5*s);

    // Show server routes (yellow lines) using the created network graph
    // servers have connexion with eachothers if they share a common side
    if(showServerRoutes){
        painter.setPen(QPen(Qt::yellow, 5));
        for (Node* node:networkGraph.getNodes()){
            for (Node* edge:node->getEdges()){
                painter.drawLine(QPointF(node->getX(), node->getY()), QPointF(edge->getX(),edge->getY()));
            }
        }
    }

    // Show servers if flag is on
    if(showServers){
        for (Vector2D s: servers) {
            painter.save();
            QFontMetrics metrics(painter.font());
            int textWidth = metrics.horizontalAdvance(s.name);
            int textHeight = metrics.height();

            // Rectangle centered around the text
            QRect srect(-textWidth / 2 - 5, -textHeight / 2 - 5, textWidth + 10, textHeight + 10);

            int imgWidth = serverImg.width();
            int imgHeight = serverImg.height();
            QRect imgRect(-imgWidth / 2 +5, -imgHeight / 2 +5, imgWidth+5, imgHeight+5);

            // Apply the transformation to position the object correctly
            painter.translate(s.x, s.y);
            painter.drawImage(imgRect, serverImg);

            // Draw the centered rectangle
            painter.setPen(Qt::NoPen); // No border for the rectangle
            painter.setBrush(QBrush(s.color));
            painter.drawRect(srect);

            // Determine the text color based on brightness
            QColor textColor;
            int red = s.color.red();
            int green = s.color.green();
            int blue = s.color.blue();
            double perceivedBrightness = 0.299 * red + 0.587 * green + 0.114 * blue;

            if (perceivedBrightness > 180 || (green > 180 && blue > 180)) {
                textColor = QColor(0, 0, 0); // Dark text for light background
            } else {
                textColor = QColor(255, 255, 255); // Light text for dark background
            }

            // Draw the centered text
            painter.save();
            painter.scale(1, -1); // Invert for the text
            painter.setPen(textColor);
            painter.drawText(srect, Qt::AlignCenter, s.name);
            painter.restore();

            painter.restore();
        }


    // If show servers flag is off, then show the polygon zones
    }else{
        painter.setPen(QPen(Qt::black));
        int i=0;
        for(Vector2D v: vertices){
            painter.save();
            painter.translate(v.x,v.y);
            painter.scale(1, -1);
            painter.fillRect(rect,QBrush(QColor(255,255,255,192)));
            painter.drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,QString::number(i++));
            painter.restore();
        }

    }

    if(voronoiDone){
        // Distance threshold to consider the drone as arrived
        const double threshold = 1.0;
        // Create a list to store each drone destination, avoid collisions
        QVector<Node *> hopList;

        for (myDrone* d : drones) {
            Vector2D position = d->getPosition();
            Vector2D goToPos = d->getGoToPosition();
            Vector2D goalPos = d->getGoalPosition();

            double distance = (d->getGoalPosition() - position).length();

            // If the drone has already requested its route (routeInitialized, no longer Yellow)
            if(d->routeInitialized && distance > threshold){
                // If the drone is not yet at its destination
                if(d->currentNodeIndex+1 < d->route.size()){
                    // Check if the next hop has been visited already
                    if(!hopList.contains(d->route[d->currentNodeIndex+1])){
                        // Mark the next hop as visited, set the drone as active, and change its color to green
                        hopList.push_back(d->route[d->currentNodeIndex+1]);
                        d->setIsActive(true);
                        QPen pen(Qt::green);
                        droneImg = droneImgGreen;
                        painter.setPen(pen);
                        qDebug() << "Drone: " + d->getName() + " active";
                    } else {
                        // The drone is waiting for the next step, set its color to blue
                        d->setIsActive(false);
                        QPen pen(Qt::blue);
                        droneImg = droneImgBlue;
                        painter.setPen(pen);
                        qDebug() << "Drone: " + d->getName() + " waiting";
                    }
                }
            } else if(distance < threshold){
                // The drone has arrived at its destination, set its color to red
                QPen pen(Qt::red);
                droneImg = droneImgRed;
                painter.setPen(pen);
                qDebug() << "Drone: " + d->getName() + " arrived, no longer active";
            } else {
                // The drone is still on its way to the first server, set its color to yellow
                QPen pen(Qt::yellow);
                droneImg = droneImgYellow;
                painter.setPen(pen);
                qDebug() << "Drone: " + d->getName() + " going to first server";
            }

            // Set the fill color to the drone's color (used for the drone's body)
            QBrush brush(d->getColor()); // Use the drone's color for filling
            painter.setBrush(brush);


            // Draw a circle on the drone using the provided color
            int radius = 7;
            int x = position.x - radius;
            int y = position.y - radius;
            int imageX = x - droneImg.width() / 2; // Center image horizontaly
            int imageY = y - droneImg.height() / 2; // Center image verticaly

            painter.drawImage(imageX, imageY, droneImg); // Draw the drone image
            painter.drawEllipse(x, y, 2 * radius, 2 * radius); // Draw the drone circle

            // If showing the next step, highlight the destination server in red
            if(!showServers && nextSteps){
                painter.setBrush(Qt::red);
                x = goalPos.x - radius;
                y = goalPos.y - radius;
                painter.drawEllipse(x, y, 2 * radius, 2 * radius);
            }

            // Calculate the distance between the current position and the target position
            distance = (goToPos - position).length();
            if (distance > threshold && d->isActive) {
                // If the distance is greater than the threshold, update the position

                // show the next hop in the route
                if (nextSteps){
                    painter.setBrush(Qt::green);
                    x = goToPos.x - radius;
                    y = goToPos.y - radius;
                    painter.drawEllipse(x, y, 2 * radius, 2 * radius);
                }

                // Update drone position
                if(droneMovement){
                    //d->updatePosition(0.016);
                    d->updatePosition(0.005);
                }
            } else {
                // arrived to first server
                // If the drone doesn't have an initialized route yet
                if (!d->routeInitialized){
                    Node* startNode = graph.getNode(goToPos.x, goToPos.y);
                    Node* endNode = graph.getNode(goalPos.x, goalPos.y);
                    server* srv = this->getServer(goToPos);

                    if(startNode && endNode){
                        // Initialize the drone's route
                        d->setRoute(&graph, &networkGraph, startNode, endNode, srv);
                        QVector<Node*> routes = d->route;

                        // Check if the current index is valid
                        if (d->currentNodeIndex + 1 < routes.size()){
                            // Move the drone to the next node
                            d->setGoToPosition(routes[d->currentNodeIndex + 1]);
                            d->currentNodeIndex++;
                        }
                    }
                } else if (d->isActive){
                    // If the route is already initialized, move forward in the route while the drone is active
                    QVector<Node*> routes = d->route;

                    // Check if the next node is still in the route
                    if (d->currentNodeIndex + 1 < routes.size()){
                        d->setGoToPosition(routes[d->currentNodeIndex + 1]);
                        d->currentNodeIndex++;
                    }

                }
            }
        }
        hopList.clear();
    }
    update();
}



// Get the bounding box of all the vertices
QPair<Vector2D,Vector2D> Canvas::getBox() {
    if (vertices.empty()) {Vector2D infLeft,supRight;
        return QPair<Vector2D,Vector2D>(Vector2D(0,0),Vector2D(200,200));
    }
    auto pts=vertices.begin();
    Vector2D infLeft(pts->x,pts->y),supRight(pts->x,pts->y);
    while (pts!=vertices.end()) {
        if (pts->x<infLeft.x) infLeft.x=pts->x;
        if (pts->y<infLeft.y) infLeft.y=pts->y;
        if (pts->x>supRight.x) supRight.x=pts->x;
        if (pts->y>supRight.y) supRight.y=pts->y;
        pts++;
    }
    return QPair<Vector2D,Vector2D>(infLeft,supRight);
}


// Handle window resizing and adjust scaling
void Canvas::resizeEvent(QResizeEvent *event) {
    reScale(); // Recalculate scale when window is resized
    update(); // Trigger a repaint
}

// Rescale the canvas based on its dimensions
void Canvas::reScale() {
    int newWidth = width()-20;
    int newHeight = height()-20;
    auto box=getBox();
    float dataWidth=box.second.x-box.first.x;
    float dataHeight=box.second.y-box.first.y;
    scale=qMin(float(newWidth)/float(dataWidth),float(newHeight)/float(dataHeight)  );
    origin.setX(box.first.x);
    origin.setY(box.first.y);
}

// Mouse move event to update the status bar with the mouse position
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    float mouseX=float(event->pos().x()-10)/scale+origin.x();
    float mouseY=-float(event->pos().y()-height()+10)/scale+origin.y();
    emit updateSB(QString("Mouse position= (") + QString::number(mouseX, 'f', 1) + "," + QString::number(mouseY, 'f', 1) + ")");

    // Update triangle highlight state based on mouse position
    for (auto &tri:triangles) {
        tri->setHighlighted(tri->isInside(mouseX,mouseY));
    }
    update(); // Trigger a repaint
}

// Mouse press event to flip triangles if needed
void Canvas::mousePressEvent(QMouseEvent * event){

    float mouseX=float(event->pos().x()-10)/scale+origin.x();
    float mouseY=-float(event->pos().y()-height()+10)/scale+origin.y();

    // If the triangle is inside the mouse click position and flippable, flip the triangle
    for (auto &tri:triangles) {

        if(tri->isInside(mouseX,mouseY) && tri->isFlippable()){

            tri->flippIt(triangles); // Flip the triangle
            checkDelaunay();  // Check if the triangles are in Delaunay condition
            qDebug() << "FlippIt ended";
            break;
        }
    }


}

// Flip all triangles to ensure they satisfy Delaunay condition
void Canvas::flippAll(){
    polygons.clear(); // Clear existing polygons
    while(!checkDelaunay()){
        auto it  = triangles.begin();
        while( it !=triangles.end() && !(*it)->isFlippable()){
            it++; // Move to the next triangle
        }
        if(it!=triangles.end()){
            (*it)->flippIt(triangles); // Flip the triangle

        }else{
            qDebug() << "issue";
        }
    }
}

// Load mesh data from a JSON file and add vertices and triangles to the canvas
void Canvas::loadMesh(const QString &title) {
    QFile file(title);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        clear(); // Clear existing canvas content
        QString JSON=file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONvertices = doc["vertices"].toArray();

        // Resize the vertices vector based on the size of JSONvertices
        vertices.resize(JSONvertices.size());
        qDebug() << "Vertices:" << JSONvertices.size();

        // Iterate through each vertex in the JSON array
        for (auto &&v : JSONvertices) {
            QJsonObject vector = v.toObject();
            qDebug() << vector["position"].toString() << "," << vector["id"].toInt();

            // Parse the position string and create a Vector2D object
            auto strPosition = vector["position"].toString().split(',');
            Vector2D pt(strPosition[0].toFloat(), strPosition[1].toFloat());

            // Get the vertex ID and assign the position to the corresponding index in the vertices vector
            auto intId = vector["id"].toInt();
            vertices[intId] = pt;
        }


        // Extract the "triangles" array from the JSON document
        QJsonArray JSONtriangles = doc["triangles"].toArray();
        qDebug() << "Triangles:" << JSONtriangles.size();

        // Iterate through each triangle in the JSON array
        for (auto &&v : JSONtriangles) {
            QJsonObject vector = v.toObject();
            qDebug() << vector["tri"].toString() << "," << vector["color"];

            // Parse the triangle vertices and color from the JSON object
            auto tri = vector["tri"].toString().split(',');
            auto color = vector["color"].toString();

            // If the triangle data is valid (has exactly 3 vertices), add it to the list
            if (tri.size() == 3) {
                addTriangle(tri[0].toInt(), tri[1].toInt(), tri[2].toInt(), QColor(color));
            }
        }

    }
    reScale();
    update();
}


// Load mesh data from a JSON file and add vertices and triangles to the canvas
void Canvas::loadServers(const QString &title) {
    qDebug("Chargement appelé");
    QFile file(title);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        clear(); // Clear existing canvas content
        QString JSON=file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONservers = doc["servers"].toArray();

        // Resize the vertices vector based on the size of JSONservers
        vertices.resize(JSONservers.size());
        qDebug() << "Servers:" << JSONservers.size();

        // Iterate through each server in the JSON array
        for (auto &&s : JSONservers) {
            QJsonObject srv = s.toObject();
            qDebug() << srv["position"].toString() << "," << srv["id"].toInt() << "," << srv["name"].toString();

            // Parse the position string and create a Vector2D object
            auto strPosition = srv["position"].toString().split(',');
            Vector2D pt(strPosition[0].toFloat(), strPosition[1].toFloat(), QString(srv["name"].toString()));

            // Set the server's color based on the JSON data
            pt.color = QColor(srv["color"].toString());

            // Add the server's position and color to the vertices list
            servers.push_back(pt);

            // Create a new server object and add it to the serversObj list
            server* serv = new server(strPosition[0].toFloat(), strPosition[1].toFloat(), pt.color, QString(srv["name"].toString()));
            serversObj.push_back(serv);
        }
    }
}

// Load mesh data from a JSON file and drones to the canva
void Canvas::loadDrones(const QString &title) {
    QFile file(title);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        clear(); // Delete existing content
        QString JSON = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONdrones = doc["drones"].toArray();

        // Log the number of drones to be processed
        qDebug() << "Drones:" << JSONdrones.size();
        int id = 0;

        // Iterate through each drone in the JSON array
        for (const QJsonValue &value : JSONdrones) {
            QJsonObject droneObj = value.toObject();

            // Extract drone attributes from JSON object
            QString name = droneObj["name"].toString();
            QColor color = QColor(droneObj["color"].toString());
            QString serverAddress = droneObj["server"].toString();

            // Parse the position string and create a Vector2D object for the drone's position
            auto positionStr = droneObj["position"].toString().split(',');
            Vector2D position(positionStr[0].toFloat(), positionStr[1].toFloat());

            // Dynamically allocate a new drone object
            myDrone* d = new myDrone(id, name, color, position, serverAddress);

            // Set the goal position and the target position
            d->setGoalPosition(servers);
            d->setGoToPosition(servers);

            // Add the drone object to the list
            drones.push_back(d); // Add the pointer to the drones list

            id++; // Increment the drone ID
        }

    }
}





// Find opposite points in triangles sharing common edges with the input triangle
QVector<const Vector2D*> Canvas::findOppositePointOfTrianglesWithEdgeCommon(const Triangle &tri){
    QVector<const Vector2D*> res;

    // Iterate over each triangle in the canvas
    for(auto t:triangles){
        // Check if the triangle shares an edge with the input triangle (tri)
        if(  tri.hasEdge(t->getVertexPtr(1),t->getVertexPtr(0)) ){
            res.push_back(t->getVertexPtr(2));  // Push opposite vertex
        }
        else if(tri.hasEdge(t->getVertexPtr(2),t->getVertexPtr(1))){

            res.push_back(t->getVertexPtr(0)); // Push opposite vertex
        }
        else if(tri.hasEdge(t->getVertexPtr(0),t->getVertexPtr(2)) ){
            res.push_back(t->getVertexPtr(1)); // Push opposite vertex
        };

    }
    return res; // Return list of opposite points
}

// Check Delaunay condition for all triangles
bool Canvas::checkDelaunay(){
    qDebug()<< "Delaunay process";
    bool areAllDelaunay = true;

     // Iterate through each triangle to check Delaunay condition
    for(auto &tri:triangles){
        bool res = tri->checkDelaunay(vertices); // Check Delaunay for current triangle

        // Check if the triangle is flippable
        if(!res){
            auto L=findOppositePointOfTrianglesWithEdgeCommon(*tri);
            //qDebug() << "L.size:" << L.size();

            // Iterate over the opposite points and check if any of them violates the circumcircle condition
            auto it=L.begin();
            while (it != L.end() && tri->circleContains(*it)) {
                ++it; // Move iterator to the next point
            }

            // If the iterator is valid, set opposite vertex for the triangle
            if (it != L.end()) {
                tri->setOpposite(const_cast<Vector2D*>(*it));
            }


            // Update the Delaunay status for the triangle
            tri->setDelaunay(false,it!=L.end());
        }
        /////////////////////////////////////////////////////////////////////////

        // Update the overall Delaunay status
        areAllDelaunay = areAllDelaunay && res;

    }
    update(); // Trigger canvas update
    qDebug()<<"areAllDelaunay:"<<areAllDelaunay;
    return areAllDelaunay;  // Return whether all triangles are Delaunay
}

// Find all triangles containing a given point M
QList<Triangle*> Canvas::findTrianglesContainingPoint(const Vector2D& M) {
    QList<Triangle*> resultList;

    // Iterate over all triangles to check if they contain point M
    for (Triangle* triangle : triangles) {
        if (triangle->contains(M)) { // Check if triangle contains M
            resultList.append(triangle);  // Add triangle to result list
        }
    }

    return resultList;  // Return list of triangles containing point M
}

// Sort triangles based on the angle of their circumcenter relative to a reference point
QVector<Triangle*> Canvas::sortTrianglesByLeft(const QVector<Triangle*>& triangles, const Vector2D& referencePoint) {
    QVector<Triangle*> sortedTriangles = triangles; // Copy original triangle list

    // Sort triangles by angle of their circumcenters relative to reference point
    std::sort(sortedTriangles.begin(), sortedTriangles.end(), [&referencePoint](Triangle* a, Triangle* b) {
        // Calculate circumcenters for both triangles
        Vector2D centerA = a->getCircleCenter(); // Get circumcenter of triangle A
        Vector2D centerB = b->getCircleCenter();  // Get circumcenter of triangle B

        // Calculate angles of circumcenters relative to the reference point
        double angleA = atan2(centerA.y - referencePoint.y, centerA.x - referencePoint.x);
        double angleB = atan2(centerB.y - referencePoint.y, centerB.x - referencePoint.x);

        // Compare angles and return sort order
        return angleA < angleB;
    });

    return sortedTriangles; // Return sorted list of triangles
}

QVector<Vector2D> Canvas::sortPointsByAngle(const QVector<Vector2D>& points, const Vector2D& referencePoint) {
    QVector<Vector2D> sortedPoints = points; // Create a copy of the points list to avoid modifying the original

    // Sort points based on their angle relative to the reference point
    std::sort(sortedPoints.begin(), sortedPoints.end(), [&referencePoint](const Vector2D& a, const Vector2D& b) {
        // Calculate the angle of each point relative to the reference point using the atan2 function
        double angleA = atan2(a.y - referencePoint.y, a.x - referencePoint.x); // Angle for point A
        double angleB = atan2(b.y - referencePoint.y, b.x - referencePoint.x); // Angle for point B

        // Compare the angles to sort the points in ascending order
        return angleA < angleB;
    });

    return sortedPoints; // Return the sorted list of points
}


// Create a polygon from sorted triangles by collecting their circumcenters
void Canvas::createPolygonFromTriangles(const QVector<Triangle*>& sortedTriangles){
    // Create new MyPolygon instance to store polygon vertices
    MyPolygon* polygon = new MyPolygon(sortedTriangles.size()+1);


    // Collect circumcenters from sorted triangles and add to polygon
    for (Triangle* triangle : sortedTriangles) {
        Vector2D center = triangle->getCircleCenter(); // Get circumcenter

        polygon->addVertex(center.x, center.y);   // Add circumcenter as a vertex of the polygon

         // Debug output for circumcenter coordinates
        qDebug()<<center.x;
        qDebug()<<center.y;
        qDebug()<<"";
    }

    // Add the created polygon to the canvas
    polygons.push_back(polygon);

    // Trigger canvas update to redraw
    update();
}


// Create a polygon from sorted triangles by collecting their circumcenters
void Canvas::createPolygon(const QVector<Vector2D> pointList){
    // Create new MyPolygon instance to store polygon vertices
    MyPolygon* polygon = new MyPolygon(pointList.size()+1);


    // Collect circumcenters from sorted triangles and add to polygon
    //qDebug()<<"Polygon with those points:";
    for (Vector2D point : pointList) {
        //Vector2D center = triangle->getCircleCenter(); // Get circumcenter

        polygon->addVertex(point.x, point.y);   // Add circumcenter as a vertex of the polygon

        // Debug output for circumcenter coordinates
        /*qDebug()<<point.x;
        qDebug()<<point.y;
        qDebug()<<"";*/
    }

    // Add the created polygon to the canvas
    polygons.push_back(polygon);

    // Trigger canvas update to redraw
    update();
}

// Create a polygon from sorted triangles by collecting their circumcenters
void Canvas::createPolygon(const QVector<Vector2D> pointList,QColor color){
    // Create new MyPolygon instance to store polygon vertices
    MyPolygon* polygon = new MyPolygon(pointList.size()+1);

    // Collect circumcenters from sorted triangles and add to polygon
    //qDebug()<<"Polygon with those points:";
    for (Vector2D point : pointList) {
        //Vector2D center = triangle->getCircleCenter(); // Get circumcenter

        polygon->addVertex(point.x, point.y);   // Add circumcenter as a vertex of the polygon

        // Debug output for circumcenter coordinates
        /*qDebug()<<point.x;
        qDebug()<<point.y;
        qDebug()<<"";*/
    }

    // Add the created polygon to the canvas
    polygon->setColor(color);
    //if(color==Qt::red)qDebug()<<"Color is red";
    polygons.push_back(polygon);
    // Trigger canvas update to redraw
    update();
}

server* Canvas::getServer(const Vector2D srv) {
    qDebug() << "Looking for" << srv;

    auto it = serversObj.begin();
    while (it != serversObj.end()) {
        server* s = *it;
        qDebug() << s->x << "," << s->y;
        if (s->x == srv.x && s->y == srv.y) {
            return s;
        }
        ++it;
    }
    return nullptr;
}



