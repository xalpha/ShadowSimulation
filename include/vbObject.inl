inline void VBObject::draw()
{
	draw( 0, vertexCount, 0xFFFF);
}


inline void VBObject::draw(int offset, int length)
{
	draw( offset, length, 0xFFFF);
}

inline void VBObject::draw(int offset, int length, unsigned int bindData)
{
	if(vertexCount != 0)
	{
		glEnableClientState( GL_VERTEX_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);
		glVertexPointer( vertexSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(normalCount != 0)
	{
        glEnableClientState( GL_NORMAL_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals);
		glNormalPointer( GL_FLOAT, 0, (char*)NULL);
	}

	if( (colorCount != 0) && ((bindData & VBO_COLOR) !=0) )
	{
		glEnableClientState( GL_COLOR_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors);
		glColorPointer( colorSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(texCoordCount != 0)
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboTexCoords);
		glTexCoordPointer( texCoordSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(indexCount != 0)
	{
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vboIndices);
		glDrawElements( primitivesType, indexCount*indexSize, GL_UNSIGNED_INT, (char*)NULL);
	}
	else
	{
		glDrawArrays( primitivesType, offset, length);
	}

	glDisableClientState( GL_VERTEX_ARRAY);
	glDisableClientState( GL_NORMAL_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY);
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}




