#version 330

out vec4 color;

in vec4 fragParticlePos;
in vec4 fragParticleCol;
in vec2 fragParticleUV;

uniform sampler2D ptexture2D;
uniform int ptexturing;
uniform vec4 pcolorOffset;
uniform int particleStats;

void main()
{
	
	vec4 texel = texture(ptexture2D,fragParticleUV );
	if(texel.a < 0.2)
		discard;
			
	vec4 tempcolor = fragParticleCol * pcolorOffset;
	
	if(particleStats == 3)
	{
		if(ptexturing == 1)
			color = texel * tempcolor; // texel , texture(texture2D,fragParticleUV)
		else	 
			color = texel * tempcolor;
	}
	else
		color = texel * fragParticleCol;
}