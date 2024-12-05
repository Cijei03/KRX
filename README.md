# KRX

![alt text](https://cijei03.github.io/resources/krx_logo.png)

## What is KRX?

Welcome in KRX repository! It is my own C++ API for rendering 3D graphics. All computes are performed on CPU.<br><br>

## Performance
![alt text](https://cijei03.github.io/resources/hello_triangle.png)

### Rendering simple triangle with barycentric interpolation on:<br>
| CPU | OS | Environment | KRX API |
| ---- | --- | --- | -- |
| Intel Core i7-4770K @ 4400MHz | Linux Kubuntu 22.04 LTS | X11 | 0.40.0 |

Produces in peak about 400 FPS. Furthermore it consumes only 6MB of RAM memory!


## Features
### Pipeline features
- depth testing
- blending
- validation layer
- programmable shaders
### Shader models capabilities

| Shader model version | krxSM1 |
| -------------------------- | --- |
| KRX API | 0.40.0 |
| Supported attributes count | 8 |
| Fragment shader outputs count | 1 |
| Uniform buffers count | 4 |
| SV_POSITION | Yes |
| SV_VERTEXID | Yes |
| SV_PIXELCOORD | No |

Remember that shader models are not backward compatible. It means newer shader model will not work on older shader model.

### Shader models arithmethical types
  | Shader model version | krxSM1 |
| -------------------------- | --- |
| f16vec4 | Yes |
| f16vec3 | Yes |
| f16vec2 | Yes |
| f32vec4 | Yes |
| f32vec3 | Yes |
| f32vec2 | Yes |
| ui32vec4 | No |
| ui32vec3 | No |
| ui32vec2 | No |
| ui32vec4 | No |
| i32vec4 | No |
| i32vec3 | No |
| i32vec2 | No |
| i32vec4 | No |
| f16mat2 | No |
| f16mat3 | No |
| f16mat4 | No |
| f32mat2 | No |
| f32mat3 | No |
| f32mat4 | No |
| ui32mat2 | No |
| ui32mat3 | No |
| ui32mat4 | No |
| i32mat2 | No |
| i32mat3 | No |
| i32mat4 | No |
