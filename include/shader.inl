inline void Shader::enable()
{
	glUseProgramObjectARB(program);
}

inline void Shader::disable()
{
	glUseProgramObjectARB(0);
}
