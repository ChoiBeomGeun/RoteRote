#version 330

in vec4 verParticlePos;
in vec4 verParticleCol;
in vec2 verParticleUV;

uniform mat4 particlemodel;
uniform mat4 particleview;
uniform mat4 particleprojection;
uniform int particleStats;


out vec4 fragParticlePos;
out vec4 fragParticleCol;
out vec2 fragParticleUV;

// this is for particle fragmentation
void main()
{
	if(particleStats == 3)
		gl_Position = particleprojection * particleview * particlemodel * vec4(verParticlePos.xyz, 1.0);
	else
		gl_Position = vec4(verParticlePos);

	fragParticlePos = verParticlePos;
	fragParticleCol = verParticleCol;
	fragParticleUV = verParticleUV;
}
