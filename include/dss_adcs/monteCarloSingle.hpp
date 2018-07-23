/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_MONTE_CARLO_SINGLE_HPP
#define DSS_ADCS_MONTE_CARLO_SINGLE_HPP

#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{
//! Execute a attitude dynamics simulator. 
/*!
 * Executes a attitude dynamics simulation for a fixed body. 
 *
 * This function is called when the user specifies the application mode to be
 * "attitude_dynamics_simulator".
 *
 * @param[in] config User-defined configuration options (extracted from JSON input file)
 */
void executeMonteCarloSingleSimulator( const rapidjson::Document& config );

//! Input for attitude_dynamics_simulator application mode.
/*!
 * Data struct containing all valid input parameters to execute the attitude_dynamics_simulator
 * application mode. This struct is populated by the checkSimulatorInput() function.
 *
 * @sa checkSimulatorInput, executeSimulator
 */
struct monteCarloSingleSimulatorInput
{
public:

    //! Construct data struct.
    /*!
     * Constructs data struct based on verified input parameters.
     *
     * @sa checkSingleSimulatorInput,                 executeSimulator
     * @param[in] aPrincipleinertia             Principle axes of inertia of the spacecraft [kg m^2]
     * @param[in] anInitialAttitudeState          Initial attitude state including attitudes and angular velocities [deg] [deg/sec] 
     * @param[in] anIntegrator                Name of selected numerical integrator
     * @param[in] aStartEpoch                 Start epoch for integration                        [s]
     * @param[in] anEndEpoch                  End epoch for integration                          [s]
     * @param[in] aTimeStep                   Time step for integration                          [s]
     * @param[in] aRelativeTolerance          Relative tolerance for integrator                  [-]
     * @param[in] anAbsoluteTolerance         Absolute tolerance for integrator                  [-]
     * @param[in] aMetadataFilePath           Path to output file for metadata
     * @param[in] aStateHistoryFilePath       Path to output file for state history
     */
    monteCarloSingleSimulatorInput( const Inertia                   aPrincipleInertiaMin,
                                    const Inertia                   aPrincipleInertiaMax,    
                                    const State                     anInitialAttitudeStateMin, 
                                    const State                     anInitialAttitudeStateMax,
                                    const Vector4                   aReferenceAttitudeState,
                                    const int                       aNumberOfSamples,
                                    const Integrator                anIntegrator,
                                    const Real                      aStartEpoch,
                                    const Real                      anEndEpoch, 
                                    const Real                      aTimeStep,
                                    const Real                      aGravitationalParameter, 
                                    const Real                      aRadius,
                                    const Real                      aSemiMajorAxis,   
                                    const Real                      aRelativeTolerance,   
                                    const Real                      anAbsoluteTolerance, 
                                    const bool                      aGravityGradientAccelerationModelFlag,
                                    const std::string&              aConceptConfiguration,
                                    const std::string&              aActuator, 
                                    const std::vector <std::string> aActuatorUuid,
                                    const std::vector< Vector2 >    aWheelOrientation,
                                    const bool                      aControlTorqueActiveModelFlag,
                                    const Real                      aNaturalFrequency, 
                                    const Real                      aDampingRatio, 
                                    const Real                      aSlewSaturationRate, 
                                    const std::string&              aControllerType,
                                    const std::string&              aMetadataFilePath,
                                    const std::string&              aStateHistoryFilePath )     
        : principleInertiaMin( aPrincipleInertiaMin ),
          principleInertiaMax( aPrincipleInertiaMax ),
          initialAttitudeStateMin( anInitialAttitudeStateMin ),
          initialAttitudeStateMax( anInitialAttitudeStateMax ),
          referenceAttitudeState( aReferenceAttitudeState ),
          numberOfSamples( aNumberOfSamples ), 
          integrator( anIntegrator ),
          startEpoch( aStartEpoch ),
          endEpoch( anEndEpoch ),
          timeStep( aTimeStep ),
          gravitationalParameter( aGravitationalParameter ),
          radius( aRadius ),
          semiMajorAxis( aSemiMajorAxis ),
          relativeTolerance( aRelativeTolerance ),
          absoluteTolerance( anAbsoluteTolerance ),
          gravityGradientAccelerationModelFlag( aGravityGradientAccelerationModelFlag ),
          conceptConfiguration( aConceptConfiguration ),
          actuator( aActuator ),
          actuatorUuid( aActuatorUuid ),
          wheelOrientation( aWheelOrientation ),
          controlTorqueActiveModelFlag( aControlTorqueActiveModelFlag ),
          naturalFrequency( aNaturalFrequency ),
          dampingRatio( aDampingRatio ), 
          slewSaturationRate( aSlewSaturationRate ),
          controllerType( aControllerType ),  
          metadataFilePath( aMetadataFilePath ),
          stateHistoryFilePath( aStateHistoryFilePath )
    { }

    //! Principle Inertia of the spacecraft [kg m^2].
    const Inertia principleInertiaMin;
    const Inertia principleInertiaMax;

    //! Initial attitude state and angular velocities of the spacecraft [deg], [deg/sec].
    const State initialAttitudeStateMin;
    const State initialAttitudeStateMax; 

    //! Reference attitude state defined by the user. 
    const Vector4 referenceAttitudeState; 
    
    //! Number of samples for monte carlo. 
    const int numberOfSamples; 
    
    //! Selected numerical integrator. 
    const Integrator integrator; 

    //! Start epoch for the simulation. 
    const Real startEpoch; 

    //! End epoch for the simulation.
    const Real endEpoch; 

    //! Time step for the simulation.
    const Real timeStep; 

    //! Gravitational Parameter of the central body. 
    const Real gravitationalParameter;

    //! Magnitude of the radius vector for the given orbital parameters. 
    const Real radius;

    //! Semi major axis of the orbit. 
    const Real semiMajorAxis; 

    //! Relative tolerance fo the integrator. 
    const Real relativeTolerance;

    //! Absolute tolerance for the integrator. 
    const Real absoluteTolerance;
    
    //! Status of gravity gradient model. 
    const bool gravityGradientAccelerationModelFlag;

    //! Configuration of the concept. 
    const std::string conceptConfiguration; 

    //! Type of actuator. 
    const std::string actuator; 

    //! Unique identifies of the actuator. 
    const std::vector <std::string> actuatorUuid; 

    //! Wheel offset values for the reaction wheels. 
    const std::vector< Vector2 > wheelOrientation; 

    //! Check if the control torque is active or not. 
    const bool controlTorqueActiveModelFlag; 

    //! Natural frequency of the system. 
    const Real naturalFrequency; 

    //! Damping ratio of the system. 
    const Real dampingRatio; 

    //! Slew saturation rate constraint. 
    const Real slewSaturationRate; 
    
    //! Type of controller to be used. 
    const std::string controllerType; 

    //! Metadata file path.
    const std::string metadataFilePath; 

    //! State History file path. 
    const std::string stateHistoryFilePath; 
    
protected:

private:
};

//! Check input parameters for attitude_dynamics_simulator application mode.
/*!
 * Checks that all inputs to execute a single dust particle simulation are valid. If not, an error
 * is thrown with a short description of the problem. If all inputs are valid, a data struct
 * containing all the inputs is returned.
 *
 * @sa executeSimulator, SingleSimulatorInput
 * @param[in] config User-defined configuration options (extracted from JSON input file)
 * @return           Struct containing all valid input for attitude_dynamics_simulator application
 *                   mode
 */
monteCarloSingleSimulatorInput checkMonteCarloSingleSimulatorInput( const rapidjson::Document& config );

} // namespace dss_adcs

#endif // DSS_ADCS_SIMULATOR_HPP
