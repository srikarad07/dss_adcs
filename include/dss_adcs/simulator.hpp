/*
 *  Copyright (c) 2018 S.D. Cherukuri (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_SIMULATOR_HPP
#define DSS_ADCS_SIMULATOR_HPP

#include <string>

#include <rapidjson/document.h>

#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{

//! Execute simulator.
/*!
 * Executes a single dust particle simulation.
 *
 * This function is called when the user specifies the application mode to be
 * "simulator".
 *
 * @param[in] config User-defined configuration options (extracted from JSON input file)
 */
void executeSimulator( const rapidjson::Document& config );

//! Input for simulator application mode.
/*!
 * Data struct containing all valid input parameters to execute the simulator
 * application mode. This struct is populated by the checkSimulatorInput() function.
 *
 * @sa checkSimulatorInput, executeSimulator
 */
struct simulatorInput
{
public:

    //! Construct data struct.
    /*!
     * Constructs data struct based on verified input parameters.
     *
     * @sa checkSimulatorInput, executeSimulator
     * 
//      */
    simulatorInput( const Real            aPrincipleInertia, 
                    const State&          anInitialStateVector,
                    const Integrator      anIntegrator,
                    const Real            aStartEpoch,
                    const Real            anEndEpoch,
                    const Real            aTimeStep,
                    const Real            aRelativeTolerance,
                    const Real            anAbsoluteTolerance,
                    const std::string&    aMetadataFilePath,
                    const std::string&    aStateHistoryFilePath )         
        : principleInertia( aPrincipleInertia ),
          initialStateVector( anInitialStateVector ),
          integrator( anIntegrator ),
          startEpoch( aStartEpoch ),
          endEpoch( anEndEpoch ),
          timeStep( aTimeStep ),
          relativeTolerance( aRelativeTolerance ),
          absoluteTolerance( anAbsoluteTolerance ),
          metadataFilePath( aMetadataFilePath ),
          stateHistoryFilePath( aStateHistoryFilePath )  
    { }

    //! Gravitational parameter of central body [km^3 s^-2].
    const Real principleInertia;
    const State& initialStateVector;
    const Integrator integrator; 
    const Real startEpoch; 
    const Real endEpoch; 
    const Real timeStep; 
    const Real relativeTolerance; 
    const Real absoluteTolerance; 
    const std::string& metadataFilePath;
    const std::string& stateHistoryFilePath;

    //! Boolean flag indicating if J2 acceleration model is active (true) or not (false).
    //const bool isJ2AccelerationModelActive;

protected:

private:
};

//! Check input parameters for single_particle_simulator application mode.
/*!
 * Checks that all inputs to execute a single dust particle simulation are valid. If not, an error
 * is thrown with a short description of the problem. If all inputs are valid, a data struct
 * containing all the inputs is returned.
 *
 * @sa executeSimulator, SimulatorInput
 * @param[in] config User-defined configuration options (extracted from JSON input file)
 * @return           Struct containing all valid input for single_particle_simulator application
 *                   mode
 */
simulatorInput checkSimulatorInput( const rapidjson::Document& config );

} // namespace dss_adcs

#endif // DSS_ADCS_SIMULATOR_HPP
