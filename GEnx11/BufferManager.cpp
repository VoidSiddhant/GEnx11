#include "BufferManager.h"

namespace CoreSystems
{ 

BufferManager::BufferManager()
{
	//Allocation Outside class
}


BufferManager::~BufferManager()
{
	//Deallocation Outside Class
}

void BufferManager::AddToBuffer(const Model::MeshData& inData)
{
	//Add to Total Data Counts
	VertexCount += inData.VertexData.size();
	IndexCount += inData.IndexData.size();

	//Store the data in the buffer to be sent to GPU
	MeshData.VertexData.insert(MeshData.VertexData.end(), inData.VertexData.begin(), inData.VertexData.end());
	MeshData.IndexData.insert(MeshData.IndexData.end(), inData.IndexData.begin(), inData.IndexData.end());
	
}

void BufferManager::CreateBuffers()
{
	bool writable;
	
	ID3D11Buffer* vb;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc,sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = sizeof(Vertex)*MeshData.VertexData.size();
	bufferDesc.CPUAccessFlags = (writable ? D3D11_CPU_ACCESS_WRITE : 0);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sysMemVerts;
	ZeroMemory(&sysMemVerts, sizeof(D3D11_SUBRESOURCE_DATA));
	sysMemVerts.pSysMem = &MeshData.VertexData.begin();
	bufferDesc.StructureByteStride = 0;
	
	device->CreateBuffer(&bufferDesc, &sysMemVerts, &vb);

	ID3D11Buffer* ib;
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
	indexDesc.CPUAccessFlags = 0;
	indexDesc.ByteWidth = sizeof(UINT)*MeshData.IndexData.size();
	indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

}

void BufferManager::Set(const ID3D11Device& const inDevice , const ID3D11DeviceContext& inContext)
{
	*device = inDevice;
	*deviceContext = inContext;
}



};