#version 330

in vec4 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//layout (location = 2) in vec4 color;

uniform vec4 colorOffset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int isHud;
uniform mat4 hudmodel;
uniform int flipx;
uniform int isJumping;
uniform bool isAnimation;
uniform float animationx;
uniform float timer;

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
	
	vec4 position = vec4(vertexPosition.x, vertexPosition.yz, 1.0);
	if(isHud == 1)
	{	
		gl_Position =  hudmodel * vec4(vertexPosition.xyz, 1.0);
	}
	else
	{
		gl_Position = projection * view * model * position;
	}
	
	fragmentPosition = vertexPosition;
	fragmentColor =  vertexColor * colorOffset; // *colorOffset
	fragmentUV = UV;
}

//void Animation(vec2 UV, float timeframe, float newUV ,int FlipX, float frame)
//{
	//if (FlipX == 1)
	//{
		//newUV = float((1.0f - UV.x) *timeframe);
	//	UV = vec2(newUV + frame , vertexUV.y);
//	}
	//else if (FlipX == 0)
	//{
	//	abc = float(abc + animationx);
	//	UV = vec2(abc, vertexUV.y);
	//}
//}