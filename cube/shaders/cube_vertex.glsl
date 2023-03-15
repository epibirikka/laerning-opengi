#version 330 core

in vec2 inpTextcoord;

in vec3 pos;
out vec2 textcoord;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
	gl_Position = projection * transform * vec4(pos, 1.0f);

	textcoord = inpTextcoord;
}
