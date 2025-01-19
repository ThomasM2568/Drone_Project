#ifndef DRONE_H
#define DRONE_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Drone related methods
 **/

#include <QWidget>
#include <QProgressBar>
#include "vector2d.h"
#include <QImage>
#include <QString>
#include <QColor>
#include "server.h"

/**
 * @brief The myDrone class represents a drone
 * including movement, collision detection, route planning, and state management.
 *
 * The class supports features such as:
 * - Setting destinations using coordinates or server positions.
 * - Updating the drone's state over time (e.g., position, speed).
 * - Route planning using graph structures and network graphs.
 */
class myDrone {
public:
    const double maxSpeed = 50; ///< Maximum speed of the drone in pixels per second.
    const double maxPower = 200; ///< Maximum power available for the drone's motors.
    const double takeoffSpeed = 2.5; ///< Speed (in units per second) at which the drone takes off.
    const double hoveringHeight = 5; ///< The height (in units) at which the drone hovers.
    const double coefCollision = 1000; ///< Coefficient used for collision avoidance calculations.
    const double damping = 0.2; ///< Damping factor for simulating motion (slows down the drone over time).
    const double chargingSpeed = 10; ///< Rate at which the drone charges its power (units of power per second).
    const double powerConsumption = 5; ///< Rate at which the drone consumes power during flight (units of power per second).
    QList<Node*> route; ///< A list that holds the nodes of the drone's flight path (route).
    bool routeInitialized = false; ///< Flag indicating whether the drone's route has been initialized.
    int currentNodeIndex; ///< Index of the current node in the route that the drone is heading towards.
    bool isActive = true; ///< Flag indicating whether the drone is currently active or not.

    enum myDroneStatus { landed, takeoff, landing, hovering, turning, flying };

    myDrone(const QString &name, const QColor &color, const Vector2D &initialPosition, const QString &serverAddress);
    myDrone(int id, const QString &name, const QColor &color, const Vector2D &initialPosition, const QString &serverAddress);
    ~myDrone();

    /**
 * @brief Starts the drone's movement or process.
 */
    void start();

    /**
 * @brief Stops the drone's movement or process.
 */
    void stop();

    /**
 * @brief Sets the drone's speed.
 * @param speed The speed to set for the drone.
 */
    void setSpeed(double speed);

    /**
 * @brief Sets the initial position of the drone.
 * @param position The position to set for the drone.
 */
    void setInitialPosition(const Vector2D &position);

    /**
 * @brief Sets the drone's destination based on a list of servers.
 * @param serverList A list of servers to navigate towards.
 */
    void setGoToPosition(const QVector<Vector2D>& serverList);

    /**
 * @brief Sets the drone's destination to a specific server.
 * @param server The server to set as the drone's destination.
 */
    void setGoToPosition(Vector2D& server);

    /**
 * @brief Sets the drone's destination to a specific server.
 * @param server The server to set as the drone's destination.
 */
    void setGoToPosition(Node* server);


    /**
 * @brief Sets the drone's goal position.
 * @param position The goal position to set for the drone.
 */
    void setGoalPosition(const Vector2D &position);

    /**
 * @brief Sets the drone's goal position based on a list of servers.
 * @param serverList A list of servers to set as the goal position.
 */
    void setGoalPosition(const QVector<Vector2D>& serverList);

    /**
 * @brief Sets the drone's position to a specific server.
 * @param server The server to set as the drone's position.
 */
    void setPosition(Node* server);

    /**
 * @brief Sets the drone's position based on x and y coordinates.
 * @param x The x-coordinate to set for the drone's position.
 * @param y The y-coordinate to set for the drone's position.
 */
    void setPosition(int x, int y);


    /**
 * @brief Gets the drone's goal position.
 * @return The current goal position of the drone.
 */
    Vector2D getGoalPosition() const;

    /**
 * @brief Gets the drone's current position.
 * @return The current position of the drone.
 */
    Vector2D getPosition() const;

    /**
 * @brief Gets the position the drone is heading to.
 * @return The target position the drone is heading towards.
 */
    Vector2D getGoToPosition() const;

    /**
 * @brief Gets the current status of the drone.
 * @return The current status of the drone.
 */
    myDroneStatus getStatus() const { return status; }

    /**
 * @brief Checks if the drone is active.
 * @return true if the drone is active, false otherwise.
 */
    bool getIsActive() const { return isActive; }

    /**
 * @brief Sets the drone's active status.
 * @param active The status to set (true for active, false for inactive).
 */
    void setIsActive(bool active) { isActive = active; }


    /**
 * @brief Gets the name of the drone.
 * @return The name of the drone.
 */
    QString getName() const;

    /**
 * @brief Gets the ID of the drone.
 * @return The unique ID of the drone.
 */
    int getId() const;

    /**
 * @brief Gets the azimuth of the drone.
 * @return The azimuth (orientation) of the drone in degrees.
 */
    double getAzimut() const;

    /**
 * @brief Gets the current power level of the drone.
 * @return The current power level of the drone (in units of power).
 */
    double getPower() const;

    /**
 * @brief Gets the color of the drone.
 * @return The color of the drone.
 */
    QColor getColor() const;


    /**
 * @brief Updates the drone's state.
 * @param dt The time step used to update the drone's state.
 */
    void update(double dt);

    /**
 * @brief Initializes the collision detection system for the drone.
 */
    void initCollision();

    /**
 * @brief Adds a potential collision to the drone's collision detection system.
 * @param other The other object involved in the collision.
 * @param threshold The distance threshold for collision detection.
 */
    void addCollision(const Vector2D &other, float threshold);

    /**
 * @brief Checks if the drone has a collision.
 * @return True if the drone has a collision, false otherwise.
 */
    bool hasCollision() const;


    /**
 * @brief Updates the position of the drone based on the time step.
 * @param dt The time step used to update the drone's position.
 */
    void updatePosition(double dt);


    /**
 * @brief Sets the route for the drone using graph pointers and specified start and end nodes.
 * @param graph The main graph used to calculate the route.
 * @param networkGraph The network graph used for route computation.
 * @param startNode The starting node of the route.
 * @param endNode The destination node of the route.
 * @param s The server that the drone will interact with.
 */
    void setRoute(Graph* graph, Graph* networkGraph, Node* startNode, Node* endNode, server* s);


private:
    const int compasSize = 48; ///< size of the compass image (compasSize x compasSize)
    const int barSpace = 150; ///< minimum size of the ProgressBar

    myDroneStatus status; ///< status of the drone
    double height; ///< current height of the drone
    QString name; ///< name of the drone
    int id; ///< id of the drone
    QColor color; ///< color representing the drone
    QString serverAddress; ///< server address associated with the drone

    QProgressBar *speedPB; ///< progress bar widget for the speed
    QProgressBar *powerPB; ///< progress bar widget for the power

    Vector2D position; ///< current position of the drone
    Vector2D goalPosition; ///< goal position for the drone
    Vector2D goToPosition;
    Vector2D direction; ///< current direction
    Vector2D velocity; ///< current speed vector
    Vector2D forceCollision; ///< force generated by the collision detection

    double speed; ///< current speed
    double speedSetpoint; ///< speed to reach if possible
    double power; ///< current power
    double azimut; ///< rotation angle of the drone

    bool showCollision; ///< true if a collision is detected

    //QImage compassImg, stopImg, takeoffImg, landingImg;
};

#endif // DRONE_H
