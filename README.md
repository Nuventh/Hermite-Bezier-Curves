# Hermite and Bezier Curves

This project is a simple OpenGL program that lets users draw **Hermite** and **Bezier curves** by clicking on the window to place control points. The program allows switching between Hermite and Bezier curve views using keyboard inputs.

## Features
- **Hermite Curves**: Displayed with user-supplied points and computed tangent vectors.
- **Bezier Curves**: Displayed with user-supplied control points.
- Switch between **Hermite** and **Bezier** modes using the keyboard.
- Click to add up to 12 control points.

## Controls
- **Left Mouse Click**: Add control points (up to 12 points).
- **Press 'H'**: Switch to Hermite curve view.
- **Press 'B'**: Switch to Bezier curve view.

## Dependencies

- [OpenGL](https://www.opengl.org/)
- [GLUT](https://www.opengl.org/resources/libraries/glut/)

Make sure you have the required libraries installed to build and run this project.

### Installing GLUT (Linux example)

For Ubuntu or other Debian-based systems, you can install the necessary libraries using:

```bash
sudo apt-get update
sudo apt-get install freeglut3 freeglut3-dev libglew-dev
```
