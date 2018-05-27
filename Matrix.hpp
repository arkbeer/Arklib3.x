#pragma once
#include<directxmath.h>

namespace ark {
	class Matrix {
		DirectX::XMMATRIX matrix;
	public:
		Matrix(const DirectX::XMMATRIX& m):matrix(m){}
		Matrix():Matrix(DirectX::XMMatrixIdentity()){}
		const DirectX::XMMATRIX GetMatrix() { return matrix; }
		Matrix SetMatrix(const DirectX::XMMATRIX& m) {
			matrix = m;
			return *this;
		}
		Matrix Identity() {
			matrix=DirectX::XMMatrixIdentity();
			return *this;
		}
#define SET(trans) Matrix trans(const float x,const float y,const float z){matrix*=DirectX::XMMatrix##trans(x,y,z);return *this;}
		SET(Scaling);
		SET(Translation);
#undef SET
#define SET(axis) Matrix Rotation##axis(const float angle){matrix*=DirectX::XMMatrixRotation##axis(angle);return *this;}
		SET(X);
		SET(Y);
		SET(Z);
#undef SET
	};
}