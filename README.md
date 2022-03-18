# ZeroVulkan

## CrossPlatform Game Engine using Vulkan as backend

### Goal
* simple game engine
* abstracts the use of vulkan (zero vulkan for the user basically)
* cross-platform compatible
* cache friendly
* to learn: Vulkan, Physics and game engines

### Features
* [x] Linux support
* [ ] OSX support
* [ ] Windows support
* [ ] Scenes
  * [x] create scenes
  * [ ] switch scenes
* [ ] Input
  * [ ] Mouse
  * [x] Keyboard
* [x] Camera
  * [x] Third Person
  * [x] First Person
  * [x] Sandbox Camera
* [ ] Renderer
  * [ ] Shaders
    * [x] Vertex
    * [x] Fragment
    * [x] Compute
    * [ ] parse shader (still in work)
  * [ ] create window
    * [x] on Linux with X11(xcb)
    * [ ] other platforms 
  * [ ] abstraction of renderable objects
    * [x] ZObject (3D)
    * [ ] ZObject2D
    * [ ] FragShader (fullscreen fragment shader)
    * [ ] Premitives
      * [ ] line
      * [x] rect
      * [ ] circle
      * [ ] cube
      * [ ] sphere
 
### Requirements
* vulkan drivers
* vulkan lib
* glslang support (to compile shader)
  
* Linux:
  * sudo pacman -S glslang vulkan-headers vulkan-validation-layers
 
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
> create premitives
```cpp
void TestScene::start() {
// ...
                  // position    width / height            color
    createRect(vec2(0.5f, 0.5f), 0.25f, 0.25f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
// ...
```
> create a mesh
```cpp
void TestScene::start() {
// ...

   // simple cube
   float vertices[] = {
       // vertex coords        // color
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
> create shaders
```cpp
ZShaderSet shaders("Test/shader/phong.vert", "Test/shader/phong.frag");
```
> create a 3D object (with default shader (phong shader))
```cpp
// ...
// create mesh
ZObject& obj = createObject(mesh);
```
> create a 3D object (with custom shader)
```cpp
// ...
// create shaders and mesh
ZObject& obj = createObject(shaders, mesh);

// get components from your uniform(s)
mat4* proj     = obj.getUniformComponent<mat4>(0);
mat4* view     = obj.getUniformComponent<mat4>(1);
mat4* model    = obj.getUniformComponent<mat4>(2);
vec3* lightDir = obj.getUniformComponent<vec3>(3);

// set default values
*model = mat4(1.f);
*lightDir = vec3(-1.f, -1.f, 3.f);

// set the matrices for 3D rendering so they will be automatically updated
obj.set3DMats(proj, view, model);
```
