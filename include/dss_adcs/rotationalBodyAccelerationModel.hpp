/*
 * Copyright (c) 2018 S.D. Cherukuri (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef ROTATIONAL_BODY_ACCELERATION_MODEL_HPP
#define ROTATIONAL_BODY_ACCELERATION_MODEL_HPP

#include <cmath>

#include <sml/sml.hpp>

// #include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{

//! Compute the rotational acceleration of a point mass body orbiting a uniform central body.
/*!
 * Computes the acceleration of a point mass body orbiting a uniform central body based on Euler's
 * moment equation.
 *
 * The expression of this acceleration is based on the two-body equation of motion, which represents
 * the relative equation of motion of a body as it orbits the central body.
 * \f[
 *      \vec{a}_{gravity}=-\frac{\mu}{r^{3}}\vec{r}
 * \f]
 *
 * where \f$\mu\f$ is a gravitational parameter (e.g., \f$\mu=GM=398,600.5 km^{3}s^-{2}\f$
 * in the case of the Earth), \f$r\f$ is the distance from the origin of the reference frame to the
 * body (i.e., the magnitude of the position vector \f$\vec{r}\f$), and \f$\vec{r}\f$ is the
 * position vector of the body relative to the origin of the reference frame.
 *
 * @tparam     Real                           Real type
 * @tparam     Vector3                        3-vector type
 * @param[in]  gravitationalParameter         Gravitational parameter of central body  [km^3 s^-2]
 * @param[in]  position                       Position vector of the orbiting body     [km]
 * @return                                    Acceleration vector                      [km s^-2]
 */
template< typename Inertia, typename Vector3 >
Vector3( const Inertia     principleInertia,
                                        const Vector3& angularVelocity )
{
    Vector3 acceleration = angularVelocity;

    acceleration[ 0 ] = ( principleInertia[1] - principleInertia[2] ) / ( angularVelocity[1]*angularVelocity[2] ); 
    acceleration[ 1 ] = ( principleInertia[2] - principleInertia[0] ) / ( angularVelocity[2]*angularVelocity[0] );
    acceleration[ 2 ] = ( principleInertia[0] - principleInertia[1] ) / ( angularVelocity[0]*angularVelocity[1] ); 

    return acceleration;
}

} // namespace astro

#endif //  ROTATIONAL_BODY_ACCELERATION_MODEL_HPP
