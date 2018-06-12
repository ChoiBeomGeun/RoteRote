#version 330

in vec4 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//layout (location = 2) in vec4 color;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int drawingStatus;
uniform mat4 hudmodel;
uniform bool isAnimation;
uniform int flipx;
uniform int isJumping;
uniform float animationx;
uniform float timer;
//uniform vec2 uv;

out vec4 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

void main()
{
	
	vec2 UV = vec2(1.0f, 1.0f);
	if(isAnimation)
	{
		float abc = float(vertexUV.x * timer);
		if(isJumping == 1)
		{
			//Animation(flipx);
			if (flipx == 1)
			{
				abc = float((1.0f - vertexUV.x) *timer);
				UV = vec2(abc + animationx , vertexUV.y);
			}
			else if (flipx == 0)
			{
				abc = float(abc + animationx);
				UV = vec2(abc, vertexUV.y);
			}
		}
		else if(isJumping == 0)
		{
			if (flipx == 1)
			{
				abc = float((1.0f - vertexUV.x) *timer);
				UV = vec2(abc + animationx , vertexUV.y);
			}
			else if (flipx == 0)
			{
				abc = float(abc + animationx);
				UV = vec2(abc, vertexUV.y);
			}
		}
	}
	else
	{
		UV = vertexUV;
	}
	
	vec4 position = vec4(vertexPosition.xyz, 1.0);
	
	if(drawingStatus == 0)
	{	
		gl_Position = projection * view * model * position;
	}
	else if (drawingStatus == 1)
	{
		gl_Position = projection * hudmodel * position;
	}
	
	
	fragmentPosition = vertexPosition;
	fragmentColor =  vertexColor;// *vec4(colorOffset);// * colorOffset; // *colorOffset
	fragmentUV = UV;
}
