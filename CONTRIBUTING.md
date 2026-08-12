# Contributing to Terminal Dungeon Crawler

First off, thank you for considering contributing to this project! Whether you are fixing a bug, adding a new mob, or expanding the mechanics your help is appreciated.

## 🛠️ How to Contribute

1. **Fork the repository** to your own GitHub account.
2. **Clone the project** to your local machine.
3. **Create a new branch** for your feature or bug fix: `git checkout -b feature/your-feature-name`
4. **Make your changes** and test them locally.
5. **Commit your changes** with a clear, descriptive commit message.
6. **Push the branch** to your fork: `git push origin feature/your-feature-name`
7. **Open a Pull Request** against the `main` branch of this repository.

## 💻 Code Guidelines

To keep the engine's architecture clean and maintainable, please follow these guidelines:

* **Architecture:** Check `UML.png` in `documentation/` to understand the architecture of the game.
* **C++ Standard:** This project is built using **C++17**. Ensure any new syntax or standard library features you use are compatible.
* **Header/Source Separation:** All class declarations (`.h` files) must go in the `include/` directory. All implementations (`.cpp` files) must go in the `src/` directory.
* **Class Relationships:** If you are adding a new Entity, Item, Room, Spell or Skill, please inherit from the existing base classes rather than creating standalone structures. When making a derived class method, make sure if the base class has a virtual method that you can override.
* **Memory Management:** This project does not use vectors, I specifically made it use my own `DynamicArray.h` in order to manually manage the memory. So whatever you do, remember to check for memory leaks.
* **Adding entities or mechanics:** The game lacks a variety of enemies, so you may derive from the `Enemy` class and make your own enemy type. Same with items like weapons, potions and armor. 
* **Game Balance:** You are welcome to tune the damage and health values for existing entities and items as long as they are balanced; current values were very tentative.
* **Story:** You may expand the story on your own as I did not have the time to make it a whole story game.
* **Serialization:** Any class that needs to be serialized must also inherit from Serializable in `Serializable.h` and override their Serialize and Deserialize methods.
* **CMake Build:** If you add new source files to the `src/` directory, the existing `CMakeLists.txt` is configured to pick them up automatically using `file(GLOB)`. You do not need to modify the CMake file unless you are adding new dependencies or entirely new directories.

## 🧪 Testing Your Changes

Before submitting a Pull Request, please verify that:
1. The project compiles successfully on your machine using the provided CMake or Makefile setup.
2. The core gameplay loop remains stable.
3. **Data Serialization:** If you modified any entity stats or map data, verify that the game can successfully save to a `.json` file and load back into that save state without crashing or corrupting.

If you have any questions about the codebase, feel free to open an Issue to ask!
