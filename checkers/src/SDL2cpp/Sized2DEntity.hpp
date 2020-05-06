
#pragma once

namespace asa
{

    template <typename Derived_t>
    class Sized2DEntity
    {
    private:
        int m_width = -1;
        int m_height = -1;

    public:
        int height(void) const noexcept
        {
            return m_height;
        }

        int width(void) const noexcept
        {
            return m_width;
        }

        void height(int v) noexcept
        {
            m_height = v;
        }

        void width(int v) noexcept
        {
            m_width = v;
        }
    };

}; // namespace asa
