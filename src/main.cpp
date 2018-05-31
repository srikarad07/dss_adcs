/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <rapidjson/document.h>

#include "dss_adcs/bulkSimulator.hpp"
#include "dss_adcs/simulator.hpp"
// #include "dss_adcs/tools.hpp"
// #include "dss_adcs/typedefs.hpp"

int main( const int numberOfInputs, const char* inputArguments[ ] )
{
	///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                              dss - adcs                          " << std::endl;
    std::cout << std::endl;
    std::cout << "     Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology" << std::endl;
    std::cout << "                           (srikarad007@gmail.com)                       " << std::endl; 
    std::cout << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                      Extract input parameters                    " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;


    // Check that only one input has been provided (a JSON file).
    if ( numberOfInputs - 1 != 1 )
    {
        std::cerr << "ERROR: Number of inputs is wrong. Please only provide a JSON input file!"
                  << std::endl;
        throw;
    }

    // Read and store JSON input document (filter out comment lines).
    // TODO: Need to make comment-line filtering more robust.
    std::ifstream inputFile( inputArguments[ 1 ] );
    std::stringstream jsonDocumentBuffer;
    std::string inputLine;
    while ( std::getline( inputFile, inputLine ) )
    {
        size_t startPosition = inputLine.find_first_not_of( " \t" );
        if ( std::string::npos != startPosition )
        {
            inputLine = inputLine.substr( startPosition );
        }

        if ( inputLine.substr( 0, 2 ) != "//" )
        {
            jsonDocumentBuffer << inputLine << "\n";
        }
    }

    rapidjson::Document config;
    config.Parse( jsonDocumentBuffer.str( ).c_str( ) );

    rapidjson::Value::MemberIterator modeIterator = config.FindMember( "mode" );
    if ( modeIterator == config.MemberEnd( ) )
    {
        std::cerr << "ERROR: Configuration option \"mode\" could not be found in JSON input!"
                  << std::endl;
        throw;
    }
    std::string mode = modeIterator->value.GetString( );
    std::transform( mode.begin( ), mode.end( ), mode.begin( ), ::tolower );
  
    if ( mode.compare( "simulator") == 0 )
    {
        std::cout << "Mode                               " << mode << std::endl;
        dss_adcs::executeSingleSimulator( config );
    }
    else if ( mode.compare("bulk_simulator") == 0)
    {
        std::cout << "Mode                               " << mode << std::endl;
        // dss_adcs::executeSingleSimulator( config );
        dss_adcs::executeBulkSimulator( config );
    }
    else
    {
        std::cout << std::endl;
        std::cerr << "ERROR: Requested \"mode\" << mode << is invalid!" << std::endl;
        throw;
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                         Exited successfully!                     " << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    return EXIT_SUCCESS;

    ///////////////////////////////////////////////////////////////////////////
}
