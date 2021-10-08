#ifndef CAMERA_H
#define CAMERA_H

#include"Core.h"
#include"Vector.h"
namespace CoreSystems
{

	class Camera
	{
		
	public:
		//Default Construsctor for initialising the camra object and setting all the matrices to 0
		Camera(void);
		Camera(const Vector3& position,const Vector3& target,const Vector3& upVector);
		~Camera();
		void UpdateProjectionMatrix(int screenW,int screenH);
		void Update();
		void Strafe(float d);
		void Rotate(Vector3 Angle);

		//Camera Transform Methods

		//Change Position Each Frame
		void Translate(const Vector3& amtToMove);
		void SetPos(const Vector3&);
		Vector3 GetPos()const { return position; }

		//Accessor Methods
		XMMATRIX GetWPVMatrix() const
		{
			XMMATRIX w = XMLoadFloat4x4(&m_worldMat);
			XMMATRIX v = XMLoadFloat4x4(&m_viewMat);
			XMMATRIX p = XMLoadFloat4x4(&m_projMat);
			return (v*p);
		}

	private:

		Vector3 position;
		Vector3 up;
		Vector3 target;
		Vector3 right;
		int screen_width, screen_height;

		//Member Methods
		void SetViewMatrix();
		void SetProjectionMatrix();
		
		//Member Variables
		XMFLOAT4X4 m_worldMat;
		XMFLOAT4X4 m_projMat;
		XMFLOAT4X4 m_viewMat;
	};
	extern Camera* camera;
};
#endif // !CAMERA_H

