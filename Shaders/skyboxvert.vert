#version 330 core

//Get the vertex position of the Skybox
layout(location = 0) in vec3 aPos;

out vec3 texCoord;


//Create projection variable
uniform mat4 projection;

//Create view variable
uniform mat4 view;

void main()
{
	vec4 pos = projection * view * vec4(aPos, 1.0);

	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);

	texCoord = aPos;
}