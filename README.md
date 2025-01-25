# Sorting Algorithms and Graph Visualizer

An interactive educational tool built in C++ using the Qt framework. This project provides intuitive visualizations of sorting algorithms and graph traversal techniques, making it an excellent resource for students, educators, and developers.
> This project was part of our `Data Structures and Problem Solving` course in semester `Fall 2024` at `Faculty of Engineering, Ain Shams University`.

We are honored that our professor recognized the value of this project and asked us to add support for macOS so he could use it as a teaching tool for our fellow colleagues. This acknowledgment underscores the educational potential of our Sorting and Graph Visualizer. ⭐

---
## Table of Contents

- [Sorting Visualizer](#sorting-visualizer)
  - [Features](#features)
- [Graph Visualizer](#graph-visualizer)
  - [Features](#features-1)
- [Technologies Used](#technologies-used)
- [Installation](#installation)
  - [Option 1: Download From Releases](#option-1-download-from-releases)
  - [Option 2: Build from Source](#option-2-build-from-source)
- [Usage](#usage)
  - [1. Launch the Application](#1-launch-the-application)
  - [2. Sorting Visualizer](#2-sorting-visualizer)
  - [3. Graph Visualizer](#3-graph-visualizer)
- [Algorithms Supported](#algorithms-supported)
  - [Sorting Algorithms](#sorting-algorithms)
  - [Graph Algorithms](#graph-algorithms)
---

### Sorting Visualizer
<img src="resources/media/Sorting%20Showcase.png" alt="Sorting Showcase" width="95%">

### Features
- 🎥 **Interactive Visualization**: Real-time visualization of sorting algorithms.
- 🔄 **Sorting Order Selection**: Sort in ascending or descending order.
- 🔢 **Element Count Control**: Adjust the number of elements in the array.
- ⏯️ **Step Control**: Choose continuous playback or step-by-step progression.
- 📊 **Algorithm Statistics**: View real-time metrics such as array access count, comparisons count, and algorithm complexity.
- 🎨 **Themes and Sound Effects**: Customize the visual theme and choose fun sound effects.
- ⏱️ **Delay Control**: Adjust the delay between steps.
- 🖥️ **Resizable Window**: Dynamically adjust window size or enable full screen mode for an immersive visualization experience.
- ⌨️ **Keyboard Shortcuts**: The program responds to various keyboard shortcuts such as:

  - **F11** or **F** to switch to full-screen mode.
  - **ESC**, **F11**, or **F**, to switch out of full-screen mode.
  - **M**, to mute; sets **Sound** to **No Sound**.
  - **G**, to switch to **Graph Visualizer**.
  - **S**, to switch to **Sorting Visualizer**.
  - **Space**, to pause sorting, then resume again with space as-well.
  - **Next Arrow Key**, to trigger next step. **Only for _Step-by-step_ mode**.
---

### Graph Visualizer
<img src="resources/media/Graph%20Showcase.png" alt="Graph Showcase" width="95%">

### Features
- **Interactive Graph Drawing**: Create and connect nodes interactively.
- **Traversal Algorithms Visualized**:
  
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra’s Algorithm
- **Path Highlighting**: Visually follow the explored path during execution.
- **Customizable Weights**: Dynamically set weights for edges.
- **Node Configuration**: Define start and end nodes interactively.

## Technologies Used
- **Programming Language**: C++
- **UI Framework**: [Qt Creator](https://www.qt.io/product/development-tools)
- **Sound Effects**: [SDL2](https://www.libsdl.org/)

## Installation

### Option 1: Download From Releases  
- Navigate to the [Releases](https://github.com/adamt-eng/sorting-visualizer/releases) page.

#### For Windows:
   - Download the file: `sorting-visualizer-win32-x64.zip`.
   - Extract the file.
   - Run `sorting-visualizer.exe` to start using the application.

#### For macOS:
   - Download the file: `sorting-visualizer.dmg`.
   - Open the file.
   - Run `sorting-visualizer.app` to start using the application.
   - Note: You may need to navigate to `System Preferences > Security & Privacy > General` and click "Open Anyway" to allow the application to run.
   
### Option 2: Build from Source

Follow these steps to build and run the project using Qt Creator:
1. Download and install [Qt Creator](https://www.qt.io/product/development-tools).
2. Run Qt Creator and select **Create New Project**.
3. Choose **Import Project** and then select **Git Clone**.
4. Paste the repository link:
      
```bash
  https://github.com/adamt-eng/sorting-visualizer
```
6. After cloning, select all kits and click **Configure Project**.
7. Build and run the project directly from Qt Creator.
## Usage

### 1. **Launch the Application**
   - Run the application. By default, the **Sorting Visualizer** will load first.

---

### 2. **Sorting Visualizer**
   - **Choose an Algorithm**: Select the sorting algorithm you want to visualize from the dropdown menu.
   - **Set Order of Sorting**: Decide whether the elements should be sorted in ascending or descending order.
   - **Adjust Elements Count**: Modify the number of elements.
   - **Playback Control**:
     - Choose **Continuous** for automatic progression through sorting steps.
     - Use **Step-by-Step** to manually trigger each sorting step for detailed analysis.
       
       > ℹ️ **Hint:** 
       > You can use your keyboard arrow key to trigger the next step!
   - **Modify Delay**: Adjust the delay between sorting steps to slow down or speed up the visualization.
   - **Statistics Text Size**: Customize the size of text displaying statistics such as comparisons and accesses for better readability and accessibility.

---

### 3. **Graph Visualizer**
   - To use the Graph Visualizer, switch to it from the **Switch To Graph Visualizer** button, or use the **G** key on your keyboard for a shortcut!
   - **Create Nodes**: Create nodes by clicking anywhere on the graphics view.
   - **Connect Nodes**: Connect the nodes by clicking on the node you want the currently selected node to be connected to.
   - **Specify Weights**: You can specify the weight of an edge when connecting two nodes together.
   - **Define Start & Goal**: Define the number of the start and end nodes.
   - **Select Algorithm**: Select any of our traversal algorithms (BFS, DFS, Dijkstra) then start!


## Algorithms Supported

### Sorting Algorithms
- Bubble Sort
- Merge Sort
- Quick Sort
- Insertion Sort
- Selection Sort
- Counting Sort
- Radix Sort
- Heap Sort
- Cocktail Sort
- Gnome Sort
- Shell Sort
- Bogo Sort (not recommended)
- Stalin Sort (for educational purposes)

---

### Graph Algorithms
- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Dijkstra’s Algorithm
