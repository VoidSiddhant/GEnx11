#include"Vertex.h"

namespace CoreSystems
{
	Model::Model(const bool& writeable,const FILL_MODE& fillMode)
	{
		//void* a = &inVertex;
		//char* ch = reinterpret_cast<char*>(a);
		this->writeable = writeable;
		fill_mode = fillMode;
		m_vB = nullptr;
		m_iB = nullptr;
		inputLayout = nullptr;
		gridIndexCount = gridVertexCount = gridVertexOffset = gridIndexOffset = 0;
		boxVertexCount = boxIndexCount = boxVertexOffset = boxIndexOffset =0;
		mFx = nullptr;
		mTech = 0;
		device = gp_Core->GetGraphics()->GetDevice();
		deviceContext = gp_Core->GetGraphics()->GetContext();
		m_fxWorldViewProj = nullptr;
		this->BuildFX();
		this->BuildVertexLayout(nullptr);
		gp_Core->AddInList(this);
	}

	void Model::BuildGeometryBuffers(bool writeable)
	{
		//Get Mesh Data whenever a new mesh is created and add it to the global vertexBuffer
	
		//Set Vertex Buffer Description
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex)*globalMesh.VertexData.size();       /////  Chagnge this to GEt
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = (writeable) ? D3D11_CPU_ACCESS_WRITE : 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vintData;
		vintData.pSysMem = &globalMesh.VertexData[0];
		device->CreateBuffer(&vbd,&vintData, &m_vB);

		//Set Index Buffer Description
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT)*globalMesh.IndexData.size();        /// Change this to get from user
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &globalMesh.IndexData[0];
		device->CreateBuffer(&ibd, &iinitData, &m_iB);
	}

	void Model::BuildFX()
	{
		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		
		//Compiled Shaders during Runtime
		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compiledMsg = 0;
		/*HRESULT hr = D3DCompileFromFile(L"G:/VS2015_PROJECTS/Projects/GEnx11/GEnx11/FX/vertex.vs"
										, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE
										, "VS", "vs_5_0", shaderFlags,NULL
										,&compiledShader,&compiledMsg);*/
		HRESULT hr = D3DX11CreateEffectFromFile(L"G:/VS2015_PROJECTS/Projects/GEnx11/GEnx11/FX/color.cso",
												shaderFlags, device, &mFx);
		if (FAILED(hr))
		{
			gp_Core->ShowMessageBox("Failed to Compiled Effect", "Error");
		}
	
		//Check For Compilation Errors both in message and hresult
		if (fill_mode == FILL_MODE_SOLID)
			mTech = mFx->GetTechniqueByName("ColorTech");
		else if (fill_mode == FILL_MODE_WIREFRAME)
			mTech = mFx->GetTechniqueByName("ColorTechWireFrame");
		
		m_fxWorldViewProj = mFx->GetVariableByName("gWorldViewProj")->AsMatrix();
		

		ReleaseCOM(compiledShader);
	}

	void Model::BuildVertexLayout(ID3D10Blob* compiledShader)
	{
		//Describe the Custom Vertex structure/Input Layout
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		
		//Create The Input Layout
		//Alternate Method...........
		/*HRESULT hr = gp_Core->GetGraphics()->GetDevice()->CreateInputLayout(vertexDesc, 2
					, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize()
					, &inputLayout);*/  
		//...........................
		D3DX11_PASS_DESC passDesc;
		mTech->GetPassByIndex(0)->GetDesc(&passDesc);
		device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature
			, passDesc.IAInputSignatureSize, &inputLayout);
		//ReleaseCOM(compiledShader);
	}

	void Model::CreateBox(const Vector3& centerPos, float scaleX, float scaleY, float scaleZ
						,XMFLOAT4 inColor,MeshData* MeshObject)
	{
		if (writeable)
			assert(MeshObject);
		//Compute Vertices for the box
		float halfX = scaleX / 2.0f;
		float halfY = scaleY / 2.0f;
		float halfZ = scaleZ / 2.0f;
		int index = 0;
		v.resize(8);
		vIndex.resize(36);
		for (UINT k = 0; k < 2; k++)
		{
			float z = -halfZ + scaleZ*k;
			for (UINT i = 0; i < 2; i++)
			{
				float y = halfY - scaleY*i;
				for (UINT j = 0; j < 2; j++)
				{
					float x = -halfX + scaleX*j;
					v[index].pos = XMFLOAT3(centerPos.x+x,centerPos.y+y,centerPos.z+z);
					v[index].color = inColor;
					index++;
				}
			}
		}

		//Compute Indices for the box
		//front face
		vIndex[0] = 2; vIndex[1] = 0; vIndex[2] = 1;
		vIndex[3] = 2; vIndex[4] = 1; vIndex[5] = 3;
		//back face
		vIndex[6] = 7; vIndex[7] = 5; vIndex[8] = 4;
		vIndex[9] = 7; vIndex[10] = 4; vIndex[11] = 6;
		//left face
		vIndex[12] = 6; vIndex[13] = 4; vIndex[14] = 0;
		vIndex[15] = 6; vIndex[16] = 0; vIndex[17] = 2;
		//right face
		vIndex[18] = 3; vIndex[19] = 1; vIndex[20] = 5;
		vIndex[21] = 3; vIndex[22] = 5; vIndex[23] = 7;
		//top face
		vIndex[24] = 0; vIndex[25] = 4; vIndex[26] = 5;
		vIndex[27] = 0; vIndex[28] = 5; vIndex[29] = 1;
		//bottom face
		vIndex[30] = 6; vIndex[31] = 2; vIndex[32] = 3;
		vIndex[33] = 6; vIndex[34] = 3; vIndex[35] = 7;
		boxIndexCount += vIndex.size();
		boxVertexCount += v.size();
		//Add to the Global Vertex and Index Data
		globalMesh.VertexData.insert(globalMesh.VertexData.end(),v.begin(),v.end());
		globalMesh.IndexData.insert(globalMesh.IndexData.end(), vIndex.begin(), vIndex.end());
		if (MeshObject)
		{
			MeshObject->VertexData.insert(MeshObject->VertexData.end(), v.begin(), v.end());
			MeshObject->IndexData.insert(MeshObject->IndexData.end(), vIndex.begin(), vIndex.end());
		}
		//Clear the temporary data
		v.clear();
		vIndex.clear();
		boxIndexOffset += gridIndexCount;
		boxVertexOffset += gridVertexCount;
		this->BuildGeometryBuffers(writeable);
		//this->BuildGeometryBuffers();

	}

	//GRID GEOMETRY CODE
	void Model::CreateGrid(const Vector3& centerPos,float width,float depth,UINT verticesX,UINT verticesZ ,
							bool addNoise,MeshData* meshObject)
	{
		if (writeable)
			assert(meshObject);
		assert(verticesX > 1 || verticesZ >1);
		gridVertexCount = verticesX*verticesZ;
		gridIndexCount = (verticesX - 1)*(verticesZ - 1)*6;
		v.resize(gridVertexCount);
		vIndex.resize(gridIndexCount);
		float dx = width / (verticesX - 1);
		float dz = depth / (verticesZ - 1);
		float halfWidth = width / 2.0f;
		float halfDepth = depth / 2.0f;
		int index =0;
		//Compute The Grid Vertex
		for (UINT rows = 0; rows < verticesZ; rows++)
		{
			float z = halfDepth - rows*dz;
			for (UINT col = 0; col < verticesX; col++)
			{
				index = col + rows*verticesX;
				float x = -halfWidth + col*dx;
				float y = (0.30f*(z*sinf(0.1f*x) + x*cosf(0.1f*z))) * (float)addNoise;
				v[index].pos = XMFLOAT3(x+centerPos.x,y, z + centerPos.z);
				if (v[index].pos.y < -10.0f)
					v[index].color = Colors::Sandy;
				else if (v[index].pos.y < 5.0f)
					v[index].color = Colors::Light_Yellow_Green;
				else if (v[index].pos.y < 12.0f)
					v[index].color = Colors::Dark_Yellow_Green;
				else if (v[index].pos.y < 20.0f)
					v[index].color = Colors::Brown;
				else
					v[index].color = Colors::White;
			}
		}
		
		//Compute the Grid Indices
		UINT k = 0;
		for (UINT row = 0; row < (verticesZ - 1); row++)
		{
			for (UINT col = 0; col < (verticesX - 1); col++)
			{
				//Index of One Quad
				vIndex[k]	  = (row + 1)*verticesX + col;//row*verticesX + col;
				vIndex[k + 1] = row*verticesX + col;//row*verticesX+col+1;
				vIndex[k + 2] = row*verticesX + col + 1;//(row + 1)*verticesX + col;
				vIndex[k + 3] = (row + 1)*verticesX + col;//row*verticesX + col + 1;
				vIndex[k + 4] = row*verticesX + col + 1;//(row + 1)*verticesX + (col + 1);
				vIndex[k + 5] = (row + 1)*verticesX + (col + 1);
				//Move to Next Quad
				k +=6;
			}
		}
		//Add Vertex Data to Global Mesh Data
		globalMesh.VertexData.insert(globalMesh.VertexData.end(), v.begin(),v.end());
		if(meshObject)
			meshObject->VertexData.insert(meshObject->VertexData.end(), v.begin(), v.end());
		v.clear();
		//Add Index Data to Global Mesh Data
		globalMesh.IndexData.insert(globalMesh.IndexData.end(), vIndex.begin(), vIndex.end());
		if (meshObject)
			meshObject->IndexData.insert(meshObject->IndexData.end(), vIndex.begin(), vIndex.end());
		vIndex.clear();
		gridIndexOffset += boxIndexCount;
		gridVertexOffset += boxVertexCount;
		//this->BuildGeometryBuffers();
	}

	Vertex* Model::MapBufferResource()
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;
		deviceContext->Map(m_vB, 0, D3D11_MAP_WRITE_DISCARD, 0,&bufferData);
		return (reinterpret_cast<Vertex*>(bufferData.pData));
	}

	void Model::UnMapBufferResource()
	{
		deviceContext->Unmap(m_vB,0);
	}

	void Model::Draw()
	{
		if (inputLayout == nullptr) return;

		//Check if Vertex and Index buffers are not NULL
		if (m_vB == nullptr || m_iB == nullptr)
			this->BuildGeometryBuffers(writeable);
		//Set the created Input Layout for the Current Draw Call
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//Change this Method Location to a Common Method wrt Model Class..ie....
		//All Models can share the below lines of instructions by making a global buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_vB, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_iB, DXGI_FORMAT_R32_UINT, 0);
		
		//Alternate Method Set Vertex Shader and Pixel Shader
		/*deviceContext->VSSetShader(vs, NULL, 0);
		deviceContext->PSSetShader(ps, NULL, 0);*/
		//Load Up All the Coordinate Matrices
		const float*  m = reinterpret_cast<const float*>(&(camera->GetWPVMatrix()));
		m_fxWorldViewProj->SetMatrix(m);
		//Effect Method
		//Finally Draw the indexed primitive
		D3DX11_TECHNIQUE_DESC techDesc;
		mTech->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; i++)
		{
			//Draw Grid

			mTech->GetPassByIndex(i)->Apply(0, deviceContext);
			deviceContext->DrawIndexed(gridIndexCount, gridIndexOffset, gridVertexOffset);
			//DrawBox
			deviceContext->DrawIndexed(boxIndexCount, boxIndexOffset, boxVertexOffset);
		}
		
	}

	//Transform Methods
	void Model::Rotate(const Vector3& rotVector)
	{

	}

	Model::~Model()
	{
		//Clear GPU memory
		ReleaseCOM(m_iB);
		ReleaseCOM(m_vB);
		ReleaseCOM(mFx);
		ReleaseCOM(inputLayout);
		//Clear System Memory
	}

}; // End of Namespace   