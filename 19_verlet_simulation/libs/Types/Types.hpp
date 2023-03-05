#pragma once

namespace types {

	template <class T>
	class Vector2 {
		/*
			i took inspiration from "https://gist.github.com/acidleaf/8957147"
			for this class
		*/
	public:
		T x;
		T y;

		Vector2(): Vector2(0, 0) {}
		Vector2(const Vector2 &vec): Vector2(vec.x, vec.y) {}
		Vector2(T x, T y) {
			this->x = x;
			this->y = y;
		}
		~Vector2() {
			// do nothing
		}

		Vector2& operator+=(double val) {
			x += val;
			y += val;
			return *this;
		}

		Vector2& operator-=(double val) {
			x -= val;
			y -= val;
			return *this;
		}

		Vector2& operator*=(double val) {
			x *= val;
			y *= val;
			return *this;
		}

		Vector2& operator/=(double val) {
			x /= val;
			y /= val;
			return *this;
		}

		Vector2 operator+(const double &val) {
			return Vector2(x + val, y + val);
		}

		Vector2 operator-(const double &val) {
			return Vector2(x - val, y - val);
		}
		Vector2 operator*(const double &val) {
			return Vector2(x * val, y * val);
		}

		Vector2 operator/(const double &val) {
			return Vector2(x / val, y / val);
		}

		Vector2& operator+=(const Vector2 &vec) {
			x += vec.x;
			y += vec.y;
			return *this;
		}

		Vector2& operator-=(const Vector2 &vec) {
			x -= vec.x;
			y -= vec.y;
			return *this;
		}

		Vector2& operator*=(const Vector2 &vec) {
			x *= vec.x;
			y *= vec.y;
			return *this;
		}

		Vector2& operator/=(const Vector2 &vec) {
			x /= vec.x;
			y /= vec.y;
			return *this;
		}

		Vector2 operator+(const Vector2 &vec) {
			return Vector2(x + vec.x, y + vec.y);
		}

		Vector2 operator-(const Vector2 &vec) {
			return Vector2(x - vec.x, y - vec.y);
		}

		Vector2 operator*(const Vector2 &vec) {
			return Vector2(x * vec.x, y * vec.y);
		}

		Vector2 operator/(const Vector2 &vec) {
			return Vector2(x / vec.x, y / vec.y);
		}

		Vector2& operator=(const Vector2 &vec) {
			x = vec.x;
			y = vec.y;
			return *this;
		}

		void set(T x, T y) {
			this->x = x;
			this->y = y;
		}
	
	};
}