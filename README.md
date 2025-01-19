# Drone Project
Kyllian Cuevas & Thomas Mirbey
Documentation is located in the doc directory in the rar file.
# Project Constraints, Overview, and Instructions

This document provides an overview of the project, key constraints, drone behavior, and how to load a new configuration file.

## Drone Movement and Routing
- **Single Drone Movement per Hop**: 
  - Only the first drone to reach a particular hop will move. All other drones at that hop will land and wait for their turn.

## Communication Constraints
- **Server Communication**: 
  - Servers can only communicate if they are within adjacent polygons.

## Graph-based Routing
- **Graph Route Construction**: 
  - The drone's movement is governed by a graph-based route system.
  - **Nodes**:
    - Each polygon has a **center point** (used as a node in the graph).
    - A point is placed at the **midpoint of each shared side** between adjacent polygons.
  - **Links**:
    - Links are established between these nodes.
    - A link exists between each server and the **nearest center point** of its polygon.

## Drone Behavior
- **Route Initialization**: 
  - After spawning, the drone will first move towards the **nearest server**.
  - Once it reaches that server, it will request the route to its **destination server**.
  - The route is dynamically requested and computed as the drone moves through the graph.

## Drone Colors and Status
- **Color Code**:
  Each drone is assigned a **primary color** based on the configuration file. Additionally, drones have a **color-coded status** that reflects their current state during the simulation:
  - **Yellow**: When the drone hasn't reached its initial position.
  - **Green**: When the drone is actively moving towards the destination server.
  - **Red**: When the drone has arrived at the destination server.
  - **Blue**: When the drone is landed and waiting to proceed to the next hop.

## Configuration File Loading
- **Loading a New Configuration File**:
  - To load a new configuration JSON file, go to line 32 of the `mainwindow.cpp` file:
    ```cpp
    QString configFile = "path/to/your/configFile/config2.json";
    ```
  - Change the `configFile` variable to the desired file path, save, and the application will use the updated configuration.
