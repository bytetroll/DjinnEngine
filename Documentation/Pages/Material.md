Material Documentation WIP

# Custom Layouts

### Shader code

In order to describe incoming data, a class with name `MaterialVertexInput` must be defined, example:

```cpp
class MaterialVertexInput {
    @Attribute("vertex", 0) float3 position;
    @Attribute("vertex", 0) float3 normal;
    @Attribute("vertex", 0) float3 tangent;
    @Attribute("vertex", 0) float2 texcoord;
}
```

Where another class with name `MaterialVertexData` defines the transformed data, example:

```cpp
class MaterialVertexData {
    this();
    @Semantic("position") float4 position;
    float4 worldPosition;
    float3 worldNormal;
    float3 worldTangent;
    float2 texcoord;
}
```

However this class must define the following callbacks:

```cpp
class MaterialVertexData {
    ...

    float3 GetPosition();
    float3 GetNormal();
    float3 GetTangent();
    float2 GetTexcoord();
}
```

In order to transform the data, a function by name `TransformVertexData` must be defined with the following signature:

```cpp
MaterialVertexData TransformVertexData(MaterialVertexInput input);
```

# Custom Types

### Header

All data needs to be organized within buckets, where the `material` bucket is required to be defined:

```cpp
scope Buckets {
    static int material = 0;
}
```

The `Data` scope must define the following callbacks:

```cpp
scope Data {
    float4x4 GetView();
    float4x4 GetProjection();
    float4x4 GetViewProjection();
    float4x4 GetWorld();
}
```

### Kernel

The kernels are the actual shaders, which must be annotated with the `Kernel` attribute, example:

```cpp
// Vertex Interop
struct VertexInterop {
    this();
    __returnof(_vertex) user;
}

// Vertex kernel
@Kernel("vertex")
VertexInterop vertexKernel(__argof(_vertex) vin) {
    auto interop = VertexInterop();
    interop.user = _vertex(vin);
    return interop;
}

// Pixel kernel
@Kernel("pixel")
float4 pixelKernel(VertexInterop interop) {
    auto raw = _pixel(interop.user).AsRaw();
    return float4(raw.diffuse, 1.0f);
}
```

# Example

```cpp
__include("Material.dsf");

// Resources
@MaterialResource extern Types.Texture2D:float4 source;

// Vertex data
struct Vertex {
    // Layout data
    MaterialVertexData data;
}

// Vertex shader
Vertex __vertex(MaterialVertexInput input) {
    auto vertex = Vertex();
    vertex.data = TransformVertexData(input);
    return vertex;
}

// Pixel shader
Surface __pixel(Vertex vertex) {
    auto surface = Surface(vertex.data);
    surface.albedo = source.Sample(vertex.data.GetTexcoord());
    return surface;
}
```