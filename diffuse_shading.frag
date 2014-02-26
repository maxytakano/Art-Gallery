// Fragment shader for per-pixel diffuse shading. The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the dot product between the unit
// surface normal and light direction, which were 
// passed as varying inputs from the vertex shader. The
// result is multiplied with the vertex color, which is 
// accessed through a pre-defined varying variable.

// Base of this code from http://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Multiple_Lights
// Describes the basic idea behind the normal lighting rendering in GLSL (with no shaders enabled)

varying vec2 vTexCoord;
varying vec3 normal, vNormal, vVertex, light1Dir;
varying vec4 vertexPosition;

uniform mat4 v_inv;

uniform sampler2D myTexture;
uniform bool sampleEnabled;

void main()
{
    vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - vertexPosition));
    vec3 world_lighting;
    world_lighting[0] = 0.0;
    world_lighting[1] = 0.0;
    world_lighting[2] = 0.0;
    vec3 direction;
    float attn;

    world_lighting = vec3(gl_LightModel.ambient * gl_FrontMaterial.ambient);
 
    vec4 textureColor = texture2D(myTexture, vTexCoord);

    // We have 3 lights (directional, point, spot)
    for(int i = 1; i <= 5; i++) {
      if(gl_LightSource[i].position[3] == 2.0) continue;
        if(gl_LightSource[i].position.w == 0.0) {
            // This is a directional light source, give up and cry
            attn = 1.0;
            direction = normalize(vec3(gl_LightSource[i].position));
        } else {
            // Not a directional light. That sucks nuts

            // Vector from us to light
            vec3 usToLight = vec3(gl_LightSource[i].position - vertexPosition);
            float d = length(usToLight);
            direction = normalize(usToLight);

            // This formula ripped off the slides with little mathematical proof on my part
            attn = 1.0 / (gl_LightSource[i].constantAttenuation + gl_LightSource[i].linearAttenuation * d + gl_LightSource[i].quadraticAttenuation * d * d);

            if(gl_LightSource[i].spotCutoff <= 90.0) {
              // Damn spotlights...

              // We need the angle between the vector from light source to spotlight position (spotDirection), and the vector from the light to us (-direction)
              // If this angle is outside the cutoff, we don't show that light.  k? k.
              float positiveCosine = max(dot(-direction, normalize(gl_LightSource[i].spotDirection)), 0.0);
              if(positiveCosine < cos(radians(gl_LightSource[i].spotCutoff))) {
                // Outside our cone, don't show that light LIKE DAMN
                attn = 0.0;
              } else {
                // This formula gloriusly stolen from http://www.talisman.org/opengl-1.1/Reference/glLight.html
                if(dot(normal, normalize(-usToLight)) < 0.0) {
                  attn = attn * pow(positiveCosine, gl_LightSource[i].spotExponent);
                } else {
                  attn = 0.0;
                }
              }
            }
        }

        //vec3 specular;
        //if(dot(normal, direction) < 0.0) {
        //    // We are facing the wrong way
        //    specular = vec3(0.0, 0.0, 0.0);
        //} else {
        //    specular = attn * vec3(gl_LightSource[i].specular) * vec3(gl_FrontMaterial.specular) * pow(max(dot(reflect(-direction, normal), viewDirection), 0.0), gl_FrontMaterial.shininess);
        //}

        vec3 diffuse;
        if(sampleEnabled) {
          diffuse = attn * vec3(gl_LightSource[i].diffuse) * vec3(textureColor) * max(dot(normal, direction), 0.0) * vec3(gl_FrontMaterial.diffuse);
        } else {
          diffuse = attn * vec3(gl_LightSource[i].diffuse) *  max(dot(normal, direction), 0.0) * vec3(gl_FrontMaterial.diffuse);
        }

        //world_lighting = world_lighting + specular + diffuse;
        world_lighting = world_lighting + diffuse;
    }

    gl_FragColor = vec4(world_lighting, 1.0);
}
