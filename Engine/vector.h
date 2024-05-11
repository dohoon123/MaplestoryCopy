#pragma once

#include <cmath>

#include "matrix.h"

template<size_t n>
using vector = matrix<1, n>;

template<size_t n>
inline float const dot(vector<n> const & u, vector<n> const & v)
{
    return u * transpose(v);
}

template<size_t n>
inline float const length(vector<n> const & v)
{
    return sqrt(dot(v, v));
}

template<size_t n>
inline vector<n> const normalize(vector<n> const & v)
{
    return v / length(v);
}

inline vector<3> const cross(vector<3> const & u, vector<3> const & v)
{
    vector<3> product = vector<3>();

    for(size_t i = 0; i < 3; ++i)
    {
        size_t const j = (i + 1) % 3;
        size_t const k = (j + 1) % 3;

        product[i] = u[j] * v[k] - u[k] * v[j];
    }

    return product;
}