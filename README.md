# Grandpas ray tracer :older_man:
This IS your grandpas ray tracer! No abstract classes or polymorphism here, just union types and switch cases. This project is heavily based on [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html). This project might as well have been written in C but I am more comfortable in C++ and some STL functionality like vectors and futures are quite useful. There still exists some artifacts in the images and there is definitely room for improvement but I really enjoyed working on this project.

## How to run
- Clone the repo
- Download [GLM](https://github.com/g-truc/glm)
- Add GLM to the include directories in the Visual Studio solution
- Change to your desired scene in scene_utilities.cpp
- Adjust your camera settings in create_image.cpp
- Run the program
- View your output in output.ppm

## This project includes
- Sphere geometries
- Symmetric box geometries
- Lambertian material
- Metal material
- Dielectric material
- Asynchronous processing of pixels
- Movable camera
- Depth of field
- Field of view
  
## Possible improvements
- Lighting
- Procedural materials
- Benchmark performance compared to traditional object oriented ray tracer
- Be able to build and run project using CMake
  
## What I've learned
- There is definitely a use case for abstract classes and polymorphism. I noticed pretty quickly that, for example, tight coupling between materials and geometries very quickly led to repeated code and further expansion of materials and geometries would probably become difficult and confusing.
- Asynchronous programming is hard. And that I don't have as much control as I initially thought over CPU cores etc.
- C++ is a language that allows you to approach a project and structure your solution in essentially any way you want, which is one of the reasons why I enjoy the language.

## Images
All images were rendered on a laptop on a AMD Ryzen 7 5800U with Radeon Graphics (1.90 GHz).

![alt](https://i.imgur.com/EpoUfYK.png)
Render time: 00:17:40

![alt](https://i.imgur.com/sj2nBBe.png)
Render time: 00:15:53

![alt](https://i.imgur.com/bRj6NUR.png)
Render time: 00:11:14

![alt](https://i.imgur.com/A9MjbiU.png)
Render time: 00:03:53
