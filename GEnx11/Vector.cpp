#include "Vector.h"

namespace CoreSystems
{
	Vector3::Vector3() { x = 0; y = 0; z = 0; SET_SIMD_VARIABLES; }

	Vector3::Vector3(const Vector3& v2)
	{
		this->vector3 = v2.GetXMVECTOR();
		SET_NORMAL_VARIABLES;
	}

	Vector3::Vector3(const XMVECTOR& xmv)
	{
		vector3 = xmv;
		SET_NORMAL_VARIABLES;
	}

	Vector3::Vector3(const float& v1, const float& v2, const float& v3 = 0.0f)
	{
		x = v1; y = v2; z = v3;
		SET_SIMD_VARIABLES;
	}

	Vector3::Vector3(const float* v1, const float* v2, const float* v3 = nullptr)
	{
		x = *v1; y = *v2; z = *v3;
		SET_SIMD_VARIABLES;
	}

	//Set XMVECTOR from XMFloat3
	void Vector3::SetXMVector()
	{		
		vector3 = XMLoadFloat3(&float3);
	}
	//Convert Vector Obj xyz to XMFloat3
	void Vector3::SetXMFloat3()
	{
		float3.x = x;
		float3.y = y;
		float3.z = z;
	}
	//Load XMFLOAT3 from XMVECTOR
	void Vector3::LoadXMFloat3()
	{
		XMStoreFloat3(&float3, vector3);
	}

	void Vector3::SetXYZ()
	{
		x = float3.x;
		y = float3.y;
		z = float3.z;
	}

	void Vector3::operator= (const Vector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		SET_SIMD_VARIABLES;
	}

	Vector3 Vector3::operator+ (const Vector3& v2)
	{
		XMVECTOR x = this->vector3 + v2.GetXMVECTOR();
		return Vector3(x);
	}

	void Vector3::operator+= (const Vector3& v2)
	{
		this->vector3 += v2.GetXMVECTOR();
		SET_NORMAL_VARIABLES;
	}

	Vector3 Vector3::operator- (const Vector3& v2)
	{
		XMVECTOR x = this->vector3 - v2.GetXMVECTOR();
		return Vector3(x);
	}

	void Vector3::operator-= (const Vector3& v2)
	{
		this->vector3 = XMVectorSubtract(this->vector3, v2.GetXMVECTOR());
		SET_NORMAL_VARIABLES;
	}

	void Vector3::operator*= (const Vector3& v2)
	{
		this->vector3 *= v2.GetXMVECTOR();
		SET_NORMAL_VARIABLES;
	}

	void Vector3::operator*= (const float& value)
	{
		this->vector3 *= value;
		SET_NORMAL_VARIABLES;
	}

	Vector3 Vector3::operator* (const Vector3& v2)
	{
		XMVECTOR x = this->vector3 * v2.GetXMVECTOR();
		return Vector3(x);
	}

	Vector3 Vector3::operator* (float value)
	{
		XMVECTOR x = this->vector3 * value;
		SET_NORMAL_VARIABLES;
		return Vector3(x);
	}

	bool Vector3::operator== (const Vector3& v2)
	{
		return (x == v2.x && y == v2.y && z == v2.z);
	}

	double Vector3::Distance(const Vector3& v2)
	{
		return sqrt((powf((this->x - v2.x), 2)) + (powf((this->y - v2.y), 2)) + (powf(this->z - v2.z, 2)) + (powf(this->z - v2.z, 2)));
	}

	void Vector3::dot(const Vector3& v2)
	{
		x *= v2.x;
		y *= v2.y;
		z *= v2.z;
	}
};