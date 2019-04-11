#pragma

struct MainHeader
{
	unsigned int meshCount;
	unsigned int materialCount;
	unsigned int characterCount;
};

struct MeshHeader
{
	unsigned int vertexCount;
	unsigned int materialID; //our different morph bois will use the same material
	unsigned int UV[2]; //should this be here? int?
	char nameOfCustomAttribute[100]; //might not be needed
	bool collision; //custom maya attribute
};

struct BoundingBoxHeader
{
	unsigned int vertexCount;
};

//Small info structs
struct Vertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float biTangent[3];
};

struct BoundingBoxVertex
{
	float position[3];
	//indices? faces?
};

struct Material
{
	//texture buffer pointer
	//shader int pointer
};

struct Texture
{
	//texture buffer
};

struct ShaderOption
{
	//what shader it should use
};