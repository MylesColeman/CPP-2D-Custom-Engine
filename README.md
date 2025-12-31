# C++: 2D Game Engine Construction
**University Year 2 | Games Engine Construction | WIP**

A highly flexible 2D game engine built from scratch using C++ and SFML. This project focuses on core engine architecture, modular asset management, and the implementation of robust design patterns for real-time simulation.

> **[🔗 View the full technical breakdown on my Portfolio](https://sites.google.com/view/myles-coleman/projects/2d-game-engine-wip)**

## 📺 Development Showcase
[![Engine Development Showcase](https://img.youtube.com/vi/8Y87SYCAyjg/0.jpg)](https://www.youtube.com/watch?v=8Y87SYCAyjg)
*Current build demonstrating sprite animation, observer-based input, and ImGui debug integration.*

## 🕹️ Engine Architecture Overview
This engine was designed to enforce a strict separation of concerns, moving away from monolithic codebases toward an encapsulated system where rendering, logic, and input are handled by dedicated managers.

## 🛠️ Key Technical Features

### Core Systems & Architecture
* **Encapsulated Graphics System:** Centralised rendering logic to manage all visual output and SFML window states through a dedicated `Graphics` class.
* **Base Entity Framework:** Implemented an extensive `Entity` class that handles transformations and state updates, serving as the parent for all game objects.
* **Texture & Animation Managers:** Built to prevent redundant resource loading. The `AnimationManager` handles sprite sheet frame data and playback speeds independently of the entities themselves.

### Advanced Implementation
* **Observer Pattern Input:** Developed a decoupled `InputManager` that utilizes the Observer pattern. This allows the system to handle simultaneous key presses and persistent input states without direct class dependencies.
* **AABB Collision System:** Custom implementation of Axis-Aligned Bounding Box (AABB) logic for efficient 2D physical interaction detection.
* **ImGui Debug Suite:** Integrated an ImGui-powered UI overlay for real-time monitoring of engine variables, frame rates, and entity states during development.

## 💻 Technical Specs
* **Language:** C++
* **Graphics Library:** SFML
* **UI/Debug:** ImGui
* **Methodology:** OOP, Singleton/Observer Design Patterns
