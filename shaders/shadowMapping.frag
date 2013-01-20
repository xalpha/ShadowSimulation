varying vec3 normalVec;
varying vec4 vertexPos;
varying float lightAngle, fragmentAlpha, shadingMode;

uniform sampler2D shadowMap;

void main()
{
	float shadowDepth, fragDepth;
	float light=1.0;
	float energy;
	vec3 normal;
	vec4 outputColor;
	
	if(true)
	{
		energy = lightAngle;
	}
	else
	{
		normal = normalize(normalVec);
		energy = max( 0.0, dot(normal, gl_LightSource[0].position.xyz) );
	}
	
	// check if the fragment is shadowed
	// TODO not sure how elegant it is to sample a depth texture via "texture2D"
	// TODO adaptive pixel offset
	shadowDepth = texture2D(shadowMap, vertexPos.xy).r + 0.0021;
	fragDepth = vertexPos.z;
	if(shadowDepth < fragDepth)
	{
		light = 0.0;	// shadowed
		
		energy = 0.0;	// shadowed
	}
	
	// TODO scale value to corespond to the overlapping of multipple sun positions
	
	// red =	light values
	// green =	energy values
	// blue =	black
	// alpha = fragmentAlpha
	outputColor = fragmentAlpha*vec4( light, energy, 0.0, 1.0); 
	//outputColor = vec4(light, energy, 0.0, 0.01); 
		
	gl_FragColor = outputColor;
}