
#pragma once

#include <cmath>
#include <algorithm>

namespace asa {

    template<typename T>
    class vector2d {
    private:
        T m_x, m_y;
    public:
        vector2d(T x, T y) : m_x(x), m_y(y) {}

        constexpr T x(void) const noexcept { return m_x; }
        constexpr T y(void) const noexcept { return m_y; }

        constexpr void x(T val) noexcept { m_x = val; }
        constexpr void y(T val) noexcept { m_y = val; }

        vector2d operator +(vector2d &&a) noexcept {
            vector2d result;
            result.m_x = this->m_x + a.m_x;
            result.m_y = this->m_y + a.m_y;
            return result;
        }

        vector2d operator +(vector2d &a) noexcept {
            return vector2d(this->m_x + a.m_x, this->m_y + a.m_y);
        }

        void operator +=(vector2d &&a) noexcept {
            this->m_x += a.m_x;
            this->m_y += a.m_y;
        }

        void operator +=(vector2d &a) noexcept {
            this->m_x += a.m_x;
            this->m_y += a.m_y;
        }

        vector2d operator -(vector2d &&a) noexcept {
            return vector2d(this->m_x - a.m_x, this->m_y - a.m_y);
        }

        vector2d operator -(vector2d &a) noexcept {
            return vector2d(this->m_x - a.m_x, this->m_y - a.m_y);
        }

        void operator -=(vector2d &&a) noexcept {
            this->m_x -= a.m_x;
            this->m_y -= a.m_y;
        }

        void operator -=(vector2d &a) noexcept {
            this->m_x -= a.m_x;
            this->m_y -= a.m_y;
        }

        vector2d operator *(T a) noexcept {
            return vector2d(this->m_x * a, this->m_y * a);
        }

        void operator *=(T a) noexcept {
            this->m_x *= a.m_x;
            this->m_y *= a.m_y;
        }

        vector2d operator /(T a) noexcept {
            return vector2d(this->m_x / a, this->m_y / a);
        }

        void operator /=(T a) noexcept {
            this->m_x *= a.m_x;
            this->m_y *= a.m_y;
        }

        T magnitude(void) const noexcept {
            double dx = static_cast<double>(this->m_x);
            double dy = static_cast<double>(this->m_y);
            T result = static_cast<T>(std::sqrt(dx * dx + dy * dy));
            return result;
        }

        T euclidean_distance(const vector2d &other) const noexcept {
            double ax = static_cast<double>(this->m_x);
            double ay = static_cast<double>(this->m_y);
            double bx = static_cast<double>(other.m_x);
            double by = static_cast<double>(other.m_y);
            return std::sqrt(((bx - ax) * (bx - ax)) + ((by - ay) * (by - ay)));
        }

        T chebyshev_distance(const vector2d &other) const noexcept {
            return std::max(std::abs(other.m_x - m_x), std::abs(other.m_x - m_x));
        }

        vector2d normalized(void) noexcept {
            auto m = magnitude();
            return (*this) / m;
        }

        T dot(const vector2d &other) const noexcept {
            return m_x * other.m_x + m_y * other.m_y;
        }

    };

}; // namespace asa
