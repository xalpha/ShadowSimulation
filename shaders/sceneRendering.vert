varying float mode;
varying vec3 normal;

void main(void)
{
	//normal = normalize(gl_NormalMatrix * gl_Normal);
	normal = gl_Normal;

	mode = gl_Color.r;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position =  ftransform();
} 