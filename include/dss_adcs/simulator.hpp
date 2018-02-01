// /*
//  *  Copyright (c) 2018 S.D. Cherukuri (srikarad007@gmail.com)
//  * Distributed under the MIT License.
//  * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
//  */

// #ifndef DUSTSIM_SINGLE_PARTICLE_SIMULATOR_HPP
// #define DUSTSIM_SINGLE_PARTICLE_SIMULATOR_HPP

// #include <string>

// #include <rapidjson/document.h>

// #include "dustsim/typedefs.hpp"

// namespace dss_adcs
// {

// //! Execute simulator.
// /*!
//  * Executes a single dust particle simulation.
//  *
//  * This function is called when the user specifies the application mode to be
//  * "single_particle_simulator".
//  *
//  * @param[in] config User-defined configuration options (extracted from JSON input file)
//  */
// void executeSimulator( const rapidjson::Document& config );

// //! Input for single_particle_simulator application mode.
// /*!
//  * Data struct containing all valid input parameters to execute the single_particle_simulator
//  * application mode. This struct is populated by the checkSingleParticleSimulatorInput() function.
//  *
//  * @sa checkSingleParticleSimulatorInput, executeSingleParticleSimulator
//  */
// struct SingleParticleSimulatorInput
// {
// public:

//     //! Construct data struct.
//     /*!
//      * Constructs data struct based on verified input parameters.
//      *
//      * @sa checkSingleParticleSimulatorInput, executeSingleParticleSimulator
//      * @param[in] aGravitationalParameter     Gravitational parameter of central body    [km^3 s^-2]
//      * @param[in] aJ2AccelerationModelFlag    Flag indicating if J2 acceleration model is active
//      * @param[in] aJ2Coefficient              J2 coefficient of gravity expansion                [-]
//      * @param[in] anEquatorialRadius          Equatiorial radius for gravity expansion          [km]
//      * @param[in] aRadiationPressureFlag      Flag indicating if radiation pressure acceleration
//      *                                        model is active
//      * @param[in] aParticleRadius             Radius of dust particle                       [micron]
//      * @param[in] aParticleBulkDensity        Bulk density of dust particle                [kg m^-3]
//      * @param[in] aRadiationPressureCoefficient
//      *                                        Radiation pressure coefficient                     [-]
//      * @param[in] anInitialKeplerState        Initial state in Keplerian elements
//      * @param[in] anIntegrator                Name of selected numerical integrator
//      * @param[in] aStartEpoch                 Start epoch for integration                        [s]
//      * @param[in] anEndEpoch                  End epoch for integration                          [s]
//      * @param[in] aTimeStep                   Time step for integration                          [s]
//      * @param[in] aRelativeTolerance          Relative tolerance for integrator                  [-]
//      * @param[in] anAbsoluteTolerance         Absolute tolerance for integrator                  [-]
//      * @param[in] aMetadataFilePath           Path to output file for metadata
//      * @param[in] aStateHistoryFilePath       Path to output file for state history
//      */
//     SingleParticleSimulatorInput( const Real            aGravitationalParameter,
//                                   const bool            aJ2AccelerationModelFlag,
//                                   const Real            aJ2Coefficient,
//                                   const Real            anEquatorialRadius,
//                                   const bool            aRadiationPressureFlag,
//                                   const Real            aParticleRadius,
//                                   const Real            aParticleBulkDensity,
//                                   const Real            aRadiationPressureCoefficient,
//                                   const State&          anInitialKeplerState,
//                                   const Integrator      anIntegrator,
//                                   const Real            aStartEpoch,
//                                   const Real            anEndEpoch,
//                                   const Real            aTimeStep,
//                                   const Real            aRelativeTolerance,
//                                   const Real            anAbsoluteTolerance,
//                                   const std::string&    aMetadataFilePath,
//                                   const std::string&    aStateHistoryFilePath )
//         : gravitationalParameter( aGravitationalParameter ),
//           isJ2AccelerationModelActive( aJ2AccelerationModelFlag ),
//           j2Coefficient( aJ2Coefficient ),
//           equatorialRadius( anEquatorialRadius ),
//           isRadiationPressureAccelerationModelActive( aRadiationPressureFlag ),
//           particleRadius( aParticleRadius ),
//           particleBulkDensity( aParticleBulkDensity ),
//           radiationPressureCoefficient( aRadiationPressureCoefficient ),
//           initialStateKeplerianElements( anInitialKeplerState ),
//           integrator( anIntegrator ),
//           startEpoch( aStartEpoch ),
//           endEpoch( anEndEpoch ),
//           timeStep( aTimeStep ),
//           relativeTolerance( aRelativeTolerance ),
//           absoluteTolerance( anAbsoluteTolerance ),
//           metadataFilePath( aMetadataFilePath ),
//           stateHistoryFilePath( aStateHistoryFilePath )
//     { }

//     //! Gravitational parameter of central body [km^3 s^-2].
//     const Real gravitationalParameter;

//     //! Boolean flag indicating if J2 acceleration model is active (true) or not (false).
//     const bool isJ2AccelerationModelActive;

//     //! J2-coefficient (unnormalized) of spherical harmonics expansion of gravity field [-].
//     const Real j2Coefficient;

//     //! Equatorial radius of central body corresponding with spherical harmonics gravity field [km].
//     const Real equatorialRadius;

//     //! Boolean flag indicating if radiation pressure acceleration model is active (true) or not
//     //! (false).
//     const bool isRadiationPressureAccelerationModelActive;

//     //! Radius of dust particle [micron].
//     const Real particleRadius;

//     //! Bulk density of dust particle [kg m^-3].
//     const Real particleBulkDensity;

//     //! Radiation pressure coefficient [-].
//     const Real radiationPressureCoefficient;


//     //! Initial state in Keplerian elements [km, -, rad, rad, rad, rad].
//     const State initialStateKeplerianElements;

//     //! Selected numerical integrator.
//     const Integrator integrator;

//     //! Start epoch for simulator [s].
//     const Real startEpoch;

//     //! End epoch for simulator [s].
//     const Real endEpoch;

//     //! Time step for simulator [s].
//     const Real timeStep;

//     //! Relative tolerance for numerical integrator [-].
//     const Real relativeTolerance;

//     //! Absolute tolerance for numerical integrator [-].
//     const Real absoluteTolerance;

//     //! Metadata file path.
//     const std::string metadataFilePath;

//     //! State history file path.
//     const std::string stateHistoryFilePath;

// protected:

// private:
// };

// //! Check input parameters for single_particle_simulator application mode.
// /*!
//  * Checks that all inputs to execute a single dust particle simulation are valid. If not, an error
//  * is thrown with a short description of the problem. If all inputs are valid, a data struct
//  * containing all the inputs is returned.
//  *
//  * @sa executeSingleParticleSimulator, SingleParticleSimulatorInput
//  * @param[in] config User-defined configuration options (extracted from JSON input file)
//  * @return           Struct containing all valid input for single_particle_simulator application
//  *                   mode
//  */
// SingleParticleSimulatorInput checkSingleParticleSimulatorInput( const rapidjson::Document& config );

// } // namespace dss_adcs

// #endif // DUSTSIM_SINGLE_PARTICLE_SIMULATOR_HPP
