/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <cmath>
#include <iostream>
#include <experimental/filesystem>

#include <catch.hpp>

#include "dss_adcs/apiCall.hpp"
#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/tools.hpp"

namespace dss_adcs
{
namespace tests
{
//! getReactionWheel consists of: callTheApi & getReactionWheelAttributes. For the test,
//! these units will be tested first and the then getReactionWheel as a whole. 

//! Test call the API
/* The product string is generated using system level cURL. If the test 
*  test fails the cURL script should be run again in the test directory to 
*  to update the product strings.
*/ 

TEST_CASE( "Test 1: Test the function callTheApi", "[MR 103D thruster]")
{
    const std::string actuatorUuid      = "15b09050-b756-51e8-85ac-7d809f4a42ef";
    
    const std::string reactionWheelDataStringComputed = callTheApi( actuatorUuid );

    // Generate the reaction wheel data string using cURL. 
    std::string currentPath     = std::experimental::filesystem::current_path(); 
    std::ifstream inputFile(currentPath + "/../test/testProductJson/product.json");

    const std::string reactionWheelDataStringExpected    = getStringFromFile(inputFile); 

    REQUIRE( reactionWheelDataStringComputed.compare(reactionWheelDataStringExpected) == 0);     
} 

TEST_CASE( "Test 2: Test the function callTheApi", "[Micro-wheel 4000 Uuid]")
{
    const std::string actuatorUuid      = "6454b937-f0e9-5fc5-8165-0cc79c44b424";
    
    const std::string reactionWheelDataStringComputed = callTheApi( actuatorUuid );

    // Generate the reaction wheel data string using cURL. 
    std::string currentPath     = std::experimental::filesystem::current_path(); 
    std::ifstream inputFile(currentPath + "/../test/testProductJson/product2.json");

    const std::string reactionWheelDataStringExpected    = getStringFromFile(inputFile); 

    REQUIRE( reactionWheelDataStringComputed.compare(reactionWheelDataStringExpected) == 0);     
} 

TEST_CASE( "Test 3: Test the function callTheApi", "[rw-0-01 Sinclair Interplanetary Uuid]")
{
    const std::string actuatorUuid      = "890c1d1a-0289-5e3b-ae68-c51468ecdaf5";
    
    const std::string reactionWheelDataStringComputed = callTheApi( actuatorUuid );

    // Generate the reaction wheel data string using cURL. 
    std::string currentPath     = std::experimental::filesystem::current_path(); 
    std::ifstream inputFile(currentPath + "/../test/testProductJson/product3.json");

    const std::string reactionWheelDataStringExpected    = getStringFromFile(inputFile); 

    REQUIRE( reactionWheelDataStringComputed.compare(reactionWheelDataStringExpected) == 0);     
} 

//! Test getReactionWheelAttributes. The test if performed by using data from the data sheets 
//! in satsearch. The conversions from units given in the datasheets to SI units is done before 
//! verifying with the values obtained from the getReactionWheelAttribute function. 

TEST_CASE( "Test 4: Test the getReactionWheelAttributes function", "[Microwheel 4000]")
{
    // Tolerance to compensate for machine precision. 
    const Real tolerance = 1e-12; 

    // Use the reaction wheel string from the previous tests from product2.json // 
    const std::string currentPath     = std::experimental::filesystem::current_path(); 
    std::ifstream inputFile(currentPath + "/../test/testProductJson/product2.json");
    const std::string reactionWheelDataString     = getStringFromFile(inputFile); 

    // Extract the properties of microwheel 4000 with the string from the getReactionWheelAttributes
    const ReactionWheel computedReactionWheel     = getReactionWheelAttributes( reactionWheelDataString ); 

    // Define a reaction wheel with the properties of Microwheel 4000 from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel( 3.30, 0.218, 0.081, 0.218, std::nan("radius"), 0.300, "MicroWheel 4000", "Microsat Systems Canada"); 

    REQUIRE( computedReactionWheel.mass         == Approx( expectedReactionWheel.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel.length       == Approx( expectedReactionWheel.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.width        == Approx( expectedReactionWheel.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.height       == Approx( expectedReactionWheel.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel.radius       == Approx( expectedReactionWheel.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.maxTorque    == Approx( expectedReactionWheel.maxTorque).epsilon(tolerance) );
    
    REQUIRE( computedReactionWheel.name.compare( expectedReactionWheel.name )                   == 0);
    REQUIRE( computedReactionWheel.supplierName.compare( expectedReactionWheel.supplierName )   == 0);
        
}

TEST_CASE( "Test 5: Test the getReactionWheelAttributes function", "[rw-0-01 Sinclair Interplanetary Uuid]")
{
    // Tolerance to compensate for machine precision. 
    const Real tolerance = 1e-12; 

    // Use the reaction wheel string from the previous tests from product3.json // 
    const std::string currentPath     = std::experimental::filesystem::current_path(); 
    std::ifstream inputFile(currentPath + "/../test/testProductJson/product3.json");
    const std::string reactionWheelDataString     = getStringFromFile(inputFile); 

    // Extract the properties ofrw-0-01 Sinclair Interplanetary with the string from the getReactionWheelAttributes
    const ReactionWheel computedReactionWheel     = getReactionWheelAttributes( reactionWheelDataString ); 

    // Define a reaction wheel with the properties ofrw-0-01 Sinclair Interplanetary from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel( 0.12, 0.05, 0.03, 0.05, std::nan("radius"), 0.001, "RW-0.01", "Sinclair Interplanetary"); 

    REQUIRE( computedReactionWheel.mass         == Approx( expectedReactionWheel.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel.length       == Approx( expectedReactionWheel.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.width        == Approx( expectedReactionWheel.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.height       == Approx( expectedReactionWheel.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel.radius       == Approx( expectedReactionWheel.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.maxTorque    == Approx( expectedReactionWheel.maxTorque).epsilon(tolerance) );
    
    REQUIRE( computedReactionWheel.name.compare( expectedReactionWheel.name )                   == 0);
    REQUIRE( computedReactionWheel.supplierName.compare( expectedReactionWheel.supplierName )   == 0);
        
}

TEST_CASE( "Test 6: Test the getReactionWheelAttributes function", "[rwp050 Blue Canyon Technologies Uuid]")
{
    // Tolerance to compensate for machine precision. 
    const Real tolerance = 1e-12; 

    // Use the callTheApi function to get the reaction wheel string
    const std::string reactionWheelDataString     = callTheApi("b4eb8ec7-f484-5357-a3e7-634c630d0a3e"); 

    // Extract the properties of rwp050 Blue Canyon Technologies with the string from the getReactionWheelAttributes
    const ReactionWheel computedReactionWheel     = getReactionWheelAttributes( reactionWheelDataString ); 

    // Define a reaction wheel with the properties of rwp050 Blue Canyon Technologies from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel( 0.24, 0.058, 0.025, 0.058, std::nan("radius"), 0.007, "RWP050", "Blue Canyon Technologies"); 

    REQUIRE( computedReactionWheel.mass         == Approx( expectedReactionWheel.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel.length       == Approx( expectedReactionWheel.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.width        == Approx( expectedReactionWheel.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.height       == Approx( expectedReactionWheel.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel.radius       == Approx( expectedReactionWheel.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel.maxTorque    == Approx( expectedReactionWheel.maxTorque).epsilon(tolerance) );
    
    REQUIRE( computedReactionWheel.name.compare( expectedReactionWheel.name )                   == 0);
    REQUIRE( computedReactionWheel.supplierName.compare( expectedReactionWheel.supplierName )   == 0);
        
}

//! Integration testing for the function getReactionWheel. 

TEST_CASE( "Test 7: Test the getReactionWheel function", "[rwp050 Blue Canyon Technologies Uuid]")
{
    // Tolerance to compensate for machine precision. 
    const Real tolerance = 1e-12; 

    // Actuator uuid. 
    std::vector< std::string > actuatorUuid; 
    actuatorUuid.push_back( "b4eb8ec7-f484-5357-a3e7-634c630d0a3e" );
    actuatorUuid.push_back( "890c1d1a-0289-5e3b-ae68-c51468ecdaf5" );
    actuatorUuid.push_back( "6454b937-f0e9-5fc5-8165-0cc79c44b424" ); 

    // Get the reaction wheel properties for BCT rwp050 from the getReactionWheelAttribute. 
    const std::vector <ReactionWheel> computedReactionWheel = getReactionWheels( "actuator" , actuatorUuid );

    // Define a reaction wheel with the properties of rwp050 Blue Canyon Technologies from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel( 0.24, 0.058, 0.025, 0.058, std::nan("radius"), 0.007, "RWP050", "Blue Canyon Technologies"); 
    
    // Define a reaction wheel with the properties ofrw-0-01 Sinclair Interplanetary from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel2( 0.12, 0.05, 0.03, 0.05, std::nan("radius"), 0.001, "RW-0.01", "Sinclair Interplanetary"); 

    // Define a reaction wheel with the properties of Microwheel 4000 from satsearch datasheet. 
    const ReactionWheel expectedReactionWheel3( 3.30, 0.218, 0.081, 0.218, std::nan("radius"), 0.300, "MicroWheel 4000", "Microsat Systems Canada"); 

    // Check for the attributes for BCT computed and expected. 
    REQUIRE( computedReactionWheel[0].mass         == Approx( expectedReactionWheel.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel[0].length       == Approx( expectedReactionWheel.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[0].width        == Approx( expectedReactionWheel.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[0].height       == Approx( expectedReactionWheel.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel[0].radius       == Approx( expectedReactionWheel.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[0].maxTorque    == Approx( expectedReactionWheel.maxTorque).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[0].name.compare( expectedReactionWheel.name )                   == 0);
    REQUIRE( computedReactionWheel[0].supplierName.compare( expectedReactionWheel.supplierName )   == 0);

    // Check the attributs for RW0-001 
    REQUIRE( computedReactionWheel[1].mass         == Approx( expectedReactionWheel2.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel[1].length       == Approx( expectedReactionWheel2.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[1].width        == Approx( expectedReactionWheel2.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[1].height       == Approx( expectedReactionWheel2.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel[1].radius       == Approx( expectedReactionWheel2.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[1].maxTorque    == Approx( expectedReactionWheel2.maxTorque).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[1].name.compare( expectedReactionWheel2.name )                   == 0);
    REQUIRE( computedReactionWheel[1].supplierName.compare( expectedReactionWheel2.supplierName )   == 0);

    // Check the attribtues for BCT rwp050.   
    REQUIRE( computedReactionWheel[2].mass         == Approx( expectedReactionWheel3.mass).epsilon(tolerance) ); 
    REQUIRE( computedReactionWheel[2].length       == Approx( expectedReactionWheel3.length).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[2].width        == Approx( expectedReactionWheel3.width).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[2].height       == Approx( expectedReactionWheel3.height).epsilon(tolerance) );
    // REQUIRE( computedReactionWheel[2].radius       == Approx( expectedReactionWheel3.radius).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[2].maxTorque    == Approx( expectedReactionWheel3.maxTorque).epsilon(tolerance) );
    REQUIRE( computedReactionWheel[2].name.compare( expectedReactionWheel3.name )                   == 0);
    REQUIRE( computedReactionWheel[2].supplierName.compare( expectedReactionWheel3.supplierName )   == 0);

}

} // tests 

} // dss_adcs
