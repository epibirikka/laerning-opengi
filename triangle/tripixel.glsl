#version 330 core

uniform float makeItRainbow;

void main()
{
	if (makeItRainbow > 0.5f)
	{
		gl_FragColor = vec4(gl_FragCoord.x/800, 0.25+min(gl_FragCoord.y/1600, 0.5), gl_FragCoord.x/1500, 1.0f);
	}
	else
	{
		gl_FragColor = vec4(0.65, 0.0, 0.0f, 1.0f);
	}
}
