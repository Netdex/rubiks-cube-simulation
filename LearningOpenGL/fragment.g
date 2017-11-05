#version 330

in vec3 frag_color;
in vec3 frag_normal;
in vec3 frag_position;

in vec3 frag_camera_position;

out vec4 render_color;

void main()
{
	vec3 result = frag_color;
		
    render_color = vec4(result, 1.0);
}