/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/numeric/odeint.hpp>
#include <boost/random/mersenne_twister.hpp>
// #include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <Eigen/Core>

#include <astro/astro.hpp>
#include <sml/sml.hpp>

#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/bulkSimulator.hpp"
#include "dss_adcs/dynamicalSystem.hpp"
#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/reactionWheelConfiguration.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/saveConceptAttributes.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/outputWriter.hpp"

namespace dss_adcs
{

void executeBulkSimulator( const rapidjson::Document& config )
{
    // Verify config parameters. Exception is thrown if any of the parameters are missing.
    const simulatorInput input = checkBulkSimulatorInput( config );

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                           Run simulator                          " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "The API is being called to extract the parameters ... " << std::endl;
    const std::vector< ReactionWheel > reactionWheels = getReactionWheels( input.actuator, input.actuatorUuid ); 

    // Define the actuator configuration. 
    std::cout << "Defining actuator configuration ... \n" << std::endl; 
    
    const unsigned int minimumNumberOfReactionWheels    = input.numberOfReactionWheels[0]; 
    const unsigned int maximumNumberOfReactionWheels    = input.numberOfReactionWheels[1] + 1;  

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>...... // 
    // Change this and if there is a better way to do it.                             // 
    // const bool asymmetricBodyTorqueModelFlag           = find( config, "is_asymmetric_body_torque_active")->value.GetBool(); 
    // const bool monteCarloInitialAttitudeActiveFlag     = find( config, "monte_carlo_on_initial_attitude" )->value.GetBool( );
    const Real slewRateUncertainty        = sml::convertDegreesToRadians(
                                                            find( config, "slew_rate_range" )->value.GetDouble( ) );  
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  // 

    // Initialize random seed.
    boost::random::mt19937 randomSeed;

    // Random number generator for principle inertia vector. 
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorXX( 
        input.principleInertia[0] - input.principleInertiaUncertainty[0], 
        input.principleInertia[0] + input.principleInertiaUncertainty[0] );
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorYY( 
        input.principleInertia[1] - input.principleInertiaUncertainty[1], 
        input.principleInertia[1] + input.principleInertiaUncertainty[1]  );
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorZZ( 
        input.principleInertia[2] - input.principleInertiaUncertainty[2], 
        input.principleInertia[2] + input.principleInertiaUncertainty[2]  );

    // Generate random seed for slew saturation rate. 
    boost::random::uniform_real_distribution< Real > slewRateGenerator( 
                                                    input.slewSaturationRate - slewRateUncertainty,                input.slewSaturationRate + slewRateUncertainty ); 

    // Create instance of dynamical system.
    std::cout << "Setting up dynamical model ..." << std::endl;
    
    // Progress bar. 
    std::cout << "Current Progress ..." << std::endl; 
    
    // std::ofstream metadatafile( input.metadataFilePath + ".csv" );
    std::map< std::pair<std::string, unsigned int>, std::vector <ReactionWheel> > 
                    reactionWheelConcepts = getReactionWheelConcepts( 
                                                input.reactionWheelConfiguration, 
                                                reactionWheels, 
                                                minimumNumberOfReactionWheels,
                                                maximumNumberOfReactionWheels, 
                                                input.wheelOrientation ); 

    //! Total number of simulations. 
    unsigned const totalNumberOfSimulations = (input.numberOfSamples - 1) + reactionWheelConcepts.size(); 
    
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
    // Find a better way to do print the metadata file. 
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 

    // std::vector< std::string > metaDataFilePath; 
    for ( unsigned int numberOfReactionWheels = minimumNumberOfReactionWheels; 
            numberOfReactionWheels != maximumNumberOfReactionWheels; 
            ++ numberOfReactionWheels )
    {
        std::ofstream metadatafile( input.metadataFilePath + 
                                    std::to_string(numberOfReactionWheels) + ".csv" ); 
        std::ofstream conceptAttribtuesFile( input.stateHistoryFilePath + 
                                    std::to_string(numberOfReactionWheels) + ".csv" );
        if ( numberOfReactionWheels == 2 )
        {
            metadatafile << "ConceptIdentifier,principleInertia1,principleInertia2,principleInertia3,InitialAttitudeState1,InitialAttitudeState2,InitialAttitudeState3,InitialAttitudeState4,InitialAttitudeState5,InitialAttitudeState6,InitialAttitudeState7,AttitudeReferenceState1,AttitudeReferenceState2,AttitudeReferenceState3,AsymmetricBodyTorqueFlag,GravityGradientBodyFlag,ControlTorqueFlag,GravitationalParameter,NaturalFrequency,DampingRatio,SlewRate,SemiMajorAxis,ControllerType,Radius,Integrator,StartEpoch,EndEpoch,TimeStep,RelativeTolerance,AbsoluteTolerance,rwUuid1,rwUUid2,rwWheelOrientation1,rwWheelOrientation1,rw2WheelOrientation2,rw2WheelOrientation2,reactionWheelName1,reactionWheelName2" << std::endl;

            conceptAttribtuesFile << "ConceptIdentifier,mass,volume,systemPeakPower,systemAvgPower,settlingTime,rwPeakPower1,rwPeakPower2,rwAvgPower1,rwAvgPower2,rwMomentum1,rwMomentum2,rwPeakPowerPercent1,rwPeakPowerPercent2,rwMomentumPercent1,rwMomentumPercent2" << std::endl; 
        }
        else if ( numberOfReactionWheels == 3 )
        {
            metadatafile << "ConceptIdentifier,principleInertia1,principleInertia2,principleInertia3,InitialAttitudeState1,InitialAttitudeState2,InitialAttitudeState3,InitialAttitudeState4,InitialAttitudeState5,InitialAttitudeState6,InitialAttitudeState7,AttitudeReferenceState1,AttitudeReferenceState2,AttitudeReferenceState3,AttitudeReferenceState4,AsymmetricBodyTorqueFlag,GravityGradientBodyFlag,ControlTorqueFlag,GravitationalParameter,NaturalFrequency,DampingRatio,SlewRate,SemiMajorAxis,ControllerType,Radius,Integrator,StartEpoch,EndEpoch,TimeStep,RelativeTolerance,AbsoluteTolerance,rw1Uuid,rw2UUid,rw3UUid,rw1WheelOrientation1,rw2WheelOrientation1,rw3WheelOrientation1,rw1WheelOrientation2,rw2WheelOrientation2,rw3WheelOrientation2,reactionWheelName1,reactionWheelName2,reactionWheelName3" << std::endl;

            conceptAttribtuesFile << "ConceptIdentifier,mass,volume,systemPeakPower,systemAvgPower,settlingTime,rwPeakPower1,rwPeakPower2,rwPeakPower3,rwAvgPower1,rwAvgPower2,rwAvgPower3,rwMomentum1,rwMomentum2,rwMomentum3,rwPeakPowerPercent1,rwPeakPowerPercent2,rwPeakPowerPercent3,rwMomentumPercent1,rwMomentumPercent2,rwMomentumPercent3" << std::endl; 
        }
        else if ( numberOfReactionWheels == 4 )
        {
            metadatafile << "ConceptIdentifier,principleInertia1,principleInertia2,principleInertia3,InitialAttitudeState1,InitialAttitudeState2,InitialAttitudeState3,InitialAttitudeState4,InitialAttitudeState5,InitialAttitudeState6,InitialAttitudeState7,AttitudeReferenceState1,AttitudeReferenceState2,AttitudeReferenceState3,AttitudeReferenceState4,AsymmetricBodyTorqueFlag,GravityGradientBodyFlag,ControlTorqueFlag,GravitationalParameter,NaturalFrequency,DampingRatio,SlewRate,SemiMajorAxis,ControllerType,Radius,Integrator,StartEpoch,EndEpoch,TimeStep,RelativeTolerance,AbsoluteTolerance,rw1Uuid,rw2UUid,rw3UUid,rw4UUid,rw1WheelOrientation1,rw2WheelOrientation1,rw3WheelOrientation1,rw4WheelOrientation1,rw1WheelOrientation2,rw2WheelOrientation2,rw3WheelOrientation2,rw4WheelOrientation2,reactionWheelName1,reactionWheelName2,reactionWheelName3,reactionWheelName4" << std::endl;

            conceptAttribtuesFile << "ConceptIdentifier,mass,volume,systemPeakPower,systemAvgPower,settlingTime,rwPeakPower1,rwPeakPower2,rwPeakPower3,rwPeakPower4,rwAvgPower1,rwAvgPower2,rwAvgPower3,rwAvgPower4,rwMomentum1,rwMomentum2,rwMomentum3,rwMomentum4,rwPeakPowerPercent1,rwPeakPowerPercent2,rwPeakPowerPercent3,rwPeakPowerPercent4,rwMomentumPercent1,rwMomentumPercent2,rwMomentumPercent3,rwMomentumPercent4" << std::endl; 
        }
        else if ( numberOfReactionWheels == 5 )
        {
            metadatafile << "ConceptIdentifier,principleInertia1,principleInertia2,principleInertia3,InitialAttitudeState1,InitialAttitudeState2,InitialAttitudeState3,InitialAttitudeState4,InitialAttitudeState5,InitialAttitudeState6,InitialAttitudeState7,AttitudeReferenceState1,AttitudeReferenceState2,AttitudeReferenceState3,AttitudeReferenceState4,AsymmetricBodyTorqueFlag,GravityGradientBodyFlag,ControlTorqueFlag,GravitationalParameter,NaturalFrequency,DampingRatio,SlewRate,SemiMajorAxis,ControllerType,Radius,Integrator,StartEpoch,EndEpoch,TimeStep,RelativeTolerance,AbsoluteTolerance,rw1Uuid,rw2UUid,rw3UUid,rw4UUid,rw5UUid,rw1WheelOrientation1,rw2WheelOrientation1,rw3WheelOrientation1,rw4WheelOrientation1,rw5WheelOrientation1,rw1WheelOrientation2,rw2WheelOrientation2,rw3WheelOrientation2,rw4WheelOrientation2,rw5WheelOrientation2,reactionWheelName1,reactionWheelName2,reactionWheelName3,reactionWheelName4,reactionWheelName5" << std::endl;
        
            conceptAttribtuesFile << "ConceptIdentifier,mass,volume,systemPeakPower,systemAvgPower,settlingTime,rwPeakPower1,rwPeakPower2,rwPeakPower3,rwPeakPower4,rwPeakPower5,rwAvgPower1,rwAvgPower2,rwAvgPower3,rwAvgPower4,rwAvgPower5,rwMomentum1,rwMomentum2,rwMomentum3,rwMomentum4,rwMomentum5,rwPeakPowerPercent1,rwPeakPowerPercent2,rwPeakPowerPercent3,rwPeakPowerPercent4,rwPeakPowerPercent5,rwMomentumPercent1,rwMomentumPercent2,rwMomentumPercent3,rwMomentumPercent4,rwMomentumPercent5" << std::endl; 
        }
        else if ( numberOfReactionWheels == 6 )
        {
            metadatafile << "ConceptIdentifier,principleInertia1,principleInertia2,principleInertia3,InitialAttitudeState1,InitialAttitudeState2,InitialAttitudeState3,InitialAttitudeState4,InitialAttitudeState5,InitialAttitudeState6,InitialAttitudeState7,AttitudeReferenceState1,AttitudeReferenceState2,AttitudeReferenceState3,AttitudeReferenceState4,AsymmetricBodyTorqueFlag,GravityGradientBodyFlag,ControlTorqueFlag,GravitationalParameter,NaturalFrequency,DampingRatio,SlewRate,SemiMajorAxis,ControllerType,Radius,Integrator,StartEpoch,EndEpoch,TimeStep,RelativeTolerance,AbsoluteTolerance,rw1Uuid,rw2UUid,rw3UUid,rw4UUid,rw5UUid,rw6UUid,rw1WheelOrientation1,rw2WheelOrientation1,rw3WheelOrientation1,rw4WheelOrientation1,rw5WheelOrientation1,rw6WheelOrientation1,rw1WheelOrientation2,rw2WheelOrientation2,rw3WheelOrientation2,rw4WheelOrientation2,rw5WheelOrientation2,rw6WheelOrientation2,reactionWheelName1,reactionWheelName2,reactionWheelName3,reactionWheelName4,reactionWheelName5,reactionWheelName6" << std::endl;

            conceptAttribtuesFile << "ConceptIdentifier,mass,volume,systemPeakPower,systemAvgPower,settlingTime,rwPeakPower1,rwPeakPower2,rwPeakPower3,rwPeakPower4,rwPeakPower5,rwPeakPower6,rwAvgPower1,rwAvgPower2,rwAvgPower3,rwAvgPower4,rwAvgPower5,rwAvgPower6,rwMomentum1,rwMomentum2,rwMomentum3,rwMomentum4,rwMomentum5,rwMomentum6,rwPeakPowerPercent1,rwPeakPowerPercent2,rwPeakPowerPercent3,rwPeakPowerPercent4,rwPeakPowerPercent5,rwPeakPowerPercent6,rwMomentumPercent1,rwMomentumPercent2,rwMomentumPercent3,rwMomentumPercent4,rwMomentumPercent5,rwMomentumPercent6" << std::endl; 
        }
        else
        {
            std::cout << "MetadataFile pattern not defined for " << numberOfReactionWheels << "number of reaction wheels!! " << std::endl; 
        }
        // metadatafile.close(); 
    }    
    
    // Initialise the current number of simulation. 
    unsigned int currentSimulationNumber = 0; 
    
    std::cout << "Looping over the reaction wheel concepts " << std::endl; 
    for ( std::map< std::pair<std::string, unsigned int>, std::vector<ReactionWheel> >::iterator 
                                        reactionWheelConceptIterator = reactionWheelConcepts.begin(); 
                                        reactionWheelConceptIterator != reactionWheelConcepts.end(); 
                                        ++reactionWheelConceptIterator )
    {
        const std::pair<std::string, unsigned int> conceptIdentifier = reactionWheelConceptIterator->first; 
        const std::vector< ReactionWheel > reactionWheelConcept = reactionWheelConceptIterator->second; 
        
        const unsigned int numberOfReactionWheels = reactionWheelConcept.size(); 

        // Print metadata to the file provided in metadatafile path (std::ios::app <- print 
        // at the end of the file) 
        std::ofstream metadatafile( input.metadataFilePath + std::to_string(numberOfReactionWheels) 
                                    + ".csv", std::ios::app );

        std::ofstream conceptAttribtuesFile( input.stateHistoryFilePath + 
                                    std::to_string(numberOfReactionWheels) + ".csv", std::ios::app );

        // std::cout << "File storepath : " << input.metadataFilePath + std::to_string(numberOfReactionWheels) 
        // + ".csv" << std::endl; 
        
        const ActuatorConfiguration actuatorConfiguration( reactionWheelConcept ); 
        // Create file stream to write state history to.
        // std::ofstream stateHistoryFile( input.stateHistoryFilePath + ".csv");
        // std::ofstream stateHistoryFile( input.stateHistoryFilePath + conceptIdentifier.first + "_" + std::to_string(numberOfReactionWheels) + "_" + std::to_string(numberOfSimulation) + ".csv");
        // if ( numberOfReactionWheels == 3 )
        // {
        //     stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,angularMomentum1,angularMomentum2,angularMomentum3,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2reactionWheelAngularVelocity3,powerConsumption1,powerConsumption2,powerConsumption3,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl; 
        // }
        // else if ( numberOfReactionWheels == 2 )
        // {
        //     stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,angularMomentum1,angularMomentum2,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,powerConsumption1,powerConsumption2disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        // }
        // else if ( numberOfReactionWheels == 4 )
        // {
        //     stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,reactionWheelAngularVelocity1reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,reactionWheelAngularVelocity4,powerConsumption1,powerConsumption2,powerConsumption3powerConsumption4,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        // }
        // else if ( numberOfReactionWheels == 5 )
        // {
        //     stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,angularMomentum5,reactionWheelAngularvelocity1reactionWheelAngularvelocity2,reactionWheelAngularvelocity3,reactionWheelAngularVelocity4,reactionWheelAngularVelocity5,powerConsumption1,powerConsumption2powerConsumption3,powerConsumption4,powerConsumption5,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        // }
        // else if ( numberOfReactionWheels == 6 )
        // {
        //     stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,motorTorque5,motorTorque6,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,angularMomentum5angularMomentum6,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,reactionWheelAngularVelocity4reactionWheelAngularVelocity5,reactionWheelAngularVelocity6,powerConsumption1,powerConsumption2,powerConsumption3,powerConsumption4,powerConsumption5powerConsumption6,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        // }
        // else
        // {
        //     std::cout << "The state history file not set up for " << numberOfReactionWheels << " reaction wheels!" << std::endl; 
        //     throw; 
        // }
        
        // Random number generator for Monte-Carlo simulations. 
        for ( unsigned int numberOfSimulation = 0; numberOfSimulation < input.numberOfSamples; 
                ++numberOfSimulation )
        {
            Inertia principleInertia; 
            //! Principle inertia random generator for X axis
            if ( input.principleInertiaUncertainty[0] != 0 )
            {
                principleInertia[0] = principleInertiaGeneratorXX(randomSeed);  
            }
            else 
            {
                principleInertia[0]  = input.principleInertia[0]; 
            }
            //! Principle inertia random generator for Y axis
            if ( input.principleInertiaUncertainty[1] != 0 )
            {
                principleInertia[1] = principleInertiaGeneratorYY(randomSeed);  
            }
            else 
            {
                principleInertia[1]  = input.principleInertia[1]; 
            }
            //! Principle inertia random generator for Z axis
            if ( input.principleInertiaUncertainty[2] != 0 )
            {
                principleInertia[2] = principleInertiaGeneratorZZ(randomSeed);  
            }
            else 
            {
                principleInertia[2]  = input.principleInertia[2]; 
            }

            // Slew saturation rate. 
            Real slewRateRandomlyGenerated; 
            if ( slewRateUncertainty != 0 )
            {
                slewRateRandomlyGenerated = slewRateGenerator( randomSeed );     
            } 
            else
            {
                slewRateRandomlyGenerated = input.slewSaturationRate; 
            }

            // Concept Identifier with simulation
            const std::string conceptIdentifierMonteCarlo = conceptIdentifier.first + "_" 
                                                            + std::to_string(numberOfSimulation); 

            // Generate current state for the integration 
            VectorXd currentState( ( input.initialAttitudeState.size() + reactionWheelConcept.size() ) );        
            for ( unsigned int stateIterator = 0; stateIterator < (input.initialAttitudeState.size() +  reactionWheelConcept.size() ); ++stateIterator )
            {
                if ( stateIterator < input.initialAttitudeState.size() )
                {
                    currentState[stateIterator] = input.initialAttitudeState[stateIterator]; 
                }
                else if ( stateIterator >= input.initialAttitudeState.size() && stateIterator <     (input.initialAttitudeState.size() +        reactionWheelConcept.size() ) )
                {
                    currentState[stateIterator] = 0.0; 
                }
                else
                {
                    std::cout << "Something is going wrong in the initial current state iterator! " << std::endl; 
                }
            }; 
            Vector4 referenceAttitudeState      = input.referenceAttitudeState; 

            // const Vector4 initialQuaternion(input.initialAttitudeState[0], input.initialAttitudeState[1], input.initialAttitudeState[2], input.initialAttitudeState[3]); 

            const Vector4 initialQuaternionError( input.initialAttitudeState[0] - input.referenceAttitudeState[0],
                                     input.initialAttitudeState[1] - input.referenceAttitudeState[1], input.initialAttitudeState[2] - input.referenceAttitudeState[2], input.initialAttitudeState[3] - input.referenceAttitudeState[3] );
            //! Save state histories within the model. 
            StateHistoryStorageContainer stateHistoryStorageContainer; 

            for ( Real integrationStartTime = input.startEpoch; integrationStartTime < input.endEpoch;  integrationStartTime++ )
            {
                const Real integrationEndTime = integrationStartTime + input.timeStep; 
                const Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
                const Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 
                VectorXd reactionWheelAngularMomentums( reactionWheelConcept.size() ); 
                for ( unsigned int angularMomentumIterator = 0; angularMomentumIterator < reactionWheelAngularMomentums.size(); ++angularMomentumIterator)
                { 
                    reactionWheelAngularMomentums[angularMomentumIterator] = currentState[input.initialAttitudeState.size() + angularMomentumIterator ]; 
                }
                VectorXd reactionWheelAngularVelocities = actuatorConfiguration.computeReactionWheelVelocities(reactionWheelAngularMomentums);
            
                Vector3 asymmetricBodyTorque( 0.0, 0.0, 0.0 ); 
                if ( input.asymmetricBodyTorqueModelFlag != false )
                {
                    asymmetricBodyTorque    = astro::computeRotationalBodyAcceleration( principleInertia, currentAttitudeRate );
                }
            
                Vector3 gravityGradientTorque( 0.0, 0.0, 0.0 ); 
                // Disturbance torques. 
                if ( input.gravityGradientAccelerationModelFlag != false )
                {
                    gravityGradientTorque += astro::computeGravityGradientTorque( input.gravitationalParameter, input.radius, principleInertia, currentAttitude ); 
                }
                Vector3 disturbanceTorque( 0.0, 0.0, 0.0 );
                disturbanceTorque[0] += gravityGradientTorque[0];
                disturbanceTorque[1] += gravityGradientTorque[1];
                disturbanceTorque[2] += gravityGradientTorque[2]; 
                // Vector3 controlTorque( 0.0, 0.0, 0.0 ); 
                std::pair < Vector3, VectorXd > outputTorques = 
                                    dss_adcs::computeRealTorqueValue( 
                                                        currentAttitude, 
                                                        referenceAttitudeState,
                                                        currentAttitudeRate, 
                                                        actuatorConfiguration, 
                                                        input.controllerType, 
                                                        input.naturalFrequency, 
                                                        input.dampingRatio, 
                                                        slewRateRandomlyGenerated,
                                                        principleInertia, 
                                                        initialQuaternionError, 
                                                        integrationStartTime ); 
                Vector3 controlTorque( outputTorques.first ); 
                VectorXd reactionWheelMotorTorque( outputTorques.second );  
                if ( input.controlTorqueActiveModelFlag == false )
                {
                    controlTorque = { 0.0, 0.0, 0.0 };
                }
                // Compute reaction wheel power. 
                VectorXd reactionWheelPowerConsumption   = actuatorConfiguration.computeReactionWheelPower( 
                                                                                   reactionWheelMotorTorque, 
                                                                                   reactionWheelAngularMomentums ); 
            
                //! Save state history writer file. 
                StateHistoryWriterToInternalStructure writer( stateHistoryStorageContainer,
                                                        controlTorque, reactionWheelMotorTorque,
                                                        disturbanceTorque, reactionWheelAngularVelocities,
                                                        reactionWheelPowerConsumption );
            
                // Dynamics of the system 
                DynamicalSystem dynamics( asymmetricBodyTorque, controlTorque, disturbanceTorque, reactionWheelMotorTorque, principleInertia );
            
                // Convert the eigen type vector to std::vector for compatibility with boost integrators. 
                VectorXdIntegration currentStateForIntegration( currentState.data(), currentState.data() + currentState.rows() * currentState.cols() );
    
                if ( input.integrator == rk4 )
                {
                    using namespace boost::numeric::odeint;
                    integrate_const( runge_kutta4< VectorXdIntegration >( ),
                                     dynamics,
                                     currentStateForIntegration,
                                     integrationStartTime,
                                     integrationEndTime,
                                     input.timeStep,
                                     writer );   
                }
                else if ( input.integrator == dopri5 )
                {
                    using namespace boost::numeric::odeint; 
                    integrate(  dynamics, 
                                currentStateForIntegration, 
                                integrationStartTime, 
                                integrationEndTime, 
                                input.timeStep, 
                                writer ); 
                }
                else 
                {
                    std::cout << "Numerical Integrator " << input.integrator << " not defined yet! " << std::endl; 
                    throw; 
                }
                currentState    = VectorXd::Map( currentStateForIntegration.data(), 
                                                 currentStateForIntegration.size() );   
            } // Integration loop! 

            //! Save high level parameters to assess the control concept.         
            SaveHighLevelAttributes dataToSave( stateHistoryStorageContainer ); 

            //! Get Peak power values for each simulation.
            std::tuple< SystemPowerPair, ReactionWheelPowerPair, const VectorXd > outputPower = 
                                            dataToSave.getPeakPower( reactionWheelConcept );  
            const SystemPowerPair systemPowerPair           = std::get<0>(outputPower); 
            const Real systemPeakPowerPerSimulation         = systemPowerPair.first; 
            const Real systemAvgPowerPerSimulation          = systemPowerPair.second; 
            const ReactionWheelPowerPair rwPeakAndAveragePower = std::get<1>(outputPower); 
            const VectorXd rwPeakPowerPerSimulation         = rwPeakAndAveragePower.first;
            const VectorXd rwAvgPowerPerSimulation          = rwPeakAndAveragePower.second;  
            const VectorXd rwPeakPowerPercentPerSimulation  = std::get<2>(outputPower); 

            //! Get Peak momentum values for each simulation. 
            std::tuple< const VectorXd, const VectorXd, const VectorXd > outputMomentum = 
                                            dataToSave.getAngularMomentums( reactionWheelConcept ); 
            const VectorXd rwPeakMomentumStorage            = std::get<0>(outputMomentum); 
            const VectorXd rwAvgMomentumStorage             = std::get<1>(outputMomentum); 
            const VectorXd rwPeakMomentumPercentStorage     = std::get<2>(outputMomentum); 

            //! Maneuver time of the attitude control concept. 
            const Real settlingTime         = dataToSave.calculateSettingTime(); 

            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            // Undo the changes in the IO.h file in struct IOFormat, change the parameters _coeffSeparator & 
            // _rowSeparator back to " " & "/n" respectively. Instead use the link below to add a custom formatter 
            // for the eigen matrix class. 
            // https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 

            //! Reaction wheel uuids to pass into the metadata function and save 
            //! reaction wheel orientation in Eigen vector for metadata printing.
            VectorXd reactionWheelOrientation1( reactionWheelConcept.size() ), 
                     reactionWheelOrientation2( reactionWheelConcept.size() ); 
            StringXd reactionWheelUuids( reactionWheelConcept.size() );
            StringXd reactionWheelNames( reactionWheelConcept.size() ); 

            unsigned int iterationLoop = 0; 
            for ( std::vector<ReactionWheel>::const_iterator conceptIterator = reactionWheelConcept.begin(); 
                        conceptIterator != reactionWheelConcept.end(); ++conceptIterator )
            {
               const ReactionWheel tempReactionWheel        = *conceptIterator; 
               reactionWheelUuids[iterationLoop]            = tempReactionWheel.actuatorUuid;
               reactionWheelOrientation1[iterationLoop]     = tempReactionWheel.wheelOrientation[0]; 
               reactionWheelOrientation2[iterationLoop]     = tempReactionWheel.wheelOrientation[1]; 
               reactionWheelNames[iterationLoop]            = tempReactionWheel.name;
               iterationLoop += 1;  
            }

            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            // - The initial attitude state is saved in quaternions, change it to the user defined 
            //     attitude representation. 
            // - Same with the reference attitude state
            // - Reaction wheel orientations saved in rad, save it in deg. 
            // - Make the simulation parameteric to initial attitude state and slew saturation rate.
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
    
            // Save the metadata to metadatafile
            doPrint( metadatafile, conceptIdentifierMonteCarlo, principleInertia, input.initialAttitudeState, 
             input.referenceAttitudeState, input.asymmetricBodyTorqueModelFlag, 
             input.gravityGradientAccelerationModelFlag, input.controlTorqueActiveModelFlag, 
             input.gravitationalParameter, input.naturalFrequency, input.dampingRatio, 
             sml::convertRadiansToDegrees(slewRateRandomlyGenerated), input.semiMajorAxis, 
             input.controllerType, input.radius, 
             input.integrator, input.startEpoch, input.endEpoch, input.timeStep, 
             input.relativeTolerance, input.absoluteTolerance, reactionWheelUuids, 
             reactionWheelOrientation1, reactionWheelOrientation2, reactionWheelNames  );  
            metadatafile << std::endl;

            // Save the conept attributes for each simulation. 
            doPrint( conceptAttribtuesFile, conceptIdentifierMonteCarlo, 
                     actuatorConfiguration.calculateMassBudget( ), 
                     actuatorConfiguration.calculateVolumeBudget( ), systemPeakPowerPerSimulation, 
                     systemAvgPowerPerSimulation, settlingTime,
                     rwPeakPowerPerSimulation, rwAvgPowerPerSimulation, rwPeakMomentumStorage, rwPeakPowerPercentPerSimulation,
                     rwPeakMomentumPercentStorage );
            conceptAttribtuesFile << std::endl; 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            // Update the progress bar to include both monte carlo simulations and the number of
            // reaction wheel concepts for printing progress bar. 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 

            // Print progress on the screen.
            progressBar( totalNumberOfSimulations, currentSimulationNumber );

            // Update the current simulation number. 
            ++currentSimulationNumber; 
        } // Monte Carlo simulation loop 
    } // Reaction wheel concept loop
};

//! Check input parameters for the attitude_dynamics_simulator mode. 
simulatorInput checkBulkSimulatorInput( const rapidjson::Document& config )
{
    // Extract principle inertia. 
    ConfigIterator principleInertiaDiagonalIterator     = find( config, "principle_inertia"); 
    Inertia principleInertia; 
    principleInertia[0]                                 = principleInertiaDiagonalIterator->value[0].GetDouble( ); 
    principleInertia[1]                                 = principleInertiaDiagonalIterator->value[1].GetDouble( ); 
    principleInertia[2]                                 = principleInertiaDiagonalIterator->value[2].GetDouble( );
    std::cout << "Principle inertia around X axis:                              " << principleInertia[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Principle inertia around Y axis:                              " << principleInertia[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Principle inertia around Z axis:                              " << principleInertia[2]
              << "[kg/m^2]" << std::endl; 

    // Extract principle inertia uncertainty 
    ConfigIterator principleInertiaUncertianityDiagonalIterator     = find( config, "principle_inertia_uncertainties"); 
    Inertia principleInertiaUncertainty; 
    principleInertiaUncertainty[0]                                 = principleInertiaUncertianityDiagonalIterator->value[0].GetDouble( ); 
    principleInertiaUncertainty[1]                                 = principleInertiaUncertianityDiagonalIterator->value[1].GetDouble( ); 
    principleInertiaUncertainty[2]                                 = principleInertiaUncertianityDiagonalIterator->value[2].GetDouble( );
    std::cout << "Uncertainty in Principle inertia around X axis:                          " << principleInertiaUncertainty[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Uncertainty in Principle inertia around Y axis:                          " << principleInertiaUncertainty[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Uncertainty in Principle inertia around Z axis:                          " << principleInertiaUncertainty[2]
              << "[kg/m^2]" << std::endl;

    // Extract attitude kinematic type. 
    const std::string attitudeRepresentationString      = find( config, "attitude_representation" )->value.GetString( );
    std::cout << "Attitude representation:                                      " << attitudeRepresentationString << std::endl; 
    
    // Extract the initial attitude states and angular velocities. 
    ConfigIterator initialAttitudeStateEulerIterator    = find( config, "initial_attitude_state");
    ConfigIterator referenceAttitudeStateIterator       = find( config, "attitude_reference_state" );
    const rapidjson::Value& intialAttitudeStateSize 	= config["initial_attitude_state"]; 

    Vector7 initialAttitudeState;
    Vector4 referenceAttitudeState; 
    Vector3 initialAttitudeStateEuler;

    if (attitudeRepresentationString.compare("euler_angles") == 0)
    {
        initialAttitudeStateEuler[0] = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[0].GetDouble() );
        initialAttitudeStateEuler[1] = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[1].GetDouble() );
        initialAttitudeStateEuler[2] = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[2].GetDouble() );
	
	    // Check the size of the input attitude state! 	
	    if (  intialAttitudeStateSize.Size()  != 6 )
	    {
		    std::cout << "FATAL error! The size of the initial attitude state is more than 6 elements for the Euler kinematic representation!" << std::endl; 
		    throw;	
	    }

        Vector4 quaternionInitialState = astro::transformEulerToQuaternion( initialAttitudeStateEuler );
        initialAttitudeState[0]        = quaternionInitialState[0];
        initialAttitudeState[1]        = quaternionInitialState[1];
        initialAttitudeState[2]        = quaternionInitialState[2];
        initialAttitudeState[3]        = quaternionInitialState[3];   
        initialAttitudeState[4]        = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[3].GetDouble() ); 
        initialAttitudeState[5]        = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[4].GetDouble() ); 
        initialAttitudeState[6]        = sml::convertDegreesToRadians(
                        initialAttitudeStateEulerIterator->value[5].GetDouble() ); 

        Vector3 referenceAttitudeStateEuler; 

        referenceAttitudeStateEuler[0] = sml::convertDegreesToRadians(
                        referenceAttitudeStateIterator->value[0].GetDouble() );
        referenceAttitudeStateEuler[1] = sml::convertDegreesToRadians(
                        referenceAttitudeStateIterator->value[1].GetDouble() );
        referenceAttitudeStateEuler[2] = sml::convertDegreesToRadians(
                        referenceAttitudeStateIterator->value[2].GetDouble() ); 

        referenceAttitudeState         = astro::transformEulerToQuaternion( referenceAttitudeStateEuler );    
    }
    else if ( attitudeRepresentationString.compare("quaternions") == 0 )
    {
	    // Check the size of the input attitude state! 	
	    if (  intialAttitudeStateSize.Size()  != 7 )
	    {
		    std::cout << "FATAL error! The size of the initial attitude state is not equal to 7 elements for the quaternion kinematic representation!" << std::endl; 
		    throw;	
	    }
        initialAttitudeState[0] = initialAttitudeStateEulerIterator->value[0].GetDouble();
        initialAttitudeState[1] = initialAttitudeStateEulerIterator->value[1].GetDouble();
        initialAttitudeState[2] = initialAttitudeStateEulerIterator->value[2].GetDouble();
        initialAttitudeState[3] = initialAttitudeStateEulerIterator->value[3].GetDouble();   
        initialAttitudeState[4] = sml::convertDegreesToRadians(
                                  initialAttitudeStateEulerIterator->value[4].GetDouble() ); 
        initialAttitudeState[5] = sml::convertDegreesToRadians(
                                  initialAttitudeStateEulerIterator->value[5].GetDouble() ); 
        initialAttitudeState[6] = sml::convertDegreesToRadians(
                                  initialAttitudeStateEulerIterator->value[6].GetDouble() ); 

        referenceAttitudeState[0]  = referenceAttitudeStateIterator->value[0].GetDouble();
        referenceAttitudeState[1]  = referenceAttitudeStateIterator->value[1].GetDouble();
        referenceAttitudeState[2]  = referenceAttitudeStateIterator->value[2].GetDouble();  
        referenceAttitudeState[3]  = referenceAttitudeStateIterator->value[3].GetDouble();
    }
    else 
    {
        std::cout << "This attitude representation is not defined! Please enter the initial attitude state in accepted representations! " << std::endl; 
        throw; 
    }

    std::cout << "Initial attitude state in quaternion:                         " << initialAttitudeState[0]<< "," << initialAttitudeState[1]<< "," << initialAttitudeState[2]<< "," << initialAttitudeState[3] << std::endl;
    std::cout << "Initial attitude rates:                                       " << initialAttitudeState[4]<< "," << initialAttitudeState[5]<< "," << initialAttitudeState[6]<< " [rad/sec]" << std::endl;

    std::cout << "Reference state in quaternion:                                " << referenceAttitudeState[0]<< "," << referenceAttitudeState[1]<< "," << referenceAttitudeState[2]<< "," << referenceAttitudeState[3] << std::endl;  
    
    // Extract integrator type. 
    const std::string integratorString                  = find( config, "integrator" )->value.GetString( );
    Integrator integrator = rk4;
    if ( integratorString.compare( "rk4" ) != 0 )
    {
        if ( integratorString.compare( "rkf78" ) == 0 )
        {
            integrator = rkf78;
        }
        else if ( integratorString.compare( "dopri5" ) == 0 )
        {
            integrator = dopri5;
        }
        else if ( integratorString.compare( "bs" ) == 0 )
        {
            integrator = bs;
        }
        else if ( integratorString.compare( "testInt" ) == 0 )
        {
            integrator = testInt;
        }
        else
        {
            std::cout << std::endl;
            std::cerr << "Selected numerical integrator \""
                      << integratorString
                      << "\" is incorrect!" << std::endl;
            throw;
        }
    }
    std::cout << "Integrator                                                    " << integratorString << std::endl;

    // Extract integrator time setttings. 
    const Real startEpoch                              = find( config, "start_epoch")->value.GetDouble( );
    std::cout << "Intergration start epoch:                                     " << startEpoch 
              << "[sec]"        << std::endl; 
    const Real endEpoch                                = find( config, "end_epoch")->value.GetDouble( );
    std::cout << "Integration end epoch:                                        " << endEpoch 
              << "[sec]" << std::endl;
    const Real timeStep                                = find( config, "time_step" )->value.GetDouble( );
    std::cout << "Timestep of the integration is:                               " << timeStep 
              << "[sec]" << std::endl; 
    
    // Extract gravitational parameter of the central body.  
    const Real gravitationalParameter                 = find( config, "gravitational_parameter")->value.GetDouble(); 
    std::cout << "Gravitational Parameter:                                      " << gravitationalParameter
              << "[km^3 s^-2]" << std::endl; 

    // Extract the radial distance of the central body. 
    const Real radius                                 = find( config, "radius")->value.GetDouble();
    std::cout << "Radial vector:                                                "      << radius
              << "[km]" << std::endl; 

    // Exract the semi major axis of the orbit. 
    const Real semiMajorAxis                          = find( config, "semi_major_axis")->value.GetDouble(); 
    std::cout << "Semi major axis:                                              "   << semiMajorAxis
              << "[km]" << std::endl; 

    // Extract integrator tolerances.  
    const Real relativeTolerance                      = find( config, "relative_tolerance")->value.GetDouble(); 
    std::cout << "Relative Tolerance:                                           " << relativeTolerance
              << "[-]" << std::endl; 
    const Real absoluteTolerance                      = find( config, "absolute_tolerance")->value.GetDouble(); 
    std::cout << "Absolute Tolerance:                                           " << absoluteTolerance
              << "[-]" << std::endl;

    // Extract number of samples for monte carlo simulation  
    const int numberOfSamples                  = find( config, "number_0f_samples")->value.GetInt( );  
    
    // Extract gravity gradient model.
    const bool gravityGradientAccelerationModelFlag = find( config, "is_gravity_gradient_active" )->value.GetBool( );
    std::cout << "Is Gravity Gradient disturbance model active?                 " << gravityGradientAccelerationModelFlag << std::endl;

    // Extract assymetric body torque flag. 
    const bool asymmetricBodyTorqueModelFlag    = find( config, "is_asymmetric_body_torque_active")->value.GetBool(); 
    std::cout << "Is asymmetric body torque model active?                       " << asymmetricBodyTorqueModelFlag << std::endl; 

    // Extract actuator model and parameters. 
    const std::string attitudeControlMethod     = find( config, "attitude_control_method")->value.GetString();
    std::cout << "Attitude control method:                                      " << attitudeControlMethod << std::endl; 
    
    // Extract the name of the actuator
    const std::string actuator                  = find( config, "actuator")->value.GetString(); 
    std::cout << "Actuator used for control:                                    " << actuator << std::endl; 

    // Extract reaction wheel configuration type. 
    const std::string reactionWheelConfiguration    = find( config, "reaction_wheel_config_type" )->value.GetString(); 
    std::cout << "The configuration of reaction wheels                          " << reactionWheelConfiguration << std::endl; 

    // Extract reaction wheel attributes.
    std::vector < Vector2 > wheelOrientation; 
    std::vector < std::string > actuatorUuid;  

    // Reaction wheel orientations defined by the user. 
    const rapidjson::Value& reactionWheelsIterator 	= config["wheel_orientation"];
	assert(reactionWheelsIterator.IsArray()); 
    for (rapidjson::Value::ConstValueIterator itr = reactionWheelsIterator.Begin(); itr !=  reactionWheelsIterator.End(); ++itr) 
	{
    	const rapidjson::Value& reactionWheelPropertiesUserDefined = *itr;
        Vector2 orientation( sml::convertDegreesToRadians( reactionWheelPropertiesUserDefined[0].GetDouble() ), 
                             sml::convertDegreesToRadians( reactionWheelPropertiesUserDefined[1].GetDouble() ) ); 
        wheelOrientation.push_back( orientation );
    }

    // Reaction wheel Uuids defined in a separate input file
    std::string reactionWheelUuidFile   = find( config, "reaction_wheel_uuids")->value.GetString(); 

    std::ifstream uuidInputFile( reactionWheelUuidFile );
	std::stringstream jsonDocumentBuffer;
	std::string uuidInputLine;

	while ( std::getline( uuidInputFile, uuidInputLine ) )
	{
	    jsonDocumentBuffer << uuidInputLine << "\n";
	}
 
    rapidjson::Document uuidConfig;
    uuidConfig.Parse( jsonDocumentBuffer.str( ).c_str( ) );

    assert(uuidConfig.IsObject()); 

    for(rapidjson::Value::ConstMemberIterator reactionWheelNameIterator = uuidConfig.MemberBegin(); reactionWheelNameIterator != uuidConfig.MemberEnd(); ++reactionWheelNameIterator )
    {
        actuatorUuid.push_back( reactionWheelNameIterator->value.GetString() ); 
    }

    // Number of reaction wheels in a concept. 
    ConfigIterator numberOfReactionWheelIterator    = find( config, "number_of_reaction_wheels"); 
    const Vector2 numberOfReactionWheels( numberOfReactionWheelIterator->value[0].GetDouble(), numberOfReactionWheelIterator->value[1].GetDouble() ); 

    // Check if the control torque is active.
    const bool controlTorqueActiveModelFlag     = find( config, "is_control_torque_active" )->value.GetBool(); 
    std::cout << "Is control torque active?                                     " << controlTorqueActiveModelFlag << std::endl; 
    
    // Control gains for the controller.  
    const Real naturalFrequency                 = find( config, "natural_frequency")->value.GetDouble(); 
    const Real dampingRatio                     = find( config, "damping_ratio")->value.GetDouble();
    const Real slewSaturationRate               = sml::convertDegreesToRadians ( find( config, "slew_saturation_rate")->value.GetDouble() );
    const std::string controllerType            = find( config, "controller_type")->value.GetString(); 

    // Extract file writer settings.
    const std::string metadataFilePath                  = find( config, "metadata_file_path" )->value.GetString( ); 
    std::cout << "Metadata file path  " <<  metadataFilePath << std::endl;
    const std::string stateHistoryFilePath              = find( config, "state_history_file_path" )->value.GetString( ); 
    std::cout << "State history file path  " <<  stateHistoryFilePath << std::endl;  

    return simulatorInput(  principleInertia,
                            principleInertiaUncertainty, 
                            initialAttitudeState,
                            referenceAttitudeState,
                            numberOfSamples,
                            integrator,
                            startEpoch,
                            endEpoch,
                            timeStep,
                            gravitationalParameter,
                            radius, 
                            semiMajorAxis,
                            relativeTolerance,
                            absoluteTolerance,
                            gravityGradientAccelerationModelFlag,
                            asymmetricBodyTorqueModelFlag,
                            attitudeControlMethod,
                            actuator, 
                            reactionWheelConfiguration,
                            actuatorUuid,
                            wheelOrientation,
                            numberOfReactionWheels,
                            controlTorqueActiveModelFlag,
                            naturalFrequency, 
                            dampingRatio, 
                            slewSaturationRate, 
                            controllerType,
                            metadataFilePath,
                            stateHistoryFilePath    );
};

} // namespace dss_adcs
