#ifndef CANVAS_H
#define CANVAS_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Canvas related methods
 **/

#include <QWidget>
#include "triangle.h"
#include "myPolygon.h"
#include "graph.h"
#include "server.h"
#include "drone.h"

/**
 * @brief The Canvas class allows to draw the graphical interface canvas content.
 *
 * It stores:
 *  - a list of triangles : QVector<Triangle*>;
 *  - a list of vertices : QVector<Vector2D> vertices;
 *  - a list of polygons : QVector<MyPolygon*>;
 *  - the scale = 0.5 (float);
 *  - the origin = QPoint(10, 10) (QPoint);
 *
 * It is used by the MainWindow class to create the data visualization.
 */
class Canvas : public QWidget
{
    Q_OBJECT

public:
    Vector2D infLeft, infRight, topLeft, topRight;
    /**
     * @brief Constructs the Canvas object.
     * @param parent The parent QWidget, default is nullptr.
     */
    explicit Canvas(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Canvas object and cleans up any resources.
     */
    ~Canvas();

    /**
     * @brief Loads a new mesh from the specified title.
     * @param title The title or path to the mesh file.
     */
    void loadMesh(const QString &title);

    /**
     * @brief Loads the server list
     * @param title The title or path to the server file.
     */
    void loadServers(const QString &title);

    /**
     * @brief Loads the drone list
     * @param title The title or path to the drone file.
     */
    void loadDrones(const QString &title);


    /**
     * @brief Handles the paint events to redraw the widget when necessary.
     * @param event The paint event that is triggered for repainting.
     */
    void paintEvent(QPaintEvent*) override;

    /**
     * @brief Handles the resize events to adjust the widget size.
     * @param event The resize event triggered when the widget's size changes.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Handles mouse press events triggered by mouse clicks.
     * @param event The mouse event triggered when a mouse button is pressed.
     */
    void mousePressEvent(QMouseEvent*) override;

    /**
     * @brief Handles mouse move events when the mouse is moved over the widget.
     * @param event The mouse event triggered when the mouse moves.
     */
    void mouseMoveEvent(QMouseEvent*) override;

    /**
     * @brief Adds points (vertices) to the graphical interface, used in loading a mesh.
     * @param tab The list of points (vertices) to add.
     */
    void addPoints(const QVector<Vector2D> &tab);

    /**
     * @brief Creates a triangle using the previously added points.
     * @param id0 The index of the first point.
     * @param id1 The index of the second point.
     * @param id2 The index of the third point.
     */
    void addTriangle(int id0, int id1, int id2);

    /**
     * @brief Creates a colored triangle using the previously added points.
     * @param id0 The index of the first point.
     * @param id1 The index of the second point.
     * @param id2 The index of the third point.
     * @param color The color to assign to the triangle.
     */
    void addTriangle(int id0, int id1, int id2, const QColor &color);

    /**
     * @brief Clears the canvas, removing all content.
     */
    void clear();

    /**
     * @brief Rescales the content on the canvas.
     */
    void reScale();

    /**
     * @brief Returns the number of vertices in the canvas.
     * @return The number of vertices.
     */
    inline int getSizeofV() { return vertices.size();}

    /**
     * @brief Returns the number of triangles in the canvas.
     * @return The number of triangles.
     */
    inline int getSizeofT() { return triangles.size();}

    /**
     * @brief Checks if all triangles in the canvas satisfy the Delaunay condition.
     * @return True if all triangles are Delaunay, false otherwise.
     */
    bool checkDelaunay();

    /**
     * @brief Flips non-Delaunay triangles to satisfy the Delaunay condition.
     */
    void flippAll();

    /**
     * @brief Finds the opposite points of triangles that share a common edge with the given triangle.
     * @param tri The triangle whose common-edge opposite points are being searched for.
     * @return A QVector of pointers to Vector2D representing the opposite points.
     */
    QVector<const Vector2D*> findOppositePointOfTrianglesWithEdgeCommon(const Triangle &tri);

    /**
     * @brief Finds all triangles that contain the specified point.
     * @param M The point to search for inside the triangles.
     * @return A QList of pointers to Triangle objects that contain the specified point.
     */
    QList<Triangle*> findTrianglesContainingPoint(const Vector2D& M);

    /**
     * @brief Sorts a list of triangles based on their position relative to the given reference point (left to right).
     * @param triangles The list of triangles to be sorted.
     * @param referencePoint The reference point to sort the triangles by.
     * @return A QVector of sorted triangles.
     */
    QVector<Triangle*> sortTrianglesByLeft(const QVector<Triangle*>& triangles, const Vector2D& referencePoint);

    /**
     * @brief Creates a polygon from a collection of triangles based on their relative positions to a reference point.
     * @param triangles The list of triangles to create a polygon from.
     * @param referencePoint The reference point used to determine the order of the triangles.
     */
    void createPolygonFromTriangles(const QVector<Triangle*>& triangles, const Vector2D& referencePoint);

    /**
     * @brief Calculates and returns the bounding box of the vertices.
     * @return A QPair containing the bottom-left and top-right coordinates of the bounding box.
     */
    QPair<Vector2D,Vector2D> getBox();

signals:
    /**
     * @brief Emits an update message to update the status bar.
     * @param s The message to be displayed in the status bar.
     */
    void updateSB(QString s);

private:
    const double droneCollisionDistance=1.5; ///< distance to detect collision with other drone

    float scale = 0.5; ///< The scale factor applied to the canvas.
    QPoint origin = QPoint(10, 10); ///< The origin point of the canvas.
    QImage droneImg,droneImgRed,droneImgBlue,droneImgYellow,droneImgGreen,serverImg; ///< Images for the graphical interface.

public:
    QVector<Vector2D> vertices; ///< List of vertices in the canvas.

    QVector<Vector2D> servers; ///< List of points representing servers in the canvas.
    QVector<server*> serversObj; ///< List of servers in the canvas.

    QVector<myDrone*> drones; ///< List of points representing servers in the canvas.


    QVector<MyPolygon*> polygons; ///< List of polygons created from the triangles.
    QVector<Triangle*> triangles; ///< List of triangles in the canvas.
    Graph graph; ///< Graph for drone routes
    Graph networkGraph; ///< Graph for server routes

    bool showTriangles = true; ///< Flag to control the visibility of triangles in the data visualization.
    bool showCenters = false; ///< Flag to control the visibility of center points in the data visualization.
    bool showCircles = false; ///< Flag to control the visibility of circles in the data visualization.

    bool showServers = true; ///< Flag to control the visibility of servers in the visualization.
    bool showRoutes = true; ///< Flag to control the visibility of routes between servers and drones.
    bool showSharedSides = false; ///< Flag to control the visibility of shared sides in the Voronoi diagram.

    bool voronoiDone = false; ///< Flag to check if the Voronoi diagram has been generated.
    bool showServerRoutes = false; ///< Flag to control the visibility of the server routes.
    bool nextSteps = false; ///< Flag to indicate if the next steps (e.g., drone movement) should be displayed.

    bool droneMovement = false; ///< Flag to control the visibility of drone movement in the simulation.


    /**
     * @brief Creates a polygon (Voronoi diagram) based on a list of sorted triangles.
     * @param sortedTriangles A sorted list of triangles used to create the Voronoi diagram.
     */
    void createPolygonFromTriangles(const QVector<Triangle*>& sortedTriangles);

    /**
     * @brief Returns the list of vertices.
     * @return A QVector of Vector2D objects representing the vertices.
     */
    QVector<Vector2D> getVertices() { return vertices; }

    /**
 * @brief Creates a polygon (Voronoi diagram) based on a list of sorted triangles.
 * @param sortedTriangles A sorted list of triangles used to create the Voronoi diagram.
 */
    void createPolygon(const QVector<Vector2D> pointList);

    /**
 * @brief Creates a polygon (Voronoi diagram) with a specified color based on a list of sorted triangles.
 * @param pointList A sorted list of triangles used to create the Voronoi diagram.
 * @param color The color to fill the polygon.
 */
    void createPolygon(const QVector<Vector2D> pointList, QColor color);

    /**
 * @brief Sorts points by angle relative to a reference point.
 * @param points A list of points to sort.
 * @param referencePoint The reference point used to calculate the angle.
 * @return A sorted list of points based on the angle from the reference point.
 */
    QVector<Vector2D> sortPointsByAngle(const QVector<Vector2D>& points, const Vector2D& referencePoint);

    /**
 * @brief Finds a server by its position.
 * @param server The position of the server to find.
 * @return A pointer to the server if found, or nullptr if not.
 */
    server* getServer(const Vector2D server);

};

#endif // CANVAS_H
