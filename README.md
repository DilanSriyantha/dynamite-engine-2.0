# 💣 DynamiteEngine 2.0

A **modular, event-driven 2D game engine** built in **C++ with SDL2**, designed with **low coupling, high cohesion**, and strong support for **interoperability and scalability**.

This engine powers **JungleQuest**, an infinite side-scrolling hack-and-slash game developed as part of a Bachelor of Computer Science project.

<div>
    <img src="https://i.ibb.co/Q3MD8jfv/ezgif-4a932540167e3587.gif" width="32%" />
    <img src="https://i.ibb.co/JFtY6DGc/ezgif-27c8df05b9a61b60.gif" width="32%" />
    <img src="https://i.ibb.co/7JPJBFtJ/2025-11-16-08-38-33-1-00-00-15-00-00-20.gif" width="32%" />
</div>

---

## 🚀 Key Architectural Principles

- 🔗 **Low Coupling & High Cohesion**
- ⚡ **Event-Driven Architecture**
- 🧩 **Interface-Based Design**
- 🌐 **Interoperability (HTTP/JSON APIs)**
- 🧬 **Virtual Identity (Cloud-based user systems)**

---

## 🏗️ Core Architecture

The engine is built around **clearly defined interfaces**, enabling flexible and maintainable system design.

### 🔧 Interfaces

- `ICanvas` – UI Container abstraction
- `IComponent` – Base for all attachable game components
- `IPosition` – Spatial data handling
- `IScene` – Scene management
- `IScript` – Game logic scripting
- `ISystem` – Core system-level operations

> These interfaces ensure that subsystems interact through **contracts rather than implementations**, making the engine highly extensible.

---

## 🔄 Event System (EventBus)

A central **EventBus** enables communication between systems without tight dependencies.

### 📌 Use Cases

- 🎮 **UI & Input Events**
  - UI actions emit events (e.g., button clicks)
  - Subscribed systems react independently

- ⚔️ **Combat System**
  - Damage events trigger updates in health, UI, etc.

### ✅ Benefits

- Decoupled system communication  
- Easy addition of new features  
- Scalable architecture  

---

## 🌐 Interoperability

The engine integrates external services using:

- **libcurl**
- **HTTP/HTTPS protocols**
- **JSON-based communication**

### 🔌 Integrated APIs

- **Heart Game API**
- **Firebase Authentication API**

---

## 🔐 Authentication System

Implemented using **Firebase Authentication**:

- 📧 Email/password registration
- 🔑 Secure login system
- ☁️ Cloud-based player identity

This enables:
- Persistent player data
- Unique user sessions
- Scalable backend integration

---

# 🎮 JungleQuest (Built with DynamiteEngine 2.0)

An **infinite side-scrolling hack-and-slash game** demonstrating the engine’s capabilities.

---

## 🕹️ Gameplay Features

- ⚔️ Continuous enemy spawning
- 🏃 Movement-driven gameplay (rightward progression)
- ❤️ **3 Life-Line System**
- 🧠 Dynamic challenge system (Heart Game API integration)
- ☁️ Cloud-based authentication and data storage

---

## 💡 Unique Mechanics

### ❤️ Life-Line System

- Player starts with **3 life-lines**
- When health reaches zero:
  - A **mini-game challenge** appears
  - Player must solve a **Heart Game API puzzle**
- Success:
  - Consumes 1 life-line
  - Fully restores health
- Failure or no life-lines:
  - Game Over

---

## 🔁 Replayability & Engagement

- Infinite gameplay loop
- Dynamic API-driven challenges
- Increasing difficulty over time
- Cloud-based score tracking

---

## ⚙️ Tech Stack

| Component        | Technology              |
|-----------------|------------------------|
| Engine          | C++                    |
| Rendering       | SDL2                   |
| Networking      | libcurl                |
| Data Format     | JSON                   |
| Authentication  | Firebase Auth          |
| APIs            | Heart Game API         |

---


---

## 🧪 Purpose of the Project

This project was developed as part of a **Bachelor of Computer Science** degree to:

- Demonstrate **software architecture principles**
- Apply **event-driven system design**
- Showcase **real-world API integration**
- Build a **scalable and extensible game engine**

---

## 🏁 Future Improvements

- 🎨 Advanced rendering pipeline
- 🧠 AI-based enemy behaviors
- 🌍 Multiplayer support
- 🧩 Plugin system for engine extensions
- 📦 Asset management system

---

## 👨‍💻 Author

**Dilan Sriyantha Thudugala**  
BSc Computer Science – University of Bedfordshire  

---

## 📜 License

MIT

## Screenshots
<div>
    <img src="https://i.ibb.co/F4G3fxf8/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-55-30-PM.png" />
    <img src="https://i.ibb.co/KjtZwmZJ/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-55-14-PM.png" />
    <img src="https://i.ibb.co/1Jf20Hbq/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-55-40-PM.png" />
    <img src="https://i.ibb.co/HLrFBMC4/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-55-49-PM.png" />
    <img src="https://i.ibb.co/tMSFT9FW/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-56-11-PM.png" />
    <img src="https://i.ibb.co/DnLFhGm/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-56-43-PM.png" />
    <img src="https://i.ibb.co/YFFBysRz/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-57-07-PM.png" />
    <img src="https://i.ibb.co/Lz1b6nQ9/2025-11-16-08-38-33-mkv-VLC-media-player-3-25-2026-2-57-14-PM.png" />
</div>
