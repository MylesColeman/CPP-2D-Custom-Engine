# C++: 2D Game Engine Construction
**University Year 2 | Games Engine Construction**

A highly flexible 2D game engine built from scratch using C++ and SFML. This project focuses on core engine architecture, modular asset management, and the implementation of robust design patterns for real-time simulation.

> **[🔗 View the full technical breakdown on my Portfolio](https://sites.google.com/view/myles-coleman/projects/2d-game-engine)**

## 📺 Development Showcase
[![Engine Development Showcase](https://img.youtube.com/vi/8Y87SYCAyjg/0.jpg)](https://www.youtube.com/watch?v=8Y87SYCAyjg)
*Out of date demo video showcasing sprite animation, observer-based input, and ImGui debug integration.*

## 🕹️ Engine Architecture Overview
This engine was designed to enforce a strict separation of concerns, moving away from monolithic codebases toward an encapsulated system where rendering, logic, and input are handled by dedicated managers.

## 🛠️ Key Technical Features

### Core Systems & Architecture
* **Decoupled Engine Architecture:** Enforced a strict separation of concerns by isolating Simulation (Physics/Logic) from Graphics (Rendering), ensuring thread-safe development and modularity.
* **Fixed-Timestep Game Loop:** Engineered a core loop utilising an accumulator-based fixed timestep to guarantee deterministic physics and consistent game speed across variable hardware and frame rates.
* **Data-Driven Level Parser:** Implemented an external level-loading pipeline that parses .txt configuration files to dynamically instantiate world geometry and entities, allowing for rapid iteration without source-code modification.
* **Modern Memory Management:** Utilised a polymorphic entity hierarchy managed through Smart Pointers (std::unique_ptr), employing an "Ownership vs. Observation" strategy to balance memory safety with high-performance access.

### Advanced Simulation & Tooling
* **Custom Axis-Separated Physics:** Developed a custom AABB collision solver with independent horizontal and vertical resolution to prevent "diagonal sticking." Includes gravity simulation and precise "grounded" state detection.
* **Object-Pooled Gameplay Systems:** Engineered a centralised Object Pool for projectiles to minimise heap fragmentation and allocation overhead during real-time combat simulation.
* **Observer Pattern Input:** Utilised the Observer Pattern to broadcast action vectors, enabling decoupled, simultaneous handling of complex movement and action states.
* **ImGui Debug Suite:** Integrated a professional developer overlay for real-time monitoring of velocity vectors, physics bounds, and entity states, facilitating rapid debugging of the simulation environment.

## 💻 Technical Specs
* **Language:** C++
* **Graphics Library:** SFML
* **UI/Debug:** ImGui
* **Methodology:** OOP, Singleton/Observer Design Patterns
