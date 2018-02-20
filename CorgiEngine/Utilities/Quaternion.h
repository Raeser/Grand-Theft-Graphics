#pragma once

namespace Corgi
{


	template<class T>
	struct Quaternion
	{
		Quaternion();
		Quaternion(T a, T b, T c, T d);
		T x, y, z, w;
	
		static Quaternion<T> GetConjugate(const Quaternion<T>& a);
		void Normalize();
	
	};
	
	template <class T>
	Quaternion<T>::Quaternion()
	{
		x = y = z = w = 0;
	}
	
	template <class T>
	Quaternion<T>::Quaternion(T a, T b, T c, T d)
	{
		x = a * sin(d / 2.f);
		y = b * sin(d / 2.f);
		z = c* sin(d / 2.f);
		w = cos(d / 2.f);
	
		//x = a;
		//y = b;
		//z = c;
		//		w = d;
	}
	
	
	template <class T>
	void Quaternion<T>::Normalize()
	{
		T l = ((x*x) + (y*y) + (z*z) + (w*w));
		l = sqrt(l);
		if (l == 0)
		{
			return;
		}
		x /= l;
		y /= l;
		z /= l;
		w /= l;
	}
	
	template <class T>
	Quaternion<T> Quaternion<T>::GetConjugate(const Quaternion<T>& a)
	{
		Quaternion<T> q;
		q.x = -a.x;
		q.y = -a.y;
		q.z = -a.z;
		q.w = a.w;
		return q;
	}
	
	template <class T>
	Quaternion<T> operator+(const Quaternion<T>& a, const Quaternion<T>& b)
	{
		Quaternion<T> q;
	
		q.x = a.x + b.x;
		q.y = a.y + b.y;
		q.z = a.z + b.z;
		q.w = a.w + b.w;
	
		return q;
	}
	
	template <class T>
	Quaternion<T> operator*(const Quaternion<T>& A, const Quaternion<T> &B)
	{
		Quaternion<T> C;
	
		C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
		C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
		C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
		C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
	
		return C;
	}


}
typedef Corgi::Quaternion<float> Quat;



