### DreamsEngine


Still work in progress and i'm still in the process of learning both Vulkan and C/C++.

## The direction i want to take for this engine 
This engine should be able to support ray-tracing and other modern techniques both for visuals and optimization. Also this engine will definitely have an editor and ideally it will support scripting in multiple languages, such as: C++, Rust, Zig and/or some higher level languages like Lua.

## Disclaimer
I can't promise to deliver all the features i have in mind since i'm still an amateur but the idea is to make the engine an actually good engine capable of being used for serious projects.

## Building the engine
The project uses xmake as its build system and dependency manager since it's less of a hassle compared to standard CMake and also because i wanted to give it a try...

To build and run the engine just clone the repository and run:
```
git clone https://github.com/ByteAfterByte/DreamsEngine.git ~/DreamsEngine
cd ~/DreamsEngine
xmake run
```
