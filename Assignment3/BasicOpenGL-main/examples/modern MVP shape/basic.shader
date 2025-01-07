#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // Unused, but still part of input
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);
    v_TexCoord = texCoord;
}

#shader fragment
#version 330

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

// Static colors for cube faces
vec4 faceColors[6] = vec4[6](
    vec4(1.0, 0.0, 0.0, 1.0), // Red for one face
    vec4(0.0, 1.0, 0.0, 1.0), // Green for another
    vec4(0.0, 0.0, 1.0, 1.0), // Blue for another
    vec4(1.0, 1.0, 0.0, 1.0), // Yellow for another
    vec4(1.0, 0.0, 1.0, 1.0), // Magenta for another
    vec4(0.0, 1.0, 1.0, 1.0)  // Cyan for another
);

void main()
{
    // Determine face color based on the texture coordinate region (example logic)
    // Replace this logic based on actual face ID from cube data
    int faceIndex = int(v_TexCoord.x * 6.0); // A placeholder heuristic
    faceIndex = clamp(faceIndex, 0, 5);      // Ensure index is within range
    FragColor = faceColors[faceIndex];
}
