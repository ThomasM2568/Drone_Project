#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo MainWindow related methods
 **/

#include <QMainWindow>
#include "node.h"
#include "graph.h"
#include "server.h"
#include "drone.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class represents the main graphical user interface of the application.
 * It handles the window and provides slots for various actions triggered by the user.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the MainWindow object and initializes the graphical interface.
     * @param parent The parent QWidget, default is nullptr.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object and cleans up any resources.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Handles the "New" action, triggered when the user selects "New" from the menu.
     * This slot can be used to create a new document or initialize a new state.
     */
    void on_actionNew2_triggered();

    /**
     * @brief Handles the "Quit" action, triggered when the user selects "Quit" from the menu.
     * This slot allows the application to exit gracefully.
     */
    void on_actionQuit_triggered();

    /**
     * @brief Handles the "Load" action, triggered when the user selects "Load" from the menu.
     * This slot allows loading a new file or data into the application.
     */
    void on_actionLoad_triggered();

    /**
     * @brief Handles the "Circles" action, triggered when toggling the circles option.
     * This slot controls whether circles should be shown in the graphical interface.
     * @param checked Indicates whether the "Circles" option is checked (true) or unchecked (false).
     */
    void on_actionCircles_triggered(bool checked);

    /**
     * @brief Handles the "Triangles" action, triggered when toggling the triangles option.
     * This slot controls whether triangles should be shown in the graphical interface.
     * @param checked Indicates whether the "Triangles" option is checked (true) or unchecked (false).
     */
    void on_actionTriangles_triggered(bool checked);

    /**
     * @brief Handles the "Center" action, triggered when toggling the center option.
     * This slot controls whether the center should be shown in the graphical interface.
     * @param checked Indicates whether the "Center" option is checked (true) or unchecked (false).
     */
    void on_actionCenter_triggered(bool checked);

    /**
     * @brief Handles the "About" action, triggered when the user selects "About" from the menu.
     * This slot shows information about the application, such as version, authors, etc.
     */
    void on_actionAbout_triggered();

    /**
     * @brief Handles the "Check Delaunay" action, triggered when the user selects this option from the menu.
     * This slot checks whether the current triangulation satisfies the Delaunay condition.
     */
    void on_actionCheck_Delaunay_triggered();

    /**
     * @brief Handles the "Flip Process" action, triggered for a flip operation.
     * This slot allows flipping non-Delaunay triangles to ensure Delaunay condition is met.
     */
    void on_actionProcess_flip_triggered();

    /**
     * @brief Handles the "Voronoi" action, triggered to generate the Voronoi diagram.
     * This slot allows the application to generate and display the Voronoi diagram.
     */
    void on_actionVoronoi_triggered();

    /**
 * @brief Triggered when the "Load Servers" action is executed.
 */
    void on_actionLoad_Servers_triggered();

    /**
 * @brief Triggered when the "New 2" action is executed.
 */
    void on_actionNew_2_triggered();

    /**
 * @brief Triggered when the "Servers" action is toggled.
 *        Shows or hides the servers in the visualization.
 * @param checked The toggle state (true for checked, false for unchecked).
 */
    void on_actionServers_triggered(bool checked);

    /**
 * @brief Triggered when the "Routes" action is toggled.
 *        Shows or hides the routes in the visualization.
 * @param checked The toggle state (true for checked, false for unchecked).
 */
    void on_actionRoutes_triggered(bool checked);

    /**
 * @brief Triggered when the "Shared Sides" action is toggled.
 *        Shows or hides shared sides in the visualization.
 * @param checked The toggle state (true for checked, false for unchecked).
 */
    void on_actionShared_sides_triggered(bool checked);

    /**
 * @brief Creates the graph representation for the network.
 */
    void createGraph();


    /**
 * @brief Returns the server corresponding to the given Vector2D object.
 * @param v The Vector2D object representing the server's location.
 * @return The server associated with the given location.
 */
    server* getServer(Vector2D* v);

    /**
 * @brief Triggered when the "Server Routes" action is toggled.
 *        Shows or hides the server routes in the visualization.
 * @param checked The toggle state (true for checked, false for unchecked).
 */
    void on_actionServer_routes_triggered(bool checked);

    /**
 * @brief Triggered when the "Next Steps" action is toggled.
 *        Shows or hides the next steps for the drones in the visualization.
 * @param checked The toggle state (true for checked, false for unchecked).
 */
    void on_actionNext_steps_triggered(bool checked);

    /**
 * @brief Triggered when the "Enable/Disable" action is toggled.
 *        Enables or disables the movement of drones.
 * @param checked The toggle state (true for enabled, false for disabled).
 */
    void on_actionEnable_Disable_triggered(bool checked);


private:
    Ui::MainWindow *ui; ///< Pointer to the graphical interface object that contains UI components.
};

#endif // MAINWINDOW_H
