#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 FragPos[];
in vec3 Normal[];

out vec3 gFragPos;
out vec3 gNormal;

void main()
{
    vec3 faceNormal = normalize(cross(FragPos[1] - FragPos[0], FragPos[2] - FragPos[0]));

    for (int i = 0; i < 3; i++)
    {
        gFragPos = FragPos[i];
        gNormal = faceNormal; // Use face normal for flat shading
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
