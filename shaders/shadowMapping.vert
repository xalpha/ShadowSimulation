varying vec3 normalVec;
varying vec4 vertexPos;
varying float lightAngle, fragmentAlpha, shadingMode;


void main(void)
{
	normalVec = gl_Normal;

	// calculate the angel between the light lector and the camera vector
	//lightAngle = max(dot(faceNormal, lightVec), 0.0);
	lightAngle = gl_Color.r;
	shadingMode = gl_Color.g;
	fragmentAlpha = gl_Color.a;

	vertexPos = gl_TextureMatrix[0] * gl_Vertex;
	
	gl_Position =  ftransform();
}