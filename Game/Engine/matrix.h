#pragma once

template<size_t m, size_t n>
class matrix
{
    friend class matrix;

private:
    float elements[m][n];

public:
    template<typename... type>
    matrix(type const &... elements) : elements { static_cast<float const>(elements)... }
    {
        static_assert(not (m == 1 and n == 1), "matrix<1, 1> cannot be created.");
    }

    auto const operator *(float const & c) const
    {
        matrix<m, n> product = *this;

        for(size_t i = 0; i < m; ++i)
            for(size_t j = 0; j < n; ++j)
                product.elements[i][j] *= c;

        return product;
    }

    auto & operator *=(float const & c)
    {
        return *this = *this * c;
    }

    auto const operator /(float const & c) const
    {
        return *this * (1 / c);
    }

    auto & operator /=(float const & c)
    {
        return *this = *this / c;
    }

    auto const operator +(void) const
    {
        return *this;
    }

    auto const operator +(matrix<m, n> const & M) const
    {
        matrix<m, n> product = *this;

        for(size_t i = 0; i < m; ++i)
            for(size_t j = 0; j < n; ++j)
                product.elements[i][j] += M.elements[i][j];

        return product;
    }

    auto & operator +=(matrix<m, n> const & M)
    {
        return *this = *this + M;
    }
    
    auto const operator -(void) const
    {
        return *this * -1;
    }

    auto const operator -(matrix<m, n> const & M) const
    {
        return *this + (-M);
    }

    auto & operator -=(matrix<m, n> const & M)
    {
        return *this = *this - M;
    }

    template<size_t l>
    auto const operator *(matrix<n, l> const & M) const
    {
        if constexpr (m == 1 and l == 1)
        {
            float product = float();

            for(size_t i = 0; i < n; ++i)
                product += (*this).elements[0][i] * M.elements[i][0];

            return product;
        }
        else
        {
            matrix<m, l> product = matrix<m, l>();

            for(size_t i = 0; i < m; ++i)
                for(size_t j = 0; j < l; ++j)
                    for(size_t k = 0; k < n; ++k)
                        product.elements[i][j] += (*this).elements[i][k] * M.elements[k][j];

            return product;
        }
    }

    auto & operator *=(matrix<n, n> const & M)
    {
        return *this = *this * M;
    }

    auto & operator [](size_t const & i)
    {
             if constexpr (m == 1 and n != 1) return elements[0][i];
        else if constexpr (m != 1 and n == 1) return elements[i][0];
        else                                  return elements[i];
    }

    auto const & operator [](size_t const & i) const
    {
        return const_cast<matrix<m, n> &>(*this)[i];
    }
};

template<size_t m, size_t n>
inline matrix<n, m> const transpose(matrix<m, n> const & M)
{
    if constexpr (m == 1 or n == 1)
    {
        return reinterpret_cast<matrix<n, m> const &>(M);
    }
    else
    {
        matrix<n, m> product = matrix<n, m>();

        for(size_t i = 0; i < n; ++i)
            for(size_t j = 0; j < m; ++j)
                product[i][j] = M[j][i];

        return product;
    }
}