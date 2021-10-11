# ZeroVulkan

## Render API using Vulkan as backend

### Goal
* abstracts the use of vulkan (zero vulkan for the user basicly)
* cross-platform compatible
* cache friendly
* for me to learn Vulkan more 
* to use it later in other projects

### Features
* [x] Linux support
* [ ] OSX support
* [ ] Windows support
* [ ] Shaders
  * [x] Vertex
  * [x] Fragment
  * [ ] Compute (not tested)
* [ ] create window
  * [x] on Linux with X11(xcb)
  * [ ] other platforms 
* abstraction of renderable objects
  * [x] ZObject (3D)
  * [ ] ZObject2D
  * [ ] Premitives
    * [ ] line
    * [ ] rect
    * [ ] circle
    * [ ] cube
    * [ ] sphere
 
### Basics
> Your main.cpp
```cpp
int main () {
   ZeroVulkan::ZProject test;
   ZeroVulkan::ZScene::create<TestScene>();
   test.run();
   return 0;
}
```
> create a scene
```cpp
class TestScene : public ZeroVulkan::ZScene 
{
public:
    TestScene();
    
    virtual void start() override; 
    virtual void update(float dt) override;
    virtual void end() override;
};
```
> create a mesh
```cpp
void TestScene::start() {
// ...

   // simple cube
   float vertices[] = {
      0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
      -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,

      0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
   };
        
   uint32_t indices[] = {
      0, 1, 2,
      2, 3, 0,

      5, 1, 0,
      0, 4, 5,

      6, 3, 2,
      6, 7, 3,

      5, 2, 1,
      5, 6, 2,

      0, 3, 4,
      7, 4, 3,

      4, 7, 6,
      6, 5, 4
   };
    
   ZMesh mesh(vertices, sizeof(vertices)/sizeof(float), indices, sizeof(indices)/sizeof(uint32_t));
// ...
}
```
> create Shaders
```cpp
ZShaderSet shaders("Test/shader/phong.vert", "Test/shader/phong.frag");

// setup Uniform ------------------------------
mat4* proj = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
mat4* view = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
mat4* model = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
vec3* lightDir = std::any_cast<vec3>( shaders.uniformLayout.addComponent(ZType::VEC3) );
```
