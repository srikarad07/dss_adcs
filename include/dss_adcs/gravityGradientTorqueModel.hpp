/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef GRAVITY_GRADIENT_TORQUE_MODEL_HPP
#define GRAVITY_GRADIENT_TORQUE_MODEL_HPP


/*  Gravity gradient torque is experienced by the orbiting body at it's center of mass 
*   due to the fact that gravity field is not uniform over the body in space. This effect 
*   was first considered by D'Lambert and Euler in 1749. This effect was later used by 
*   Lambert to explain the reason for moon to face the earth with it's same side.   
*/

namespace dss_adcs
{

//  Compute the gravity gradient torque on orbiting body under the inlfuence of uniform 
//  gravity field of a central body. 
/*  Computes the gravity gradient torque acting on a rigid orbiting body under the 
*   influence of a uniform gravity field of a rotating body.
*    
*   The expression of this torque is defined as per 'Spacecraft Dynamics and Control:
*   A practical approach' (Marcel J. Sidl). The expression is based on assymetry of the 
*   orbitng body is expressed in terms of inertial reference frame. 
*   \f[
*           \vec{T_{gx}} = \frac{3 \mu}{R^3}(J_z - J_y)C_{yz}C_{zz} \\
*           \vec{T_{gy}} = \frac{3 \mu}{R^3}(J_z - J_x)C_{xz}C_{zz} \\
*           \vec{T_{gx}} = \frac{3 \mu}{R^3}(J_x - J_y)C_{xz}C_{yz}
*   \f] 
*   where $\vec{T_{gi}}$ is the gravity gradient torque in $i = [x, y, z]$ axes, $\mu$
*   is the gravitational parameter of the central body, $R$ is the instantenous radius
*   of the orbiting body from the central body, $J_i$ is the principle inertia value around
*   $i^{th}$ axis and $C_{ij}$ is the value of the direction cosine matrix for the
*   transformation from body fixed reference frame to newtonian inertial reference frame.  
*
*   @tparam         Real                        Real type
*   @tparam         Vector3                     3-vector type
*   @tparam         Matrix33                   3x3 matrix type
*   @param[in]      gravitationalParamater      Gravitational Parameter of the central body 
*                                                                   [km^3 s⁻2]
*   @param[in]      radius                      Instantanious radial distance of the orbiting
*                                               body from the central body [km]
*   @param[in]      prinicpleInertia            Prinicple Inertia of the rotating body [kg m^2]
*   @param[in]      directionCosineMatrix       Direction cosine matrix []
*/ 

template< typename Real, typename Vector3, typename Matrix33 >
Vector3 computeGravityGradientTorque( const Real        gravitationalParameter,
                                      const Real        radius,
                                      const Vector3     principleInertia,
                                      const Matrix33   directionCosineMatrix )
{
    Vector3 gravityGradientTorque;
    Vector3 postMultiplier; 
    postMultiplier[0]               = ( principleInertia[3] - principleInertia[2] ) * 
                                                directionCosineMatrix(2,3) *  
                                                        directionCosineMatrix(3,3);
    postMultiplier[1]               = ( principleInertia[3] - principleInertia[1] ) * 
                                                directionCosineMatrix(1,3) *  
                                                        directionCosineMatrix(3,3);
    postMultiplier[2]               = ( principleInertia[1] - principleInertia[2] ) * 
                                                directionCosineMatrix(1,3) *  
                                                        directionCosineMatrix(2,3);   
    gravityGradientTorque[0]        = ( 3*gravitationalParameter / (2*radius) ) * postMultiplier[0];
    gravityGradientTorque[0]        = ( 3*gravitationalParameter / (2*radius) ) * postMultiplier[1];
    gravityGradientTorque[0]        = ( 3*gravitationalParameter / (2*radius) ) * postMultiplier[2];

    std::cout << "Gravity gradient torque: " << gravityGradientTorque[0] << std::endl;    
    return gravityGradientTorque;          
}


} // namespace astro

#endif
