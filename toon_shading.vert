varying vec3 normal;
varying vec3 position;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();
	position = gl_Position.xyz / gl_Position.w;
    gl_FrontColor = gl_Color;
}
