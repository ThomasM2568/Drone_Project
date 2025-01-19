#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtGlobal>
#include <node.h>
#include <graph.h>

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Main window logic, canvas updates, mesh loading, and Delaunay operations.
 **/

// Constructor for the MainWindow, sets up the UI and initial canvas state.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this); // Set up the UI elements.

    // Initialize checkboxes based on the canvas current display settings.
    ui->actionCircles->setChecked(ui->canvas->showCircles);
    ui->actionTriangles->setChecked(ui->canvas->showTriangles);
    ui->actionCenter->setChecked(ui->canvas->showCenters);

    // Load a default mesh file into the canvas when the window is created.
    ui->canvas->loadMesh("../../models/mesh2.json");

    // Load a default configuration file for drones and servers when the window is created.
    QString configFile = "path/to/your/configFile/config2.json";
    ui->canvas->loadServers(configFile);
    ui->canvas->loadDrones(configFile);

    // Connect canvas update signal to show messages in the status bar.
    QObject::connect(ui->canvas, SIGNAL(updateSB(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(ui->actionServer_routes, &QAction::triggered, this, &MainWindow::on_actionServer_routes_triggered);
    connect(ui->actionNext_steps, &QAction::triggered, this, &MainWindow::on_actionNext_steps_triggered);
    // void MainWindow::on_actionEnable_Disable_triggered(bool checked)
    ui->actionEnable_Disable->setCheckable(true);
    connect(ui->actionEnable_Disable, &QAction::triggered, this, &MainWindow::on_actionEnable_Disable_triggered);


}

// Destructor: Cleans up the UI when the main window is destroyed.
MainWindow::~MainWindow()
{
    delete ui;
}

// Slot to handle the creation of a new mesh when triggered.
void MainWindow::on_actionNew2_triggered() {
    ui->canvas->clear(); // Clear any existing content on the canvas.

    // Add a set of predefined points to the canvas.
    QVector<Vector2D> tab={ {100,100},{600,30},{200,400},{800,800},{1500,500}};
    ui->canvas->addPoints(tab);

    // Create a set of predefined triangles from the points.
    ui->canvas->addTriangle(0,1,2);
    ui->canvas->addTriangle(2,1,3);
    ui->canvas->addTriangle(1,4,3);

    // Update the status bar with the number of vertices and triangles.
    ui->statusBar->showMessage("Stats: |V|="+QString::number(ui->canvas->getSizeofV())+
                               "  |T|="+QString::number(ui->canvas->getSizeofT()));
}

// Slot to quit the application when triggered.
void MainWindow::on_actionQuit_triggered() {
    QApplication::quit(); // Exit the application.
}

// Slot to handle loading a mesh from a file.
void MainWindow::on_actionLoad_triggered() {
    // Open a file dialog to select a JSON file for the mesh.
    QString fileName = QFileDialog::getOpenFileName(this,"Load Polygon file",".","*.json");
    ui->canvas->loadMesh(fileName); // Load the selected mesh into the canvas.

    // Update the status bar with the number of vertices and triangles in the loaded mesh.
    ui->statusBar->showMessage("Stats: |V|="+QString::number(ui->canvas->getSizeofV())+
                               "  |T|="+QString::number(ui->canvas->getSizeofT()));
}

// Slot to toggle the visibility of circles on the canvas.
void MainWindow::on_actionCircles_triggered(bool checked) {
    ui->canvas->showCircles=checked; // Set the showCircles property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}

// Slot to toggle the visibility of triangles on the canvas.
void MainWindow::on_actionTriangles_triggered(bool checked) {
    ui->canvas->showTriangles=checked; // Set the showTriangles property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}

// Slot to toggle the visibility of centers (circumcenters) on the canvas.
void MainWindow::on_actionCenter_triggered(bool checked) {
    ui->canvas->showCenters=checked; // Set the showCenters property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}

// Slot to display an About message with information about the program.
void MainWindow::on_actionAbout_triggered() {
    //QMessageBox::information(this,"About Circumcircle","Initial program to compute the circumcenter of the loaded triangles.");
    QMessageBox::information(this, "About Drone Project",
                             "Graphical interface to simulate drone movement from their initial position to the destination server using Voronoi diagrams and graph algorithms. \n\n"
                             "All drones are positioned according to a configuration file. Their first step is to reach the nearest server to request a route to their destination. \n\n"
                             "Drone color code: \n"
                             "   - Yellow: When the drone hasn't reached its initial position. \n"
                             "   - Green: When the drone is moving towards the destination server. \n"
                             "   - Red: When the drone has arrived at the destination server. \n"
                             "   - Blue: When the drone is waiting (landed) to proceed to the next hop."
                             );
}

// Slot to check the Delaunay condition for the current mesh and triangles.
void MainWindow::on_actionCheck_Delaunay_triggered()
{
    ui->canvas->checkDelaunay(); // Perform the Delaunay check on the canvas.
}

// Slot to flip all triangles that do not satisfy the Delaunay condition.
void MainWindow::on_actionProcess_flip_triggered()
{
    ui->canvas->flippAll(); // Flip all triangles that are not Delaunay.
}


void MainWindow::on_actionVoronoi_triggered() {
    // Mark that Voronoi diagram is done and clear the existing polygons
    ui->canvas->voronoiDone = true;
    ui->canvas->polygons.clear();

    // Set screen dimensions for boundary calculations
    int screenWidth = 1500;
    int screenHeight = 1200;

    // Temporary list to store points for each polygon
    QVector<QVector<Vector2D>> tempList;

    // Loop through each vertex in the canvas
    for (Vector2D pt : ui->canvas->getVertices()) {
        // Set color for the vertex based on its x and y coordinates
        int r = (static_cast<int>(pt.x) % 256);  // Red value based on x
        int g = (static_cast<int>(pt.y) % 256);  // Green value based on y
        int b = (static_cast<int>(pt.x + pt.y) % 256);  // Blue value based on the sum of x and y
        pt.color = QColor(r, g, b);  // Assign color to the vertex

        // Find triangles that contain the current vertex
        QList<Triangle*> tr = ui->canvas->findTrianglesContainingPoint(pt);

        // Temporary lists to hold points and triangles for the current polygon
        QVector<Vector2D> pointList;
        QVector<Triangle*> triangleL;
        QVector<Vector2D> sortedPointList;

        // Iterate over each triangle containing the vertex
        for (Triangle* t : tr) {
            bool isOpen = false;
            triangleL.append(t);  // Add the triangle to the list

            // Check if the triangle is open
            t->checkIsOpen(ui->canvas->triangles);

            // If the triangle is open, find and process the open sides
            if (t->isOpen) {
                for (const auto& edge : t->openSides) {
                    // Get the two points of the open edge
                    Vector2D* firstPoint = edge.first;
                    Vector2D* secondPoint = edge.second;

                    // Check if the edge involves the current vertex
                    if (firstPoint == pt || secondPoint == pt) {

                        // Get the circumcenter (center of the circumcircle) of the triangle
                        Vector2D circumCenter = t->getCircleCenter();

                        // Vector calculations for finding the intersection with screen boundaries
                        Vector2D ABvector = Vector2D(secondPoint->x - firstPoint->x, secondPoint->y - firstPoint->y);
                        Vector2D APvector = Vector2D(circumCenter.x - firstPoint->x, circumCenter.y - firstPoint->y);
                        float dotProduct = ABvector.x * APvector.x + ABvector.y * APvector.y;
                        float ABLengthSquared = ABvector.x * ABvector.x + ABvector.y * ABvector.y;
                        float projection = dotProduct / ABLengthSquared;

                        // Projected point (perpendicular intersection)
                        float xQ = firstPoint->x + projection * (secondPoint->x - firstPoint->x);
                        float yQ = firstPoint->y + projection * (secondPoint->y - firstPoint->y);

                        // Directional vector (from circumcenter to the projected point)
                        Vector2D V = Vector2D(xQ - circumCenter.x, yQ - circumCenter.y);

                        // List to store possible intersection values with screen edges
                        std::vector<float> t_values;

                        // Calculate intersections with the screen's left and right edges
                        if (V.x != 0) {
                            float t = (-circumCenter.x) / V.x;  // Intersection with left edge (x = 0)
                            if (t > 0) t_values.push_back(t);
                        }
                        if (V.x != 0) {
                            float t = (screenWidth - circumCenter.x) / V.x;  // Intersection with right edge
                            if (t > 0) t_values.push_back(t);
                        }

                        // Handle intersections with top and bottom edges
                        if (V.y != 0) {
                            // Calculate intersection with top and bottom edges based on direction
                            if (V.y > 0 && V.y > 200) {
                                float t1 = (-screenHeight / 8 - circumCenter.y) / V.y;
                                if (t1 > 0) t_values.push_back(t1);
                            } else if (V.y > 0) {
                                float t1 = (-circumCenter.y) / V.y;
                                if (t1 > 0) t_values.push_back(t1);
                            } else if (V.y < 0) {
                                float t2 = (screenHeight - circumCenter.y) / V.y;
                                if (t2 > 0) t_values.push_back(t2);
                            }
                        }

                        // Find the minimal positive t value (intersection point)
                        if (!t_values.empty()) {
                            float t_min = *std::min_element(t_values.begin(), t_values.end());
                            // Calculate the intersection point with the screen edge
                            float xIntersect = circumCenter.x + t_min * V.x;
                            float yIntersect = circumCenter.y + t_min * V.y;

                            // Add the intersection point to the list
                            pointList.push_back(Vector2D(xIntersect, yIntersect));
                        }
                    }
                }
            }

            // Add the circumcenter to the list of points for the polygon
            pointList.push_back(t->getCircleCenter());

            // Sort the points in counter-clockwise order around the vertex
            sortedPointList = ui->canvas->sortPointsByAngle(pointList, pt);
        }

        // If there are enough points for a polygon, create it and add it to the canvas
        if (sortedPointList.size() > 2) {
            ui->canvas->createPolygon(sortedPointList, pt.color);
            ui->canvas->update();
        }
    }

    // After all polygons are created, associate shared sides between them
    for (MyPolygon *poly : ui->canvas->polygons) {
        poly->getSharedSides(ui->canvas->polygons);
    }

    // Finally, create a graph representation based on the Voronoi diagram
    createGraph();
}



void MainWindow::createGraph() {
    // Loop through all polygons in the canvas
    for (MyPolygon *poly : ui->canvas->polygons) {
        // Get shared sides of the current polygon
        poly->getSharedSides(ui->canvas->polygons);

        // Iterate through the shared sides of the polygon
        for (auto it = poly->sharedSides.begin(); it != poly->sharedSides.end(); ++it) {
            QPair<Vector2D, Vector2D> side = it.value();  // Get the shared side (two points)
            MyPolygon* adjacentPolygon = it.key();  // Get the adjacent polygon

            // Calculate the center of the shared side
            int middleX = (side.first.x + side.second.x) / 2;
            int middleY = (side.first.y + side.second.y) / 2;
            int middleID = ui->canvas->graph.addNode2(middleX, middleY);  // Add node for the shared side center
            Node* middleNode = ui->canvas->graph.getNode(QString::number(middleID));

            // Calculate the centroid of the current polygon
            Vector2D center = poly->calculateCentroid();
            int centerID = ui->canvas->graph.addNode2(center.x, center.y);  // Add node for the center of the polygon
            Node* centerNode = ui->canvas->graph.getNode(QString::number(centerID));

            // Calculate the centroid of the adjacent polygon
            Vector2D center2 = adjacentPolygon->calculateCentroid();
            int center2ID = ui->canvas->graph.addNode2(center2.x, center2.y);  // Add node for the center of the adjacent polygon
            Node* center2Node = ui->canvas->graph.getNode(QString::number(center2ID));

            // Add edges between the middle point and the centroids if nodes exist
            if (middleNode && centerNode && center2Node) {
                ui->canvas->graph.addEdge(middleNode, centerNode);  // Add edge between middle point and current polygon centroid
                ui->canvas->graph.addEdge(middleNode, center2Node);  // Add edge between middle point and adjacent polygon centroid
            }

            // Create a list to hold servers inside the current polygon
            QVector<Vector2D> currentServers;
            for (Vector2D server : ui->canvas->servers) {
                int srvID = ui->canvas->graph.addNode2(server.x, server.y);  // Add node for the server
                Node* srvNode = ui->canvas->graph.getNode(QString::number(srvID));

                // If the server is inside the current polygon, add it to the list
                if (poly->isPointInside(&server)) {
                    currentServers.append(server);
                    if (centerNode && srvNode) {
                        ui->canvas->graph.addEdge(centerNode, srvNode);  // Add edge between the centroid and server
                    }
                }
            }

            // Create a list to hold servers inside the adjacent polygon
            QVector<Vector2D> adjacentServers;
            for (Vector2D server : ui->canvas->servers) {
                // If the server is inside the adjacent polygon, add it to the list
                if (adjacentPolygon->isPointInside(&server)) {
                    adjacentServers.append(server);
                }
            }

            // If both polygons contain servers, create edges between their servers
            if (!currentServers.isEmpty() && !adjacentServers.isEmpty()) {
                for (const Vector2D& currentServer : currentServers) {
                    for (const Vector2D& adjacentServer : adjacentServers) {
                        // Add nodes for the current and adjacent servers
                        int currentID = ui->canvas->networkGraph.addNode2(currentServer.x, currentServer.y);
                        Node* currentNode = ui->canvas->networkGraph.getNode(QString::number(currentID));

                        int adjacentID = ui->canvas->networkGraph.addNode2(adjacentServer.x, adjacentServer.y);
                        Node* adjacentNode = ui->canvas->networkGraph.getNode(QString::number(adjacentID));

                        // Add edge between servers if both nodes exist
                        if (currentNode && adjacentNode) {
                            ui->canvas->networkGraph.addEdge(currentNode, adjacentNode);
                        }
                    }
                }
            }
        }
    }
}



void MainWindow::on_actionLoad_Servers_triggered()
{
    // Open a file dialog to select a JSON file for the mesh.
    QString fileName = QFileDialog::getOpenFileName(this,"Load Server file",".","*.json");
    ui->canvas->loadServers(fileName); // Load the selected mesh into the canvas.
}



void MainWindow::on_actionNew_2_triggered()
{
    ui->canvas->clear(); // Clear any existing content on the canvas.

    // Add a set of predefined points to the canvas.
    QVector<Vector2D> tab={ {100,100},{600,30},{200,400},{800,800},{1500,500}};
    ui->canvas->addPoints(tab);

    // Create a set of predefined triangles from the points.
    ui->canvas->addTriangle(0,1,2);
    ui->canvas->addTriangle(2,1,3);
    ui->canvas->addTriangle(1,4,3);

    // Update the status bar with the number of vertices and triangles.
    ui->statusBar->showMessage("Stats: |V|="+QString::number(ui->canvas->getSizeofV())+
                               "  |T|="+QString::number(ui->canvas->getSizeofT()));
}


void MainWindow::on_actionServers_triggered(bool checked)
{
    ui->canvas->showServers=checked; // Set the showCircles property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}


void MainWindow::on_actionRoutes_triggered(bool checked)
{
    ui->canvas->showRoutes=checked; // Set the showRoutes property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}


void MainWindow::on_actionShared_sides_triggered(bool checked)
{
    ui->canvas->showSharedSides=checked; // Set the showSharedSides property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}

void MainWindow::on_actionServer_routes_triggered(bool checked)
{
    ui->canvas->showServerRoutes=checked; // Set the showServerRoutes property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}




server* MainWindow::getServer(Vector2D* v) {
    auto it = ui->canvas->serversObj.begin();
    while (it != ui->canvas->serversObj.end()) {
        server* s = *it;
        if (v->x == s->x && v->y == s->y) {
            return s;
        }
        ++it;
    }
    return nullptr;
}






void MainWindow::on_actionNext_steps_triggered(bool checked)
{
    ui->canvas->nextSteps=checked; // Set the showServerRoutes property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
}



void MainWindow::on_actionEnable_Disable_triggered(bool checked)
{
    ui->canvas->droneMovement=checked; // Set the showServerRoutes property based on the checkbox state.
    update(); // Trigger a repaint of the canvas.
    QString word = checked ? "enabled" : "disabled";
    qDebug()<<"Drone movement is"<<word;

    // Update the status bar with the status
    ui->statusBar->showMessage("Drone movement is "+word);
}

