/*
 * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <iomanip>
#include <iostream>

#include "dss_adcs/typedefs.hpp"

#ifndef DSS_ADCS_OUTPUT_WRITER_HPP
#define DSS_ADCS_OUTPUT_WRITER_HPP

namespace dss_adcs
{

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
template< typename DataType >
inline void print( std::ostream& stream,
                   const DataType value,
                   const int width = 25,
                   const char filler = ' ' )
{
    stream << std::left << std::setw( width ) << std::setfill( filler ) << value;
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
template< typename DataType >
inline void print( std::ostream& stream,
                   const std::string& parameterName,
                   const DataType value,
                   const std::string& units,
                   const char delimiter = ',',
                   const int width = 25,
                   const char filler = ' ' )
{
    print( stream, parameterName, width, filler );
    stream << delimiter;
    print( stream, value, width, filler );
    stream << delimiter;
    print( stream, units, width, filler );
}

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

    StateHistoryWriter( std::ostream& aStateHistoryStream )
        : stateHistoryStream( aStateHistoryStream )
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
    void operator( )( const State& state, const double time )
    {
        // const State stateAttitudeElements = state; 

        stateHistoryStream  << std::setprecision ( std::numeric_limits< double>::digits10 )
                            << time         << ','
                            << state[ 0 ]   << ','
                            << state[ 1 ]   << ','
                            << state[ 2 ]   << ','
                            << state[ 3 ]   << ','
                            << state[ 4 ]   << ','
                            << state[ 5 ]   << ','
                            << state[ 6 ]   << std::endl;                            
    }

protected:

private:

    //! Output stream to write state history to.
    std::ostream& stateHistoryStream;
};

}

#endif 