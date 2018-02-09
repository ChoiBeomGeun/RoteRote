#version 330

out vec4 color;

in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

uniform sampler2D texture2D;
uniform int texturing;

void main()
{
	
	vec4 texel = texture(texture2D,fragmentUV );
	if(texel.a < 0.5)
		discard;

	if(texturing ==1)
	color = texel * fragmentColor; // texel , texture(texture2D,fragmentUV)
	else	 
	color = fragmentColor;
}

