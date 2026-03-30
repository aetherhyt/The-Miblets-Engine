# The Miblets Engine

This ZIP contains a minimal real-code bootstrap for:
- C++20
- GLFW window creation
- Vulkan instance/device/swapchain setup
- Clear-color render loop
- AGENTS.md for future engine direction

## Build

```bash
cmake --preset windows-vs2022-x64
cmake --build --preset build-windows-debug
```

## Next planned step
- Graphics pipeline
- Triangle
- Vertex buffers
- Camera
- NoesisGUI integration
