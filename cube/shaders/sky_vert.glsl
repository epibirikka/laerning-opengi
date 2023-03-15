#version 330 core

in vec3 pos;
in vec2 inpTexCoords;
out vec2 fragTexCoords;

void main()
{
	gl_Position = vec4(pos, 1.0f);
	fragTexCoords = inpTexCoords;
}
