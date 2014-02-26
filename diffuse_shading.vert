// Vertex shader for per-pixel diffuse shading.The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the unit light direction and unit
// surface normal, which are passed to the fragment
// shader as varying variables.

varying vec2 vTexCoord;
varying vec3 normal, vNormal, vVertex, light1Dir;
varying vec4 vertexPosition;

void main()
{	
  vertexPosition = gl_ModelViewMatrix * gl_Vertex;
	light1Dir = normalize(vec3(gl_LightSource[1].position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
  vNormal = gl_Normal;
  vVertex = gl_Vertex.xyz;
  vTexCoord = vec2(gl_MultiTexCoord0);

  vec4 old = ftransform();

	gl_Position = ftransform();
}
