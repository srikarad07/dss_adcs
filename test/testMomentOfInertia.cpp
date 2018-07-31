/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/
#include <cmath>

#include <catch.hpp>

#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs 
{
namespace tests 
{

TEST_CASE( "Test 1: Moment of Inertia functions: ", "[moment of Inertia]" ) 
{
    const std::string actuator = "reactionWheels"; 
    
    // Set tolerance = error between expected value and computed value.
    const Real tolerance = 1.0e-6;
    
    std::vector < std::string > actuatorUuids;

    actuatorUuids.push_back( "759fef15-c22a-5b9f-b461-894500bfc0c1" ); // rw35
    actuatorUuids.push_back( "0624bb6f-363b-5669-a111-4138a841cd01" ); // rw90 
    actuatorUuids.push_back( "07e0ee8d-2dc0-59b2-91e2-5e99a0bc65b7" ); // rw150
    actuatorUuids.push_back( "e35b9c57-d5a2-58b5-858a-a5bf6c15604d" ); // rw250 

    std::vector< ReactionWheel > reactionWheels = getReactionWheels( actuator, actuatorUuids );  

    ActuatorConfiguration actuatorConfiguration( reactionWheels );

    const VectorXd calculatedMomentsOfInertia = actuatorConfiguration.calculateMomentOfInertia(); 

    const Vector4 expectedMomentsOfInertia( 1.9e-4, 4.2e-4, 1.6e-3, 5.5e-3 ); 

    // const Vector4 errorInMomentOfInertia    = 0.2 * calculatedMomentsOfInertia - expectedMomentsOfInertia; 

    Vector4 errorPercentMomentOfInertia; 
    errorPercentMomentOfInertia[0]   = 100.0 * abs( 0.3 * calculatedMomentsOfInertia[0] - expectedMomentsOfInertia[0] ) / expectedMomentsOfInertia[0];
    errorPercentMomentOfInertia[1]   = 100.0 * abs( 0.3 * calculatedMomentsOfInertia[1] - expectedMomentsOfInertia[1] ) / expectedMomentsOfInertia[1];
    errorPercentMomentOfInertia[2]   = 100.0 * abs( 0.4 * calculatedMomentsOfInertia[2] - expectedMomentsOfInertia[2] ) / expectedMomentsOfInertia[2];
    errorPercentMomentOfInertia[3]   = 100.0 * abs( 0.4 * calculatedMomentsOfInertia[3] - expectedMomentsOfInertia[3] ) / expectedMomentsOfInertia[3]; 
    
    std::cout << "Percentage error in moment of inertia: " << errorPercentMomentOfInertia << std::endl;

    const Real tolerancePercent                 = 20.0 ; 
    REQUIRE( errorPercentMomentOfInertia[0]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[1]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[2]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[3]  <= tolerancePercent ); 

    // REQUIRE( errorInMomentOfInertia[0]  <= tolerance ); 
    // REQUIRE( errorInMomentOfInertia[1]  <= tolerance ); 
    // REQUIRE( errorInMomentOfInertia[2]  <= tolerance ); 
    // REQUIRE( errorInMomentOfInertia[3]  <= tolerance ); 

    // REQUIRE( calculatedMomentsOfInertia[0] == Approx( expectedMomentsOfInertia[0] ).epsilon(tolerance) ); 
    // REQUIRE( calculatedMomentsOfInertia[1] == Approx( expectedMomentsOfInertia[1] ).epsilon(tolerance) ); 
    // REQUIRE( calculatedMomentsOfInertia[2] == Approx( expectedMomentsOfInertia[2] ).epsilon(tolerance) ); 
    // REQUIRE( calculatedMomentsOfInertia[3] == Approx( expectedMomentsOfInertia[3] ).epsilon(tolerance) ); 

}

TEST_CASE( "Test 2: Moment of Inertia functions: ", "[moment of Inertia]" ) 
{
    typedef Eigen::Matrix< double, 6, 1 > Vector6;

    const std::string actuator = "reactionWheels"; 
    
    // Set tolerance = error between expected value and computed value.
    const Real tolerance = 1.0e-6;
    
    std::vector < std::string > actuatorUuids;

    // Surrey Sattelite technology reaction wheels     
    ReactionWheel spmReactionWheel( 0.96, 
                                      std::nan("length"), 
                                      sml::convertMillimeterToMeter(101.0), 
                                      std::nan("length"), 
                                      sml::convertMillimeterToMeter(109.0 / 2.0), 
                                      0.011, 
                                      "10spm", 
                                      "surrey-satellite" ); 
    ReactionWheel spOReactionWheel( 2.6, 
                                       std::nan("length"), 
                                       sml::convertMillimeterToMeter(120.0), 
                                       std::nan("length"), 
                                       sml::convertMillimeterToMeter(131.0 / 2.0), 
                                       0.11, 
                                       "10spo", 
                                       "surrey-satellite" );

    // New space systems reaction wheels 
    ReactionWheel nrwaT065ReactionWheel( 1.55, 
                                         sml::convertMillimeterToMeter(102.0), 
                                         sml::convertMillimeterToMeter(105.0), 
                                         sml::convertMillimeterToMeter(102.0), 
                                         std::nan("radius"), 
                                         sml::convertMilliNewtonMeterToNewtonMeter(20.0), 
                                         "nrwaT065", 
                                         "new-space-systems" );

    ReactionWheel nrwaT10ReactionWheel( 5.0, 
                                        std::nan("length"), 
                                        sml::convertMillimeterToMeter(71.0), 
                                        std::nan("length"), 
                                        sml::convertMillimeterToMeter(235.0 / 2.0),
                                        sml::convertMilliNewtonMeterToNewtonMeter(210.0), 
                                        "nrwaT10", 
                                        "new-space-systems" );

    // Vectronic space reaction wheels. 
    ReactionWheel vrw02ReactionWheel( 1.0, 
                                      sml::convertMillimeterToMeter(70.0), 
                                      sml::convertMillimeterToMeter(48.0), 
                                      sml::convertMillimeterToMeter(70.0), 
                                      std::nan("radius"), 
                                      sml::convertMilliNewtonMeterToNewtonMeter(20.0), 
                                      "vrw02", 
                                      "vectornic" );
    ReactionWheel vrw01ReactionWheel( 1.8, 
                                      sml::convertMillimeterToMeter(115.0), 
                                      sml::convertMillimeterToMeter(77.0), 
                                      sml::convertMillimeterToMeter(115.0), 
                                      std::nan("radius"), 
                                      sml::convertMilliNewtonMeterToNewtonMeter(25.0), 
                                      "vrw01", 
                                      "vectornic" );

    std::vector< ReactionWheel > reactionWheels; 
    reactionWheels.push_back(spmReactionWheel);
    reactionWheels.push_back(spOReactionWheel);
    reactionWheels.push_back(nrwaT065ReactionWheel);
    reactionWheels.push_back(nrwaT10ReactionWheel);
    reactionWheels.push_back(vrw02ReactionWheel);
    reactionWheels.push_back(vrw01ReactionWheel);  

    ActuatorConfiguration actuatorConfiguration( reactionWheels );

    const VectorXd calculatedMomentsOfInertia = actuatorConfiguration.calculateMomentOfInertia(); 

    Vector6 expectedMomentsOfInertia; 
    expectedMomentsOfInertia[0] = 8.0e-4;
    expectedMomentsOfInertia[1] = 2.8e-3;
    expectedMomentsOfInertia[2] = 1.0e-3;
    expectedMomentsOfInertia[3] = 0.02;
    expectedMomentsOfInertia[4] = 3.4e-4;
    expectedMomentsOfInertia[5] = 2.0e-3; 

    // const Vector4 errorInMomentOfInertia    = 0.2 * calculatedMomentsOfInertia - expectedMomentsOfInertia; 

    Vector6 errorPercentMomentOfInertia; 
    errorPercentMomentOfInertia[0]   = 100.0 * abs( 0.3 * calculatedMomentsOfInertia[0] - expectedMomentsOfInertia[0] ) / expectedMomentsOfInertia[0];
    errorPercentMomentOfInertia[1]   = 100.0 * abs( 0.3 * calculatedMomentsOfInertia[1] - expectedMomentsOfInertia[1] ) / expectedMomentsOfInertia[1];
    errorPercentMomentOfInertia[2]   = 100.0 * abs( 0.4 * calculatedMomentsOfInertia[2] - expectedMomentsOfInertia[2] ) / expectedMomentsOfInertia[2];
    errorPercentMomentOfInertia[3]   = 100.0 * abs( 0.3 * calculatedMomentsOfInertia[3] - expectedMomentsOfInertia[3] ) / expectedMomentsOfInertia[3]; 
    errorPercentMomentOfInertia[4]   = 100.0 * abs( 0.4 * calculatedMomentsOfInertia[4] - expectedMomentsOfInertia[4] ) / expectedMomentsOfInertia[4]; 
    errorPercentMomentOfInertia[5]   = 100.0 * abs( 0.4 * calculatedMomentsOfInertia[5] - expectedMomentsOfInertia[5] ) / expectedMomentsOfInertia[5]; 
    std::cout << "Percentage error in moment of inertia: " << errorPercentMomentOfInertia << std::endl;
    const Real tolerancePercent                 = 25.0 ; 
    REQUIRE( errorPercentMomentOfInertia[0]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[1]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[2]  <= tolerancePercent ); 
    REQUIRE( errorPercentMomentOfInertia[3]  <= tolerancePercent ); 

}

// TEST_CASE( "Test 2: Moment of Inertia functions: ", "[moment of Inertia]" ) 
// {
//     const std::string actuator = "reactionWheels"; 
    
//     // Set tolerance = error between expected value and computed value.
//     const Real tolerance = 1.0e-6;
    
//     std::vector < std::string > actuatorUuids;

//     actuatorUuids.push_back( "0a3db8e9-025a-59e1-92c3-68c007f459c1" ); // 10sp-o surrey satellite 
//     actuatorUuids.push_back(  "8b028105-5fa8-5023-8b9b-c19e152c0608" ); // 10 sp-m surrey satellite 

//     std::vector< ReactionWheel > reactionWheels = getReactionWheels( actuator, actuatorUuids );  

//     ActuatorConfiguration actuatorConfiguration( reactionWheels );

//     const VectorXd calculatedMomentsOfInertia = actuatorConfiguration.calculateMomentOfInertia(); 

//     const Vector2 expectedMomentsOfInertia( 8.0e-4, 2.8e-3 ); 

//     // const Vector2 errorInMomentOfInertia    = 0.2 * calculatedMomentsOfInertia - expectedMomentsOfInertia; 

//     Vector2 errorPercentMomentOfInertia; 
//     errorPercentMomentOfInertia[0]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[0] - expectedMomentsOfInertia[0] ) / expectedMomentsOfInertia[0];
//     errorPercentMomentOfInertia[1]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[1] - expectedMomentsOfInertia[1] ) / expectedMomentsOfInertia[1];

//     const Real tolerancePercent                 = 10.0 ; 
//     REQUIRE( errorPercentMomentOfInertia[0]  <= tolerancePercent ); 
//     REQUIRE( errorPercentMomentOfInertia[1]  <= tolerancePercent ); 

// }

// TEST_CASE( "Test 2: Moment of Inertia functions: ", "[moment of Inertia]" ) 
// {
//     const std::string actuator = "reactionWheels"; 
    
//     // Set tolerance = error between expected value and computed value.
//     const Real tolerance = 1.0e-6;
    
//     std::vector < std::string > actuatorUuids;

//     actuatorUuids.push_back( "8f4157de-f6d4-5f47-9067-bb9569c849b1" ); // nrwa-t10
//     actuatorUuids.push_back( "8e958826-7064-5e8b-a8f2-4e7035d5887a" ); // nrwa-t-065
    
//     std::vector< ReactionWheel > reactionWheels = getReactionWheels( actuator, actuatorUuids );  

//     ActuatorConfiguration actuatorConfiguration( reactionWheels );

//     const VectorXd calculatedMomentsOfInertia = actuatorConfiguration.calculateMomentOfInertia(); 

//     const Vector2 expectedMomentsOfInertia( 1.0e-13, 0.02 ); 

//     // const Vector2 errorInMomentOfInertia    = 0.2 * calculatedMomentsOfInertia - expectedMomentsOfInertia; 

//     Vector2 errorPercentMomentOfInertia; 
//     errorPercentMomentOfInertia[0]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[0] - expectedMomentsOfInertia[0] ) / expectedMomentsOfInertia[0];
//     errorPercentMomentOfInertia[1]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[1] - expectedMomentsOfInertia[1] ) / expectedMomentsOfInertia[1];
//     errorPercentMomentOfInertia[2]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[2] - expectedMomentsOfInertia[2] ) / expectedMomentsOfInertia[2];
//     errorPercentMomentOfInertia[3]   = 100.0 *( 0.3 * calculatedMomentsOfInertia[3] - expectedMomentsOfInertia[3] ) / expectedMomentsOfInertia[3]; 

//     const Real tolerancePercent                 = 10.0 ; 
//     REQUIRE( errorPercentMomentOfInertia[0]  <= tolerancePercent ); 
//     REQUIRE( errorPercentMomentOfInertia[1]  <= tolerancePercent ); 
//     REQUIRE( errorPercentMomentOfInertia[2]  <= tolerancePercent ); 
//     REQUIRE( errorPercentMomentOfInertia[3]  <= tolerancePercent ); 

// } 

} // tests 

} // dss_adcs