# 🧱 Minecraft Engine

> A custom **game engine** built from scratch for voxel-style games like *Minecraft* and *Terraria* — optimized for procedural world generation, chunk rendering, and simulation performance.

<p align="center">
  <img src="./Assets/Logo.png" alt="Logo" width="200">
</p>

---

## 🚀 Overview

This engine is designed specifically for **Minecraft-like voxel games**, focusing on performance, chunk management, and real-time procedural generation — without the overhead of general-purpose engines like Unity or Unreal.

By narrowing the engine’s scope, development can target **core voxel mechanics** (terrain, lighting, rendering, and physics) with maximum efficiency.

---

## 🛠️ Current Features

![Editor](./Assets/Editor.png)

<!-- <p align="center">
  <img src="./Assets/Editor.png" alt="Editor" width="200">
</p> -->

- [x] **OpenGL-based rendering** of squares and cubes using the **Blinn–Phong lighting model** — optimized to draw entire scenes in a **single draw call**.  
- [x] Integrated **[Dear ImGui](https://github.com/ocornut/imgui)** for lightweight, real-time UI controls and debugging interfaces.  
- [x] Basic window management, input handling, and rendering loop via C++/OpenGL backend.  
- [x] Configurable camera controls and shader-based material system.  

---

## 🧩 Planned Features

- [ ] **Additional Primitive Support** — spheres, cylinders, cones, and more.  
- [ ] **Procedural Generation Algorithms** — Perlin Noise, Simplex Noise, and hybrid terrain methods.  
- [ ] **World Generation Techniques** — Chunked LOD, Greedy Meshing, seamless world streaming.  
- [ ] **Basic Physics System** — rigid bodies, collisions, and forces.  
- [ ] **Physics-Driven Animation** — ragdoll, procedural motion, and dynamic interaction.  

---

## ⚙️ Build & Run

### 1️⃣ Clone the repository
```bash
git clone https://github.com/zong4/MinecraftEngine.git
```

### 2️⃣ Install dependencies

Make sure [**XMake**](https://xmake.io) is installed on your system.

### 3️⃣ Build and run
```bash
cd MinecraftEngine
xmake run -y
```

> 💡 xmake will automatically download and compile all required dependencies on first run.

## 💡 Design Philosophy

> “Focus on what matters for voxel games.”

This engine deliberately avoids unnecessary systems (animation controllers, complex asset pipelines, etc.) to prioritize:

* **Efficiency** — small codebase, optimized memory layout
* **Clarity** — clean architecture for rapid iteration
* **Flexibility** — modular design for future expansion

The goal is to understand — and control — **every piece** of a voxel-based engine from the ground up.

## 📚 References & Inspirations
* 📘 [OpenGL Tutorials – LearnOpenGL.com](https://learnopengl.com/)
* 🧠 [Game Engine Architecture (by Jason Gregory)](https://www.gameenginearchitecture.com/)
* 🧱 [Hazel Engine (by TheCherno)](https://github.com/TheCherno/Hazel)

## 🧠 Tech Stack

| Category              | Technology                               |
| --------------------- | ---------------------------------------- |
| **Language**          | C++17                                    |
| **Build System**      | [XMake](https://xmake.io)                |
| **Rendering**         | OpenGL                                   |
| **UI Layer**          | Dear ImGui                               |
| **Lighting**          | Blinn–Phong model                        |
| **Physics (planned)** | Custom lightweight solver                |
| **Platform**          | Windows & MacOS                          |
