#include"Camera.h"

namespace CoreSystems
{
	extern Camera* camera = nullptr;

	Camera::Camera()
	{
		//vs = { 0.0f,0.0f,0.0f };
		this->position = Vector3(0.0f, 0.0f, -10.0f);
		this->up =  Vector3(0.0f, 1.0f, 0.0f);
		this->target = Vector3(0.0f, 0.0f, 1.0f);
		this->right = Vector3(1.0f, 0.0f, 0.0f);
		//CreateView and Projection Matrix
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_worldMat, I);
		XMStoreFloat4x4(&m_viewMat, I);
		XMStoreFloat4x4(&m_projMat, I);
		RECT rect;
		GetClientRect(gp_Core->GetHWND(), &rect);
		screen_width = (int)(rect.right - rect.left);
		screen_height = (int)(rect.bottom - rect.top);
		this->SetViewMatrix();
		this->SetProjectionMatrix();
	}

	Camera::Camera(const Vector3& position, const Vector3& target, const Vector3& upVector)
	{
		//Set Variables for the Camera Object
		this->position = position;
		this->target = target;
		this->up =upVector;
		this->right = Vector3(1.0f, 0.0f, 0.0f);
		RECT rect;
		GetClientRect(gp_Core->GetHWND(), &rect);
		screen_width = (int)(rect.right - rect.left);
		screen_height = (int)(rect.bottom - rect.top);
		//CreateView and Projection Matrix
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_worldMat, I);
		XMStoreFloat4x4(&m_viewMat, I);
		XMStoreFloat4x4(&m_projMat, I);
		this->SetViewMatrix();
		this->SetProjectionMatrix();
	}
	
	void Camera::UpdateProjectionMatrix(int w,int h)
	{
		screen_width = w;
		screen_height = h;
		this->SetProjectionMatrix();
	}

	void Camera::SetViewMatrix()
	{
		//Orthogonalize the right up and look vector
		XMVECTOR L = XMVector3Normalize(target.GetXMVECTOR());
		//recompute Up Vector and normalize it
		XMVECTOR U = XMVector3Normalize(XMVector3Cross(L, right.GetXMVECTOR()));
		//recompute Right Vector
		right = XMVector3Cross(U,L);
		//Compute Dot Product p.r,p.u,p.t
		float x = -XMVectorGetX(XMVector3Dot(position.GetXMVECTOR(),right.GetXMVECTOR()));
		float y = -XMVectorGetX(XMVector3Dot(position.GetXMVECTOR(), up.GetXMVECTOR()));
		float z = -XMVectorGetX(XMVector3Dot(position.GetXMVECTOR(), target.GetXMVECTOR()));

		//Create View Matrix
		m_viewMat(0, 0) = right.x;
		m_viewMat(1, 0) = right.y;
		m_viewMat(2, 0) = right.z;
		m_viewMat(3, 0) = x;
		m_viewMat(0, 1) = up.x;
		m_viewMat(1, 1) = up.y;
		m_viewMat(2, 1) = up.z;
		m_viewMat(3, 1) = y;
		m_viewMat(0, 2) = target.x;
		m_viewMat(1, 2) = target.y;
		m_viewMat(2, 2) = target.z;
		m_viewMat(3, 2) = z;
		m_viewMat(0, 3) = 0.0f;
		m_viewMat(1, 3) = 0.0f;
		m_viewMat(2, 3) = 0.0f;
		m_viewMat(3, 3) = 1.0f;
	}

	void Camera::SetProjectionMatrix()
	{
		//Create Projection Matrix
		float angle = 0.25*XM_PI;
		XMMATRIX p = XMMatrixPerspectiveFovLH(angle, screen_width / screen_height, 1.0f, 1000.0f);
		XMStoreFloat4x4(&m_projMat, p);
	}

	void Camera::Strafe(float d)
	{
		XMVECTOR strafe = XMVectorReplicate(d);
		XMVECTOR currentPos = position.GetXMVECTOR();
		//Compute Strafe Along X
		position = XMVectorMultiplyAdd(strafe, right.GetXMVECTOR(), currentPos);
		this->Update();
	}

	void Camera::Translate(const Vector3& amtToMove)
	{
		XMVECTOR move = XMVectorReplicate(amtToMove.z);
		XMVECTOR strafe = XMVectorReplicate(amtToMove.x);
		//Compute Move Along Z
		position = XMVectorMultiplyAdd(move, target.GetXMVECTOR(), position.GetXMVECTOR());
		//Compute Strafe Along X
		position = XMVectorMultiplyAdd(strafe, right.GetXMVECTOR(), position.GetXMVECTOR());
		this->Update();
	}

	void Camera::Rotate(Vector3 Angle)
	{
#define Rad(x) XMConvertToRadians(x)
		XMMATRIX rotY = XMMatrixRotationAxis(right.GetXMVECTOR(), Rad(Angle.y));
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(0.0f,Rad(Angle.x), Rad(Angle.z));
		target = XMVector3TransformNormal(target.GetXMVECTOR(), rotY);
		up = XMVector3TransformNormal(up.GetXMVECTOR(), rotY);
		target = XMVector3TransformNormal(target.GetXMVECTOR(), rotMat);
		up = XMVector3TransformNormal(up.GetXMVECTOR(), rotMat);
		right = XMVector3TransformNormal(right.GetXMVECTOR(), rotMat);
		this->Update();
	}


	void Camera::SetPos(const Vector3& vin)
	{
		position = vin;
		//target.x = vin.x;
		//target.y = vin.y;
		this->Update();
 	}

	Camera::~Camera()
	{

	}

	void Camera::Update()
	{
		//Update View Matrix each draw call
		this->SetViewMatrix();
	}
};