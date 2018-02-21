/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_RUNGE_KUTTA_HPP
#define DSS_ADCS_RUNGE_KUTTA_HPP 

#include <boost/array.hpp>

#include <sml/sml.hpp>
#include <astro/astro.hpp>

// #include "dss_adcs/typedefs.hpp"
namespace dss_adcs
{

//! Integrate a given ordinary differential function from a start time to a final time using 4th order runge kutta. 
/*! Integrates the functions from a start time to an end time with a given stepsize 
*   based on 4th order Runge kutta mehtod as per defined in http://lpsa.swarthmore.edu/NumInt/NumIntFourth.html. 
*   The expression of this integrator is based on a method of numerically integrating 
*   time dependent ordinary differential equations (ODE) by using four approximations of the slope. 
*   \f[
*      \frac{\partial y(t)}{\partial t} = y^{\prime}(t) = f(y(t), t)   
*   \f]        
*   where, \f$\frac{\partial y(t)}{\partial t}\f$ represents a time dependent 
*   partial differential equation. The solution to the ODE will be given as:   
*   \f[        
*       k_1 = f(y(t_o))
*       k_2 = f(y(t_o) + \frac{h}{2}k_1, t_o + \frac{h}{2})
*       k_3 = f(y(t_o) + \frac{h}{2}k_2, t_o + \frac{h}{2})
*       k_4 = f(y(t_o) + k_3, t_(o) + h)                    
*   \f] 
*   where, \f[k_1\f] is the slope at the starting of the timestep,
*   \f[k_2\f] is the slope at the middle of the timestep,
*   \f[k_3\f] is the slope is another estimate at the middle of the timestep, 
*   \f[k_4\f] is the slope at the end of timestep,
*   \f[h\f] is the timestep given for integration,
*   \f[y(t_o)\f] is the known initial condition.  
*   \f[
*       y(t_o+h) = y(t_o) + \frac{k_1 + 2k_2 + 2k_3 + k_4}{6}h
*   \f]
*   where, \f[y(t_o+h)\f] is the numerical solution of the differential equation at the 
*   timestep \f[t_o+h\f]. 
*/  
template< typename Function, typename IntegratorSettings >
void performRungeKutta4Integration( Function dynamics, 
                                       const IntegratorSettings input )
{
    typedef boost::array< double, 6 > Vector6;
    typedef boost::array< double, 6 > Derivative;

    const double timeStep       = input.timeStep; 
    const double startEpoch     = input.startEpoch; 
    const double endEpoch       = input.endEpoch; 
    const Vector6 initialState  = input.initialAttitudeState; 

    Vector6 currentState        = initialState; 
    Vector6 stateDerivative     = {0, 0, 0, 0, 0, 0};

    dynamics( currentState, stateDerivative );
    Derivative k1               = stateDerivative; 
    
    dynamics( sml::add( currentState, sml::multiply( k1, (timeStep/2) ) ), stateDerivative );
    Derivative k2               = stateDerivative; 
    
    dynamics( sml::add( currentState, sml::multiply( k2, (timeStep/2) ) ), stateDerivative );
    Derivative k3               = stateDerivative;

    dynamics( sml::add( currentState, k3 ), stateDerivative );
    Derivative k4               = stateDerivative;

    Derivative derivative       = sml::add( k1, sml::multiply(k2, 2), sml::multiply(k3, 2), k4 );
    currentState                = sml::add( currentState, sml::multiply( derivative, (timeStep/6) ) ); 

}
  
} // namespace dss_adcs

#endif 