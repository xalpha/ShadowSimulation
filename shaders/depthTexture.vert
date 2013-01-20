varying vec4 vertexPos;

void main(void)
{
	vertexPos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position =  ftransform();
}