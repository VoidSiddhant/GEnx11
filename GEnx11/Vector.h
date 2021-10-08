
#ifndef VECTOR_H
#define VECTOR_H

#include<DirectXMath.h>

#ifndef SET_SIMD_VARIABLES
#define SET_SIMD_VARIABLES            \
			SetXMFloat3();			\
			SetXMVector();			
#endif

#ifndef SET_NORMAL_VARIABLES
#define SET_NORMAL_VARIABLES		\
					LoadXMFloat3(); \
					SetXYZ(); 
#endif					

using namespace DirectX;

namespace CoreSystems
{
	class Vector3
	{
	public:
		Vector3();
		Vector3(const Vector3& v2);
		Vector3(const XMVECTOR& xmv);
		Vector3(const float& v1, const float& v2, const float& v3 );
		Vector3(const float* v1, const float* v2, const float* v3 );
		void operator= (const Vector3& v);
		//void operator= (const D3DXVECTOR3& v);
		Vector3 operator+ (const Vector3& v2);
		Vector3 operator- (const Vector3& v2);
		void operator-= (const Vector3& v2);
		void operator+= (const Vector3& v2);
		void operator*= (const Vector3& v2);
		void operator*= (const float& value);
		bool operator== (const Vector3& v);
		Vector3 operator* (const Vector3& v2);
		Vector3 operator* (float value);
		double Distance(const Vector3& v2);
		void dot(const Vector3& v2);

		//Set XMFLOAT3 VECTOR XYZ
		void SetXMVector();
		void LoadXMFloat3();
		void SetXMFloat3();
		XMVECTOR GetXMVECTOR() const { return vector3; }
		void SetXYZ();
		float x, y, z;
	private:
		
		XMFLOAT3 GetXMFLOAT() const { return float3; }
		XMVECTOR vector3;
		XMFLOAT3 float3;
	};

};
#endif
