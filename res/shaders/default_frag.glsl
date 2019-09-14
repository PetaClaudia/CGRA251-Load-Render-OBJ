#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec4 uColor;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	// calculate shading
    vec4 surfaceColor = uColor;
	vec3 eye = normalize(-f_in.position); // direction towards the eye
	float light = abs(dot(normalize(f_in.normal), eye)); // difference between the surface normal and direction towards the eye
	vec4 finalColor = mix(surfaceColor / 4, surfaceColor, light);

	// output to the frambuffer
    fb_color = finalColor;
}
