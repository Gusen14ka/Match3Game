# Match3Game

A simple Match-3 puzzle game written in C++ using SFML and CMake.  
Swap adjacent tiles to form horizontal or vertical lines of three or more matching colors, clear them, and watch cascading collapses. Special “bonus” tiles occasionally appear with animated pop-in effects.

---

## 🕹️ Features

- **Classic Match-3 mechanics**: swap, match, remove, collapse.  
- **Bonus tiles**: Bombs and Painters appear randomly and trigger animated effects.  
- **Smooth animations**: swap, collapse and bonus effects are all driven by a lightweight `AnimationManager`.  
- **Modular architecture**: clear separation between `Game` (state machine), `Board` (logical grid), `Tile` (rendered cell), and `Animations`.

---

## 🛠️ Prerequisites

- **C++20** compiler (MSVC / Clang / GCC)  
- [CMake ≥ 3.15](https://cmake.org/)  
- [vcpkg](https://github.com/microsoft/vcpkg) in **manifest** mode with:
  ```json
  {
    "dependencies": [
      "sfml",
      "tgui"
    ]
  }
  ```
- Windows only: WinAPI used for exe‐path resolution (can be adapted for Linux/Mac).

---

## 📦 Building

1. **Clone** this repository:
   ```bash
   git clone https://github.com/<you>/Match3Game.git
   cd Match3Game
   ```
2. **Bootstrap** and integrate vcpkg (if not already):
   ```bash
   cd ../vcpkg
   git pull
   .\bootstrap-vcpkg.bat
   cd ../Match3Game
   ```
3. **Generate** and **build** with CMake:
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build build --config Debug
   ```
   This will produce:
   ```
   build/Debug/Match3Game.exe
   build/Debug/Includes/Arial.ttf
   ```
   (Similarly for `Release`.)

---

## 🚀 Running

From the build folder:
```bash
cd build/Debug        # or build/Release
./Match3Game.exe
```
Ensure the `Includes/` folder (with `Arial.ttf`) sits next to the executable.

---

## 🎮 Controls

- **Click** a tile, then click an adjacent tile to swap.  
- If swap creates a match (3+ tiles), they scale-fade out → collapse → new tiles fall in.  
- **Bonus tiles** (`B` = Bomb, `P` = Painter) pop in with a fade animation:
  - **Bomb**: clears up to 5 tiles in a small radius.  
  - **Painter**: repaints several random tiles.

---

## 📂 Project Structure

```
Match3Game/
├── Includes/             # Assets (fonts, sprites, etc.)
│   └── Arial.ttf
├── src/
│   ├── Game.{cpp,h}      # main state machine, input & render loop
│   ├── Board.{cpp,h}     # logical grid, match detection, collapse logic
│   ├── Tile.{cpp,h}      # single cell rendering & state
│   ├── Pos.h             # row/col coordinate type
│   ├── Bonuses/          # bonus-generation & application strategies
│   └── Animations/       # Animation base + Move, Scale, BonusAnimation, Manager
├── CMakeLists.txt        # project build definition
└── README.md
```

---

## 🤝 Contributing

1. Fork this repo  
2. Create a feature branch:
   ```bash
   git checkout -b feature/YourFeature
   ```
3. Commit your changes:
   ```bash
   git add .
   git commit -m "Add awesome feature"
   git push origin feature/YourFeature
   ```
4. Open a Pull Request and describe your work.

---

Enjoy matching!
