# ZeroVulkan

## 3D render API using Vulkan as backend

### Goal
* abstract the use of vulkan (zero vulkan for the user basicly)
* Crossplatform compatible (for now only Linux)
* for me to learn Vulkan more and to use it later in other projects

### Features
* VertexShader, FragmentShader, ComputeShader (not tested)
* uses X11(xcb) to create a window
* create a ZObject (abstraction of a renderable Object)
    * contains Shader, UniformBuffer, Vertices, Indices, ...

### Todo
* Refectoring!!!!
* draw primitives on the fly
