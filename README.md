# Grandpas ray tracer :older_man:
This IS your grandpas ray tracer! No abstract classes or polymorphism here, just union types and switch cases. 

In this project I've picked my favorite parts from [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html), [Ray Tracing: The Next Week](https://raytracing.github.io/v3/books/RayTracingTheNextWeek.html) and [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/v3/books/RayTracingTheRestOfYourLife.html). The contents have been implemented in my own way, not exactly like it is implemented in the books.

This project might as well have been written in C but I am more comfortable in C++ and some STL functionality like vectors and futures are quite useful. There still exists some artifacts in the images and there is definitely room for improvement but I really enjoyed working on this project! Let me know in the comments if you have an idea for improvement or anything else!

## How to run using CMake
- Clone the repo
- Download [GLM](https://github.com/g-truc/glm)
- Download [CMake](https://cmake.org/download/) and add it to `PATH`
- Set an environment variable `GLM_DIR` to the include path for GLM `<your_download_directory>/glm-master/glm`
- From the root level in the project directory, the level with CMakeLists.txt, run the following commands:
```
mkdir build
```
```
cd build
```
```
cmake ../
```
```
cmake --build .
```
```
cd Debug
```
```
.\grandpas_ray_tracer_exe.exe
```
Last command to run the executable may differ depending on which OS you are using.

## How to run using Visual Studio
- Clone the repo
- Download [GLM](https://github.com/g-truc/glm)
- Open the solution and add the include path for GLM `<your_download_directory>/glm-master/glm` to the VC++ include directories
- Run the program

## Creating your own scenes
To select which scene is rendered, change the function call in scene_creation.cpp. You can also create your own scenes using the helper functions. All camera settings can also be changed from default in scene_creation.cpp. 

## This project includes
- Sphere geometries
- Symmetric box geometries
- Asymmetric box geometries
- Constant density mediums
- Lambertian material
- Metal material
- Dielectric material
- Lighting
- Colored lights
- Caustics
- Asynchronous processing of pixels
- Movable camera
- Depth of field
- Field of view
  
## Possible improvements
- Bounding volume hierarchy
- Procedural materials
- Benchmark performance compared to traditional object oriented ray tracer
  
## What I've learned
- There is definitely a use case for abstract classes and polymorphism. I noticed pretty quickly that, for example, tight coupling between materials and geometries led to repeated code and further expansion of materials and geometries would probably become difficult and confusing.
- Asynchronous programming is hard. And that I don't have as much control as I initially thought over CPU cores.
- C++ is a language that allows you to approach a project and structure your solution in essentially any way you want, which is one of the reasons why I enjoy the language.
- Gaussian filtering is a useful method to achieve a similar effect to multi-sampling while using less computational resources. Obviously there has to be a balance between actual multi-sampling and gaussian filtering but it allows for using few samples per pixel and achieving an adequate result.

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
