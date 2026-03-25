#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <iostream>

/*
* we are going to use vectors like
* vec1.add(vec)
* vec1 + vec2
* vec1 += vec2 likewise,
* thats the main idea and we are chasing that
* goal within this Vector2D class
*/

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	/*
	* the reason of not using friend keyword in this operator
	* overload statements is the friend funtions are not member
	* functions and they have no access to private members of
	* the class. but the following overloads are member
	* functions which have access to the private members
	* of the class.
	*
	* consider the following example
	* vec1 += vec2;
	* then vec1 is the object which calls the function and
	* it's x and y values should be changed accordingly.
	* in this case we have to use member function operator
	* overload instead of friend functions since the member
	* functions has access to the private members of the class.
	*/
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	Vector2D& operator+=(const float& val);
	Vector2D& operator-=(const float& val);
	Vector2D& operator*=(const float& val);
	Vector2D& operator/=(const float& val);

	Vector2D& operator*(const int& i);
	Vector2D& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};

#endif