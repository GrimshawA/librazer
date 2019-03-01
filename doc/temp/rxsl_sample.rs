
struct attributes
{
    [location {0}]
    vec4 pos;
    vec4 color;
    vec2 uv;
}

uniform struct
{
    world : mat4
    view : mat4
    projection : mat4

    let wvp: world * view * projection;
}

[vertex]
func main(attributes attr) : vec4
{
    return wvp * attr.pos;
}