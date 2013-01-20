varying vec4 vertexPos;

void main()
{
	float depth = vertexPos.z*0.5+0.5;
	gl_FragColor = vec4(depth);
	gl_FragDepth = depth;
}