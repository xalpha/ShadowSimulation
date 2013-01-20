varying float mode;
varying vec3 normal;

uniform sampler2D texture;

vec4 energyToHeat(float energy)
{
	vec4 result = vec4(0,0,0,1);
	
	if(energy <= 0.166666)
	{
		result.b = energy*6.0;
	}
	else if(energy <= 0.333333)
	{
		result.g = (energy - 0.166666) * 6.0;
		result.b = 1.0;
	}
	else if(energy <= 0.5)
	{
		result.g = 1.0;
		result.b = 1.0 - (energy - 0.333333) * 6.0;
		
	}
	else if(energy <= 0.666666)
	{
		result.r = (energy - 0.5) * 6.0;
		result.g = 1.0;
	}
	else if(energy <= 0.833333)
	{
		result.r = 1.0;
		result.g = 1.0 - (energy - 0.666666) * 6.0;
	}
	else if(energy <= 1.0)
	{
		result.r = 1.0;
		result.g = (energy - 0.833333) * 6.0;
		result.b = (energy - 0.833333) * 6.0;
	}
	else
	{
		result.r = 1.0;
		result.g = 1.0;
		result.b = 1.0;
	}
	
	return result;
}

void main()
{
	vec4 inputColor = texture2D(texture,gl_TexCoord[0].st);
	vec4 outputColor;
	
	float light, energy;
	
	// compensate for interpolation using the blue channel as a mask: 
	// the more blue the fragment has, the more its original value has been altered
	if( inputColor.b >= 1.0 )
	{
		light = 0.0;
		energy = 0.0;
	}
	else
	{
		light = inputColor.r/(1.0-inputColor.b);
		energy = inputColor.g/(1.0-inputColor.b);
	}

	// set the rendering mode	
	if(mode < 0.24)	outputColor = vec4(light);
	else if(mode < 0.49)	outputColor = energyToHeat(light);
	else if (mode < 0.74) outputColor = vec4(energy);
	else if(mode < 1.0)outputColor = energyToHeat(energy);
	
	gl_FragColor = outputColor;
}