/*
 * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <iomanip>
#include <iostream>

#include <astro/quaternionToEulerAngleTransformation.hpp>
#include <sml/sml.hpp>
#include <Eigen/Dense>

#include "dss_adcs/typedefs.hpp"

#ifndef DSS_ADCS_OUTPUT_WRITER_HPP
#define DSS_ADCS_OUTPUT_WRITER_HPP

namespace dss_adcs
{
typedef std::vector<double> VectorXdIntegration; 
//! Print value to stream.
/*!
 * Prints a specified value to stream provided, given a specified width and a filler character.
 *
 * @tparam     DataType  Type for specified value
 * @param[out] stream    Output stream
 * @param[in]  value     Specified value to print
 * @param[in]  width     Width of value printed to stream, in terms of number of characters
 *                       (default = 25)
 * @param[in]  filler    Character used to fill fixed-width (default = ' ')
 */
// template< typename DataType >
// inline void print( std::ostream& stream,
//                    const DataType value,
//                    const int width = 25,
//                    const char filler = ' ' )
// {
//     stream << std::left << std::setw( width ) << std::setfill( filler ) << value;
// }

template <typename Arg, typename... Args>
void doPrint(std::ostream& out, Arg&& arg, Args&&... args)
{
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(out << ',' << std::forward<Args>(args)),0)...};
}

//! Print metadata parameter to stream.
/*!
 * Prints metadata parameter to stream provided, given a specified name, value, units, and
 * delimiter.
 *
 * @tparam     DataType      Type for specified value
 * @param[out] stream        Output stream
 * @param[in]  parameterName Name for metadata parameter
 * @param[in]  value         Specified value to print
 * @param[in]  units         Units for value
 * @param[in]  delimiter     Character used to delimiter entries in stream (default = ' ')
 * @param[in]  width         Width of value printed to stream, in terms of number of characters
 *                           (default = 25)
 * @param[in]  filler        Character used to fill fixed-width (default = ' ')
 */
// template< typename DataType >
// inline void print( std::ostream& stream,
//                    const std::string& parameterName,
//                    const DataType value,
//                    const std::string& units,
//                    const char delimiter = ',',
//                    const int width = 25,
//                    const char filler = ' ' )
// {
//     print( stream, parameterName, width, filler );
//     stream << delimiter;
//     print( stream, value, width, filler );
//     stream << delimiter;
//     print( stream, units, width, filler );
// }

// template< typename DataType >
// inline void print( std::ostream& stream,
//                    const std::string& parameterName,
//                    const std::string& units,
//                    const std::string& parameterName2,
//                    const std::string& units2,
//                    const std::string& parameterName3,
//                    const std::string& units3,
//                    const std::string& parameterName4,
//                    const std::string& units4, 
//                    const char delimiter = ',',
//                    const int width = 25,
//                    const char filler = ' ' )
// {
//     print( stream, parameterName, width, filler );
//     stream << delimiter;
//     print( stream, value, width, filler );
//     stream << delimiter;
//     print( stream, units, width, filler );
// }

//! Class to write state history to an output stream.
/*!
 * This class contains parameters and functions to write the state history to a user-provided
 * output stream, e.g., console, file.
 */
class StateHistoryWriter
{
public:
    //! Construct state history writer.
    /*!
     * Constructor for state history writer to write state history to a user-defined output stream.
     *
     * This class writes the state history in Cartesian elements to file. In the same file, the
     * corresponding Keplerian (osculating) elements are included, by computing the conversion from
     * Cartesian to Keplerian elements.
     *
     * @param[in] aStateHistoryStream       Output stream
     * @param[in] aGravitationalParameter   Gravitation parameter of central body        [km^3 s^-2]
     */

    StateHistoryWriter( std::ostream&   aStateHistoryStream, 
                        const Vector3   aControlTorque, 
                        const VectorXd  aMotorTorque, 
                        const Vector3   aDisturbanceTorque, 
                        const VectorXd  aReactionWheelAngularVelocities )
        : stateHistoryStream( aStateHistoryStream ),
          controlTorque( aControlTorque ),
          motorTorque( aMotorTorque ), 
          disturbanceTorque( aDisturbanceTorque ), 
          reactionWheelAngularVelocities( aReactionWheelAngularVelocities )   
    { }

    //! Overload ()-operator to write state to output stream.
    /*!
     * Overloads ()-operator to write current state state and epoch to a user-defined output stream.
     * The output stream is defined through the class constructor.
     *
     * This function fulfills the prototype required to work with the Boost Odeint library.
     *
     * @sa boost::odeint:integrator
     * @param[in] state Current state
     * @param[in] time  Current epoch
     */
    void operator( )( const VectorXdIntegration& state, const double time )
    {
        const Vector4 quaternion( state[0], state[1], state[2], state[3] ); 
        const Vector3 eulerAngles       = astro::transformQuaternionToEulerAngles( quaternion ); 
        const Real eulerRotationAngle   = 2 * acos( state[3] ); 
        const Vector3 attitudeRate( state[4], state[5], state[6] ); 
        const Real slewRate             = attitudeRate.norm(); 
        const VectorXd powerConsumption = motorTorque * reactionWheelAngularVelocities; 

        stateHistoryStream  << std::setprecision ( std::numeric_limits< double>::digits10 )
                            << time                                                     << ','
                            << state[ 0 ]                                               << ','
                            << state[ 1 ]                                               << ','
                            << state[ 2 ]                                               << ','
                            << state[ 3 ]                                               << ','
                            << sml::convertRadiansToDegrees( eulerRotationAngle )       << ','
                            << sml::convertRadiansToDegrees( eulerAngles[0] )           << ','
                            << sml::convertRadiansToDegrees( eulerAngles[1] )           << ','
                            << sml::convertRadiansToDegrees( eulerAngles[2] )           << ','
                            // << sml::convertRadiansToDegrees( state[ 4 ] )               << ','
                            // << sml::convertRadiansToDegrees( state[ 5 ] )               << ','
                            // << sml::convertRadiansToDegrees( state[ 6 ] )               << ','
                            << state[ 4 ]                                               << ','
                            << state[ 5 ]                                               << ','
                            << state[ 6 ]                                               << ','
                            << sml::convertRadiansToDegrees( slewRate  )                << ','
                            << controlTorque[0]                                         << ','
                            << controlTorque[1]                                         << ',' 
                            << controlTorque[2]                                         << ',' ;
    for ( unsigned int iterator = 0; iterator < motorTorque.size(); ++iterator )
    {
        stateHistoryStream  << motorTorque[iterator]                                    << ',';
    }
    for ( unsigned int stateIterator = 7; stateIterator < state.size(); ++stateIterator )
    {
        stateHistoryStream  << state[stateIterator]                                     << ',';
    }
    for ( unsigned int velocityIterator = 0; velocityIterator < motorTorque.size(); ++velocityIterator )
    {
        //! Angular velocities obtained in sec-1, converted into rpm by multiplying with 60! 
        stateHistoryStream  << 60.0 * reactionWheelAngularVelocities[velocityIterator]  << ',';
    }
    for ( unsigned int powerIterator = 0; powerIterator < motorTorque.size(); ++powerIterator )
    {
        stateHistoryStream  << powerConsumption[powerIterator]                          << ',';
    }
        stateHistoryStream  << disturbanceTorque[0]                                     << ','
                            << disturbanceTorque[1]                                     << ',' 
                            << disturbanceTorque[2]                                     << std::endl;                      
    }

protected:

private:

    //! Output stream to write state history to.
    std::ostream& stateHistoryStream;

    //! Control torque 
    const Vector3 controlTorque; 

    //! Motor torque 
    const VectorXd motorTorque; 

    //! Disturbance Torque
    const Vector3 disturbanceTorque; 

    //! Reaction Wheel angular velocities. 
    const VectorXd reactionWheelAngularVelocities; 
};

}

#endif 