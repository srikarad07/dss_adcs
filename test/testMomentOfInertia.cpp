/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <catch.hpp>

#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs 
{
namespace tests 
{
TEST_CASE( "Test Moment of Inertia functions: ", "[moment of Inertia]" ) 
{
    const std::string actuator = "reactionWheels"; 
    
    // Set tolerance = error between expected value and computed value.
    const Real tolerance = 1.0e-1;
    
    std::vector < std::string > actuatorUuids;

    actuatorUuids.push_back( "759fef15-c22a-5b9f-b461-894500bfc0c1" ); // rw35
    actuatorUuids.push_back( "0624bb6f-363b-5669-a111-4138a841cd01" ); // rw90 
    actuatorUuids.push_back( "07e0ee8d-2dc0-59b2-91e2-5e99a0bc65b7" ); // rw150
    actuatorUuids.push_back( "e35b9c57-d5a2-58b5-858a-a5bf6c15604d" ); // rw250 

    std::vector< ReactionWheel > reactionWheels = getReactionWheel( actuator, actuatorUuids );  

    ActuatorConfiguration actuatorConfiguration( reactionWheels );

    const VectorXd caluclatedMomentsOfInertia = actuatorConfiguration.calculateMomentOfInertia(); 

    Vector4 expectedMomentsOfInertia( 1.9e-4, 4.2e-4, 1.6e-3, 5.5e-3 ); 

    REQUIRE( calculateMomentOfInertia[0] == expectedMomentsOfInertia[0].epsilon(tolerance) ); 
    REQUIRE( calculateMomentOfInertia[1] == expectedMomentsOfInertia[1].epsilon(tolerance) ); 
    REQUIRE( calculateMomentOfInertia[2] == expectedMomentsOfInertia[2].epsilon(tolerance) ); 
    REQUIRE( calculateMomentOfInertia[3] == expectedMomentsOfInertia[3].epsilon(tolerance) ); 

} 

} // tests 

} // dss_adcs