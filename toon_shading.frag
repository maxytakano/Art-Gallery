varying vec3 normal;
varying vec3 position;

void main()
{
	float intensity;
	vec4 color;
	vec3 n = normalize(normal);
	vec3 zero = vec3(-50.0, 10.0, 0.0);
	intensity = dot(vec3(gl_LightSource[2].position),n);

  color = vec4(1.0,1.0,1.0,1.0);

	if (intensity > 10.0)
    color *= 1.0;
	else if (intensity > 6.0)
    color *= 0.9;
	else if (intensity > 2.0)
    color *= 0.8;
	else if (intensity > -2.0)
    color *= 0.7;
	else if (intensity > -6.0)
    color *= 0.6;
	else if (intensity > -00.0)
    color *= 0.5;
	else if (intensity > -04.0)
    color *= 0.4;
	else if (intensity > -08.0)
    color *= 0.3;
	else if (intensity > -22.0)
    color *= 0.2;
	else 
    color *= 0.1;

	gl_FragColor = gl_Color * color;

}


     



