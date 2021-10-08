#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include"Core.h"

namespace CoreSystems
{
	
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		//static member methods
		static void AddToBuffer(const Model::MeshData&);
		static void Draw();

		//Accessor Methods
		UINT& GetVertexCount() const { return VertexCount; }
		UINT& GetIndexCount() const { return IndexCount; }
		static void Set(const ID3D11Device& const Device , const ID3D11DeviceContext& const DeviceContext);
	private:
		//Member Methods
		static void CreateBuffers();

		//Member Variables
		static Model::MeshData MeshData;
		static UINT VertexCount;
		static UINT IndexCount;
		static UINT* IndexOffset;
		static UINT* VertexOffset;
		static const ID3D11Device* const device;
		static const ID3D11DeviceContext* const deviceContext;
	};


	UINT BufferManager::VertexCount = 0;
	UINT BufferManager::IndexCount = 0;
	UINT* BufferManager::IndexOffset = 0;
	UINT* BufferManager::VertexOffset = 0;
	
};
#endif