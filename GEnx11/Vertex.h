#ifndef VERTEX_H
#define VERTEX_H
#include"Core.h"
#include<d3dx11effect.h>
#include<D3Dcompiler.h>
#include<vector>

#define CREATEMODEL(name) Create##name();
//Vertex Class Defination
namespace CoreSystems
{
	enum VERTEX_LISTING_TYPE
	{
		POINT_LIST = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		LINE_LIST = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		LINE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		TRIANGLE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

	};

	enum FILL_MODE
	{
		FILL_MODE_SOLID,
		FILL_MODE_WIREFRAME
	};

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	struct VertexTextured
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 tex0;
	};

	class Model
	{
	public:
		//Structure to Hold the Mesh Data
		typedef struct MeshData
		{
			vector<Vertex> VertexData;
			vector<UINT> IndexData;
		}MeshData;

		Model(const bool& writeable = false,const FILL_MODE& fillMode = FILL_MODE_SOLID);
		//Model(const VertexTextured& InputVertexList);
		~Model();
		//Gets Called in Each Draw Call
		void Draw();
		void BuildGeometryBuffers(bool writeable = false);
		void BuildFX();
		void BuildVertexLayout(ID3D10Blob* compiledShader);
		Vertex* MapBufferResource();
		void UnMapBufferResource();
		//Different Geometry Create Methods
		void CreateBox(const Vector3& centerPos, float scaleX, float scaleY, float scaleZ
						,XMFLOAT4 color,MeshData* MeshObject = nullptr);
		void CreateGrid(const Vector3& centerPosition,float witdth,float depth,UINT verticesX
						,UINT verticesZ,bool AddNoise = false,MeshData* MeshObject = NULL);

		//Transform Methods
		static void Rotate(const Vector3& rotVector);
		UINT GetVertexCount() const { return (UINT)globalMesh.VertexData.size(); }
	private:
		//Vertices List
		 //Vertex v;
		//Vertex Index List
		//UINT vIndex;
		//Global Vertex Buffer Handler
		 ID3D11Buffer* m_vB;
		//Global Index Buffer Handler
		ID3D11Buffer* m_iB;
		//Input Layout Handler
		ID3D11InputLayout* inputLayout;
		//Device Pointer
		ID3D11Device* device;
		//Context Pointer
		ID3D11DeviceContext* deviceContext;
		//WORLD VIEW PROJ MAT
		//XMMATRIX m_fxWorldViewProj;
		//Shader Related Stuff
		ID3DX11Effect* mFx;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

		//Geometry Related Stuff
		int fill_mode;
		UINT gridVertexCount, gridIndexCount,gridIndexOffset,gridVertexOffset;
		UINT boxVertexCount, boxIndexCount,boxIndexOffset,boxVertexOffset;
		//Global Engine Mesh Data
		MeshData globalMesh;
		bool writeable;
		//Local Vertex Data
		vector<Vertex> v;
		vector<UINT> vIndex;
	}; //End of Class
}; // End of namespace

#endif
