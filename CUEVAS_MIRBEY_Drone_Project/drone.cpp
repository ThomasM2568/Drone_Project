#include "drone.h"
#include "server.h"

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Drone related methods
 **/

myDrone::myDrone(const QString &name, const QColor &color, const Vector2D &initialPosition, const QString &serverAddress)
    : name(name), color(color), position(initialPosition), serverAddress(serverAddress),
    status(landed), speed(0), power(maxPower / 2.0), azimut(0), showCollision(false) {
}

myDrone::myDrone(int id, const QString &name, const QColor &color, const Vector2D &initialPosition, const QString &serverAddress)
    : id(id), name(name), color(color), position(initialPosition), serverAddress(serverAddress),
    status(landed), speed(0), power(maxPower / 2.0), azimut(0), showCollision(false) {
}

myDrone::~myDrone() {}

void myDrone::start() {
    status = takeoff;
    height = 0;
}

void myDrone::stop() {
    status = landing;
}

void myDrone::setSpeed(double speed) {
    speedSetpoint = (speed > maxSpeed) ? maxSpeed : speed;
}

void myDrone::setInitialPosition(const Vector2D &position) {
    if (status == landed) this->position = position;
}

#include <cmath> // Pour std::sqrt

void myDrone::setGoToPosition(const QVector<Vector2D>& serverList) {
    //qDebug() << "setGoalPosition called for drone" << this->name;

    Vector2D* closestServer = nullptr;  // Pointer to store the closest server
    double minDistance = std::numeric_limits<double>::max();  // Initialize the minimum distance as an infinite value

    // Loop through the list of servers
    for (const auto& server : serverList) {
        // Calculate the distance between the drone's current position and the server's position
        double distance = std::sqrt(std::pow(server.x - position.x, 2) + std::pow(server.y - position.y, 2));

        // If the distance is smaller than the current minimum distance
        if (distance < minDistance) {
            minDistance = distance;  // Update the minimum distance
            closestServer = const_cast<Vector2D*>(&server);  // Update the closest server pointer
        }
    }

    // If a closest server has been found, use its coordinates
    if (closestServer) {
        //qDebug() << "Closest server is at" << closestServer->x << "," << closestServer->y;
        goToPosition = *closestServer;  // Update the goToPosition with the coordinates of the closest server
        //qDebug() << "Goal position set to" << goalPosition.x << "," << goalPosition.y;
    } else {
        // If no servers are found, set goToPosition to the current position
        //qDebug() << "No servers found, using current position as goal.";
        goToPosition = position;
    }
}


void myDrone::setGoToPosition(Node* N) {
    goToPosition=new Vector2D(N->getX(),N->getY());
}

void myDrone::setPosition(Node* server) {
    position=new Vector2D(server->getX(),server->getY());
}
void myDrone::setPosition(int x, int y) {
    position=new Vector2D(x,y);
}

void myDrone::setGoalPosition(const Vector2D &position) {
    goalPosition = position;
}

void myDrone::setGoalPosition(const QVector<Vector2D>& serverList) {
    //qDebug()<<"setGoalPosition called for drone"<<this->name;
    auto it = serverList.begin(); // Iterator to traverse the QVector
    while (it != serverList.end()) {
        if (it->name == serverAddress) {
            //qDebug()<<"Modified destination";
            goalPosition = new Vector2D(it->x,it->y);
            //qDebug()<<goalPosition.x<<","<<goalPosition.y;
            return; // Exit the function once the condition is met
        }
        ++it; // Move to the next element
    }
    // If no match is found, set goalPosition to position
    //qDebug()<<"Destination unchanged";
    goalPosition = position;
}


Vector2D myDrone::getGoalPosition() const {
    return goalPosition;
}

Vector2D myDrone::getPosition() const {
    return position;
}

Vector2D myDrone::getGoToPosition() const {
    return goToPosition;
}


QString myDrone::getName() const {
    return name;
}

int myDrone::getId() const {
    return id;
}

double myDrone::getAzimut() const {
    return azimut;
}

double myDrone::getPower() const {
    return 100.0 * power / maxPower;
}

QColor myDrone::getColor() const {
    return color;
}




/*QString myDrone::getServerAddress() const {
    return QString(serverAddress.x);
}*/

void myDrone::update(double dt) {
    if (status == landed) {
        power += dt * chargingSpeed;
        if (power > maxPower) power = maxPower;
        return;
    }

    if (status == takeoff) {
        height += dt * takeoffSpeed;
        if (height >= hoveringHeight) {
            height = hoveringHeight;
            status = hovering;
        }
        power -= dt * powerConsumption;
        if (power < 20 + powerConsumption / takeoffSpeed) {
            status = landing;
            speed = 0;
        }
        return;
    }

    if (status == landing) {
        height -= dt * takeoffSpeed;
        if (height <= 0) {
            height = 0;
            status = landed;
        }
        power -= dt * powerConsumption;
        return;
    }

    if (status >= hovering) {
        Vector2D toGoal = goalPosition - position;
        double distance = toGoal.length();

        velocity = (1 - dt * (1 - damping)) * velocity + ((maxPower * dt / distance) * toGoal);
        position = position + dt * velocity;
        speed = velocity.length();

        if (toGoal.length() < 1.0 && speed < 10) {
            velocity.set(0, 0);
            speed = 0;
            status = landing;
        }

        power -= dt * powerConsumption;
        if (power < 20 + powerConsumption / takeoffSpeed) {
            speed = 0;
            velocity.set(0, 0);
            status = landing;
        }
    }
}

void myDrone::initCollision() {
    forceCollision.set(0, 0);
    showCollision = false;
}

void myDrone::addCollision(const Vector2D &other, float threshold) {
    Vector2D diff = other - position;
    double length = diff.length();
    if (length < threshold) {
        forceCollision = forceCollision + (-coefCollision / threshold) * diff;
        showCollision = true;
    }
}

bool myDrone::hasCollision() const {
    return showCollision;
}


void myDrone::updatePosition(double dt) {
    // Calculate the direction vector towards the goal position
    Vector2D directionToGoal = goToPosition - position;
    double distanceToGoal = directionToGoal.length();  // Calculate the distance to the goal position

    // If the distance to the goal is small, do not move the drone
    if (distanceToGoal < 1.0) {
        return;  // Drone is close enough to the goal, no need to move
    }

    // Calculate the drone's speed based on maximum speed and delta time
    double speed = maxSpeed * dt * 0.08;  // maxSpeed is in pixels per second, adjust speed based on time

    // Move the drone towards the goal at the calculated speed (without normalizing direction)
    position = position + directionToGoal * speed;  // Move the drone without normalizing the direction vector

    // If the drone overshoots the goal position, set it exactly to the goal position
    if ((goToPosition - position).length() == 0) {
        position = goToPosition;  // Set the position exactly at the goal position
    }
}



void myDrone::setRoute(Graph* graph,Graph* networkGraph,Node* startNode, Node* endNode, server* s) {
    route = s->findNextHopToServer(graph,networkGraph,startNode,endNode);
    currentNodeIndex=0;
    routeInitialized=true;
}


