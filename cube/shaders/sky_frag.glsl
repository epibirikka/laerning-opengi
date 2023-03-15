#version 330 core

in vec2 fragTexCoords;

uniform sampler2D texture0;
uniform float timer;

void main()
{
	gl_FragColor = texture(texture0, fragTexCoords + vec2(cos((fragTexCoords.y * 10) + (timer * 9)) * .005, sin((fragTexCoords.x * 20) + (timer * 4)) * .0125)) * 
					vec4(0.5 + (cos(fragTexCoords.x + (timer * 1)) * 0.5), 0.5 + (sin((fragTexCoords.y * 2) + (timer * 0.5)) * 0.5), 0.5 + (sin(timer * 2) * 0.5), 1.0f);
}
