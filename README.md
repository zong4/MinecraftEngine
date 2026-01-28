# Minecraft Engine

[![C++](https://img.shields.io/badge/Language-C++-00599C?logo=c%2B%2B)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](./LICENSE)
[![Status](https://img.shields.io/badge/Build-Release-blue)](https://github.com/zong4/MinecraftEngine/releases)

<p align="center">
  <img src="./Assets/Logo.png" alt="Logo" width="200">
</p>

## What we want?

This engine is designed specifically for **Minecraft-like voxel games**, focusing on performance, chunk management, and real-time procedural generation — without the overhead of general-purpose engines like Unity or Unreal.

By narrowing the engine’s scope, development can target **core voxel mechanics** (terrain, lighting, rendering, and physics) with maximum efficiency.

## What we have?

![Editor](./Assets/Editor.png)

- [✔] Input handling for keyboard and mouse interactions.
- [✔] Render system optimized for rendering basic voxel primitives (cubes).
- [✔] UI layer built with ImGui for in-engine tools and debugging. 
- [✔] ECS architecture for modular game object management.
- [✔] Physic system with basic collision detection and response.
- [✔] Basic particle system for visual effects.
- [✔] Basic audio system for sound effects and background music.
- [✔] Collider system with bvh tree for basic ray tracing.
- [✔] Support basic lua scripting for game logic and modding.

## What is next?

- [ ] **Procedural Generation Algorithms** — Perlin Noise, Simplex Noise, and hybrid terrain methods.  
- [ ] **World Generation Techniques** — Chunked LOD, Greedy Meshing, seamless world streaming.  
- [ ] **Physics-Driven Animation** — ragdoll, procedural motion, and dynamic interaction.

## How to run?

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

> xmake will automatically download and compile all required dependencies on first run.
