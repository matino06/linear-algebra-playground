#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    FragPos = vec3(mMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(mMatrix))) * aNormal;

    gl_Position = pMatrix * vMatrix * vec4(FragPos, 1.0);
}
