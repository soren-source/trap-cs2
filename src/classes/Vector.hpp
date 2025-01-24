#pragma once
#include <string>
#include <sstream>

class Vector2;

class Vector3 {
public:
	Vector3( ) : x( 0.f ), y( 0.f ), z( 0.f ) { };
	Vector3( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ) { };
	float x, y, z;

	Vector3 operator+( const Vector3& other )
	{
		Vector3 vec;
		vec.x = this->x + other.x;
		vec.y = this->y + other.y;
		vec.z = this->z + other.z;
		return vec;
	}

	Vector3 operator-( const Vector3& other )
	{
		Vector3 vec;
		vec.x = this->x - other.x;
		vec.y = this->y - other.y;
		vec.z = this->z - other.z;
		return vec;
	}

	Vector3 operator*( const Vector3& other )
	{
		Vector3 vec;
		vec.x = this->x * other.x;
		vec.y = this->y * other.y;
		vec.z = this->z * other.z;
		return vec;
	}

	Vector3 operator*( const float& other )
	{
		Vector3 vec;
		vec.x = this->x * other;
		vec.y = this->y * other;
		vec.z = this->z * other;
		return vec;
	}

	Vector3 operator*( float& other )
	{
		Vector3 vec;
		vec.x = this->x * other;
		vec.y = this->y * other;
		vec.z = this->z * other;
		return vec;
	}

	bool operator==( const Vector3& other )
	{
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	bool operator!=( const Vector3& other )
	{
		return this->x != other.x || this->y != other.y || this->z != other.z;
	}

	[[nodiscard]]
	std::string ToString( ) const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str( );
	}

	[[nodiscard]]
	Vector3 Multiply( float diff ) {
		Vector3 vec;
		vec.x = this->x * diff;
		vec.y = this->y * diff;
		vec.y = this->y * diff;
		return vec;
	}

	friend std::ostream& operator<<( std::ostream& os, const Vector3& vec )
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	[[nodiscard]]
	float DistanceTo( Vector3 to )
	{
		float _x = x - to.x;
		float _y = y - to.y;
		float _z = z - to.z;
		return sqrtf( _x * _x + _y * _y + _z * _z );
	}

	[[nodiscard]]
	Vector3 Sub( Vector3 v2 )
	{
		return Vector3( x - v2.x, y - v2.y, z - v2.z );
	}

	[[nodiscard]]
	Vector3 Scale( float factor )
	{
		return Vector3( x * factor, y * factor, z * factor );
	}

	bool ToScreen( Vector2& to );
};


class Vector2
{
public:
	Vector2( ) : x( 0.f ), y( 0.f ) {};
	Vector2( float _x, float _y ) : x( _x ), y( _y ) {};

	float DistanceTo( const Vector2& to ) 
	{
		float _x = x - to.x;
		float _y = y - to.y;

		return sqrtf( _x * _x + _y * _y );
	}

	float x, y;
};