#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 camera_position;

out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_position;

out vec3 frag_camera_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	frag_color = color;
	frag_normal = normal;
	frag_position = position;
	
	frag_camera_position = (inverse(model) * vec4(camera_position, 1.0)).xyz;
	
    gl_Position = proj * view * model * vec4(position, 1.0);
}