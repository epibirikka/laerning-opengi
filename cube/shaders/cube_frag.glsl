#version 330 core

in vec2 textcoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float timer;

void main()
{
	vec2 t2_coord = textcoord + vec2(
		0.0f, sin((timer * 5) + (12 * textcoord.y)) * 0.16f
	);

	gl_FragColor = mix(texture(texture1, textcoord), texture(texture2, t2_coord), 0.3);
}
