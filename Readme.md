# Plant Drawing Using L-Systems

*The complete documentation can be found [here](./doc/Overview.html)*

This project was made as a part of the Computer Graphics course (IS F311) offered at BITS Pilani, Hyderabad Campus. It features graphical rendering of trees using Lindenmayer Systems. The trees are composed entirely of straight lines and circles, which are themselves drawn using Bresenham's Midpoint Algorithm.

The project is written entirely in C++, using the Open Graphics Library (OpenGL) for all graphical rendering needs. Certain third party libraries which complement OpenGL are also used, consisting of but not limited to [GLFW](http://www.glfw.org/) and [GLAD](http://glad.dav1d.de/).

We faced many design and implementation challenges while working on this project, the major roadblocks being:
* **Rendering using OpenGL:** The mere setup of something as new as a graphics library took us a while. The concepts of fragment shaders, vertex shaders, and the limited -1 to 1 coordinate system of OpenGL took some time to understand.
* **Working with headers and classes in C++:** Having used C-style languages only for single file algorithmic problems before, it was a major challenge writing an entire project in C++, where OOP paradigms are a little harder to implement than say, in Java or Python.
* **Optimizations to prevent long processing times**

The overall time complexity of the preprocessing phase is mostly due to the string expansion algorithm, and is approximately **O(k ^ n)**, where **k** is the number of rewritable letters ('T' in our strings) in the rewritten string, and **n** is the number of iterations to be processed (4 in our case).

The time complexity during the program runtime (while changing the iteration to display) is linear in the number of coordinates to render, i.e., **O(v)**, where **v** is the number of vertices to be rendered.

The program can be evaluated by running the .exe file in the root folder of the project (Visual Studio and a graphics card supporting OpenGL 4.0 or higher are required).

**Project Authors: Asutosh Sistla, Kushal Agrawal, and Suchit Kar**