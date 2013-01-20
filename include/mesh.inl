inline float Mesh::getSizeX(){	return  sizeX;}
inline float Mesh::getSizeY(){	return  sizeY;}
inline float Mesh::getSizeZ(){	return  sizeZ;}

inline unsigned int Mesh::getMeshType(){	return meshType;	}
inline int Mesh::getVertexPerFace(){	return vertexPerFace;	}
inline int Mesh::getVertexCount(){	return vertexCount;	}
inline float* Mesh::getVertices(){	return vertices;	}
inline int Mesh::getIndexCount() {	return indexCount;	}
inline unsigned int* Mesh::getIndices(){	return indices;	}
inline int Mesh::getNormalCount(){	return normalCount;	}
inline float* Mesh::getNormals() {	return normals;		}
inline int Mesh::getFaceNormalCount(){	return faceNormalCount;}
inline float* Mesh::getFaceNormals(){	return faceNormals;	}
