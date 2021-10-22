#ifndef H_TYPES
#define H_TYPES

#include <math.h>
#include <iostream>
#include <ostream>
#include <sstream>

namespace ZeroVulkan 
{
#define ZTYPE_COUNT 5
	enum class ZType
		: uint32_t
	{
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT4
	};

    inline const char* zTypeToStr(ZType type) {
        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new cases)");
        
        switch (type)
        {
        case ZType::MAT4:
            return "MAT4";
            break;
        case ZType::VEC4:
            return "VEC4";
            break;
        case ZType::VEC3:
            return "VEC3";
            break;
        case ZType::VEC2:
            return "VEC2";
            break;
        case ZType::FLOAT:
            return "FLOAT";
            break;
        default:
            printf("ERROR: unknown ZType %d\n", (int)type);
            return "\0";
        }
    }
    
#define ZTOPOLOGY_COUNT 3
    enum class ZTopology 
        : uint32_t
    {
        TRIANGLE,
        LINE,
        POINT
    };

	template<typename T, short size>
	struct vec
	{
		T values[size];

		vec()
			: values{ 0 }
		{}

		vec(T x)
			: values{ x }
		{}

		vec(T x, T y)
			: values{ x, y }
		{}

		vec(T x, T y, T z)
			: values{ x, y, z }
		{}

		vec(T x, T y, T z, T w)
			: values{ x, y, z, w }
		{}

		template<short otherSize>
		void operator= (vec<T, otherSize> v)
		{
			for (short i = 0; i < size; i++)
				values[i] = v[i];
		}

		template<short otherSize>
		void operator= (vec<T, otherSize> v) const
		{
			for (short i = 0; i < size; i++)
				values[i] = v[i];
		}

		vec<T, size> operator+ (T scalar)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] + scalar;

			return res;
		}
		const vec<T, size> operator+ (T scalar) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] + scalar;

			return res;
		}

		vec<T, size> operator- (T scalar)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] - scalar;

			return res;
		}
		const vec<T, size> operator- (T scalar) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] - scalar;

			return res;
		}

		vec<T, size> operator* (T scalar)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] * scalar;

			return res;
		}
		const vec<T, size> operator* (T scalar) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] * scalar;

			return res;
		}
		
		vec<T, size> operator/ (T scalar)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] / scalar;

			return res;
		}
		const vec<T, size> operator/ (T scalar) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] / scalar;

			return res;
		}


		vec<T, size> operator* (vec<T, size> v1)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] * v1[i];

			return res;
		}
		const vec<T, size> operator* (vec<T, size> v1) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] * v1[i];

			return res;
		}

		vec<T, size> operator/ (vec<T, size> v1)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] / v1[i];

			return res;
		}
		const vec<T, size> operator/ (vec<T, size> v1) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] / v1[i];

			return res;
		}

		vec<T, size> operator+ (vec<T, size> v1)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] + v1[i];

			return res;
		}
		const vec<T, size> operator+ (vec<T, size> v1) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] + v1[i];

			return res;
		}

		vec<T, size> operator- (vec<T, size> v1)
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] - v1[i];

			return res;
		}
		const vec<T, size> operator- (vec<T, size> v1) const
		{
			vec<T, size> res = vec<T, size>();

			for (short i = 0; i < size; i++)
				res[i] = values[i] - v1[i];

			return res;
		}


		void operator+= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
				values[i] += v1[i];
		}

		void operator-= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
				values[i] -= v1[i];
		}

		void operator*= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
				values[i] *= v1[i];
		}

		void operator/= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
				values[i] /= v1[i];
		}


		void operator+= (const T v)
		{
			for (short i = 0; i < size; i++)
				values[i] += v;
		}

		void operator-= (const T v)
		{
			for (short i = 0; i < size; i++)
				values[i] -= v;
		}

		void operator*= (const T v)
		{
			for (short i = 0; i < size; i++)
				values[i] *= v;
		}

		void operator/= (const T v)
		{
			for (short i = 0; i < size; i++)
				values[i] /= v;
		}


		T& operator[] (int i)
		{
			return values[i];
		}
		const T& operator[] (int i) const
		{
			return values[i];
		}


		bool operator< (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
			{
				if(values[i] >= v1[i])
					return false;
			}

			return true;
		}

		bool operator> (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
			{
				if(values[i] <= v1[i])
					return false;
			}

			return true;
		}

		bool operator<= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
			{
				if(values[i] > v1[i])
					return false;
			}

			return true;
		}

		bool operator>= (vec<T, size> v1)
		{
			for (short i = 0; i < size; i++)
			{
				if(values[i] < v1[i])
					return false;
			}

			return true;
		}
	};

	typedef vec<float, 4> vec4;

	typedef vec<float, 3> vec3;

	typedef vec<float, 2> vec2;


	template<typename T, short sizeX, short sizeY>
	struct mat
	{
		vec<T, sizeX> values[sizeY];

		mat()
			: values{ 0 }
		{}

		mat(T s)
			: values{ 0 }
		{
			for (short i = 0; i < sizeX; i++)
				values[i][i] = s;
		}

		mat(T s1, T s2, T s3)
			: values{
				vec4(s1, 0, 0, 0),
				vec4(0, s2, 0, 0),
				vec4(0, 0, s3, 0),
				vec4(0, 0, 0, 1)
		}
		{}


		template<short X, short Y>
		mat<T, X, Y> resize()
		{
			mat<T, X, Y> res;

			for (short i = 0; i < X; i++)
			{
				for (short j = 0; j < Y; j++)
					res[i][j] = values[i][j];
			}

			return res;
		}

		mat<T, sizeX, sizeY> inverse()
		{
			mat<T, sizeX, sizeY> res;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					res[i][j] = (
						( values[(j + 1) % 3][(i + 1) % 3] * values[(j + 2) % 3][(i + 2) % 3] )
						- ( values[(j + 1) % 3][(i + 2) % 3] * values[(j + 2) % 3][(i + 1) % 3] )
					) / deteminant();
				}
			}

			return res;
		}

		float deteminant()
		{
			float d = 0.f;

			for (int i = 0; i < 3; i++)
			{
				d += (
					values[0][i] * 
					( values[1][(i + 1) % 3] * values[2][(i + 2) % 3] 
						- values[1][(i + 2) % 3] * values[2][(i + 1) % 3] ) );
			}

			return d;
		}

		mat<T, sizeX, sizeY> adjoint()
		{

		}

		mat<T, sizeX, sizeY> transpose() 
		{
			mat<T, sizeY, sizeX> res;

			for (uint32_t y = 0; y < sizeY; y++)
			{
				for (uint32_t x = 0; x < sizeX; x++)
				{
					res[x][y] = values[y][x];
				}
			}

			return res;
		}


		vec<T, sizeX>& operator[] (int i)
		{
			return this->values[i];
		}
		const vec<T, sizeX>& operator[] (int i) const
		{
			return this->values[i];
		}

		vec<T, sizeX> operator* (vec<T, sizeX> v1)
		{
			vec<T, sizeX> vres;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					vres[y] += this->values[y][x] * v1[x];
			}

			return vres;
		}
		const vec<T, sizeX> operator* (vec<T, sizeX> v1) const
		{
			vec<T, sizeX> vres;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					vres[y] += this->values[y][x] * v1[x];
			}

			return vres;
		}

		mat<T, sizeX, sizeY> operator+ (mat<T, sizeX, sizeY> m)
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					res[y][x] = m[y][x] + this->values[y][x];
			}

			return res;
		}
		const mat<T, sizeX, sizeY> operator+ (mat<T, sizeX, sizeY> m) const
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					res[y][x] = m[y][x] + this->values[y][x];
			}

			return res;
		}

		mat<T, sizeX, sizeY> operator* (mat<T, sizeX, sizeY> m)
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				res[y] = vec<T, sizeX>();

				for (short x = 0; x < sizeX; x++)
					res[y] += this->values[x] * m[y][x];
			}

			return res;
		}
		const mat<T, sizeX, sizeY> operator* (mat<T, sizeX, sizeY> m) const
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				res[y] = vec<T, sizeX>();

				for (short x = 0; x < sizeX; x++)
					res[y] += this->values[x] * m[y][x];
			}
			
			return res;
		}

		mat<T, sizeX, sizeY> operator* (T s)
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					res[x][y] = this->values[x][y] * s;
			}

			return res;
		}
		const mat<T, sizeX, sizeY> operator* (T s) const
		{
			mat<T, sizeX, sizeY> res;

			for (short y = 0; y < sizeY; y++)
			{
				for (short x = 0; x < sizeX; x++)
					res[x][y] = this->values[x][y] * s;
			}

			return res;
		}

		void operator= (vec<T, sizeX> v)
		{
			for (short i = 0; i < sizeX; i++)
				this->values[i][i] = v[i];
		}

		void operator= (T f)
		{
			for (short i = 0; i < sizeof(this->values) / sizeof(T); i++)
				this->values[i][i] = f;
		}

		void operator+= (vec<T, sizeX> v)
		{
			for (short i = 0; i < sizeX; i++)
				this->values[i][i] += v[i];
		}

		void operator*= (mat<T, sizeX, sizeY> m)
		{
			for (short y = 0; y < sizeY; y++)
			{
				this->values[y] = this->values[0] * m[y][0];

				for (short x = 1; x < sizeX; x++)
					this->values[y] += this->values[x] * m[y][x];
			}
		}
	};

	typedef mat<float, 4, 4> mat4;

	typedef mat<float, 3, 3> mat3;

	typedef mat<float, 2, 2> mat2;


	// ostringstream caused compiler errors with const vec/mat
	// ostringstream derives from ostream and ostream supports const vec/mat properly
	// TODO: find out why exactly compiler errors
	inline std::ostream& operator<< (std::ostream& stream, const vec2& v)
	{
		stream << "vec2 { " << v.values[0] << ", " << v.values[1] << " }";
		return stream;
	}

	inline std::ostream& operator<< (std::ostream& stream, const vec3& v)
	{
		stream << "vec3 { " << v.values[0] << ", " << v.values[1] << ", " << v.values[2] << " }";
		return stream;
	}

	inline std::ostream& operator<< (std::ostream& stream, const vec4& v)
	{
		stream << "vec4 { " << v.values[0] << ", " << v.values[1] << ", " << v.values[2] << ", " << v.values[3] << " }";
		return stream;
	}


	inline std::ostream& operator<< (std::ostream& stream, const mat2& m)
	{
		stream << "mat4 {" << "\n\t";
		stream << m[0] << "\n\t";
		stream << m[1] << "\n}";
		return stream;
	}

	inline std::ostream& operator<< (std::ostream& stream, const mat3& m)
	{
		stream << "mat4 {" << "\n\t";
		stream << m[0] << "\n\t";
		stream << m[1] << "\n\t";
		stream << m[2] << "\n}";
		return stream;
	}

	inline std::ostream& operator<< (std::ostream& stream, const mat4& m)
	{
		stream << "mat4 {" << "\n\t";
		stream << m[0] << "\n\t";
		stream << m[1] << "\n\t";
		stream << m[2] << "\n\t";
		stream << m[3] << "\n}";
		return stream;
	}


	//TODO: cross for vec2 and vec4
	inline vec3 cross(const vec3& v1, const vec3& v2)
	{
		vec3 res;

		res[0] = v1[1] * v2[2] - v2[1] * v1[2];
		res[1] = v1[2] * v2[0] - v2[2] * v1[0];
		res[2] = v1[0] * v2[1] - v2[0] * v1[1];

		return res;
	}

	template<short size>
	inline float dot(const vec<float, size>& v1, const vec<float, size>& v2)
	{
		float res = 0;

		for (short i = 0; i < size; i++)
			res += v1[i] * v2[i];

		return res;
	}

	template<short size>
	inline float length(const vec<float, size>& v)
	{
		float sum = 0.f;

		for (short i = 0; i < size; i++)
			sum += std::pow(v[i], 2);

		return std::sqrt(sum);
	}

	template<short size>
	inline vec<float, size> normal(const vec<float, size>& v)
	{
		float len = length(v);

		if(len)
			return v / len;
		else
			return v;
	}

	template<short size>
	inline vec<float, size> abs(const vec<float, size>& v)
	{
		vec<float, size> res;

		for (short i = 0; i < size; i++)
			res[i] = std::abs(v[i]);

		return res;
	}

	inline double radian(double degree)
	{
		return degree * M_PI / 180.f;
	}

	inline double degree(double radian)
	{
		return radian / M_PI * 180.f;
	}

	// angle in Degree
	inline vec3 rotate(const vec3& v, const vec3& axis, double angle)
	{
		angle = radian(angle);
		
		double cosA = std::cos(angle);
		double sinA = std::cos(angle);
		
		vec3 res = (v * cosA) 
			+ ( cross(axis, v) * sinA )
			+ ( axis * dot(axis, v) ) * (1 - cosA);
		return res;
	}

	// angle in Degree
	inline vec3 rotate(const vec3& v, const vec3& angles)
	{
		double X = radian(angles[0]);
		double Y = radian(angles[1]);
		double Z = radian(angles[2]);

		//rotation on x-Axis
		mat3 xRot(1.f);
		xRot[1][1] = std::cos(X);
		xRot[1][2] = -std::sin(X);
		xRot[2][1] = std::sin(X);
		xRot[2][2] = std::cos(X);

		//rotation on y-Axis
		mat3 yRot(1.f);
		yRot[0][0] = std::cos(Y);
		yRot[0][2] = std::sin(Y);
		yRot[2][0] = -std::sin(Y);
		yRot[2][2] = std::cos(Y);

		//rotation on z-Axis
		mat3 zRot(1.f);
		zRot[0][0] = std::cos(Z);
		zRot[0][1] = -std::sin(Z);
		zRot[1][0] = std::sin(Z);
		zRot[1][1] = std::cos(Z);

		vec3 res = xRot * yRot * zRot * v; // XYZ-rotation (order is important)

		return res;
	}
}

#endif // H_TYPES
