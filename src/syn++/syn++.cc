//
// File    : syn++.cc
// ------------------
//
// ES: Elementary Supernova Spectrum Synthesis, Copyright (c) 2010, The
// Regents of the University of California, through Lawrence Berkeley
// National Laboratory (subject to receipt of any required approvals
// from the U.S. Dept. of Energy). All rights reserved.
//
// If you have questions about your rights to use or distribute this
// software, please contact Berkeley Lab's Technology Transfer
// Department at TTD@lbl.gov.
//
// NOTICE. This software was developed under partial funding from the
// U.S. Department of Energy. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, and perform publicly and display
// publicly. Beginning five (5) years after the date permission to
// assert copyright is obtained from the U.S. Department of Energy, and
// subject to any subsequent five (5) year renewals, the U.S. Government
// is granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, distribute copies to the public,
// perform publicly and display publicly, and to permit others to do so.
//

#include "ES_Synow.hh"

#include <yaml-cpp/yaml.h>


#include <fstream>
#include <getopt.h>
#include <cstdlib>
#include <sstream>
//#include <stream>
#include <string>
#include <wordexp.h>
#include <stdlib.h>
//#include <utility>

#if __cplusplus <= 199711L
#define nullptr NULL
#endif

void operator >> ( const YAML::Node& node, ES::Synow::Setup& setup )
{
    setup.a0      = node[ "a0"      ].as<double>();
    setup.a1      = node[ "a1"      ].as<double>();
    setup.a2      = node[ "a2"      ].as<double>();
    setup.v_phot  = node[ "v_phot"  ].as<double>();
    setup.v_outer = node[ "v_outer" ].as<double>();
    setup.t_phot  = node[ "t_phot"  ].as<double>();
    for( size_t i = 0; i < node[ "ions"    ].size(); ++ i ) setup.ions.push_back(    node[ "ions"    ][ i ].as<int>()    );
    for( size_t i = 0; i < node[ "active"  ].size(); ++ i ) setup.active.push_back(  node[ "active"  ][ i ].as<bool>()   );
    for( size_t i = 0; i < node[ "log_tau" ].size(); ++ i ) setup.log_tau.push_back( node[ "log_tau" ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "v_min"   ].size(); ++ i ) setup.v_min.push_back(   node[ "v_min"   ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "v_max"   ].size(); ++ i ) setup.v_max.push_back(   node[ "v_max"   ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "aux"     ].size(); ++ i ) setup.aux.push_back(     node[ "aux"     ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "temp"    ].size(); ++ i ) setup.temp.push_back(    node[ "temp"    ][ i ].as<double>() );
}

void usage( std::ostream& stream )
{
    stream << "usage: syn++ [--verbose] control.yaml" << std::endl;
}

std::string resolve_path(const std::string &i_strPath)
{
	std::string strTemp = i_strPath;
	std::string strResult_File;
	size_t i = 0;
	while ((i = strTemp.find_first_of ("\\", i)) != std::string::npos)
	{
		strTemp.erase(i, 1);
	}
	wordexp_t cResults;
	if (wordexp(strTemp.c_str(),&cResults,WRDE_NOCMD|WRDE_UNDEF) == 0)
	{
		strResult_File = cResults.we_wordv[0];
		wordfree(&cResults);
	}
    if( strResult_File.empty() )
    {
        std::cerr << "syn++: Unable to resolve " << i_strPath << std::endl;
        exit( 137 );
    }
	return strResult_File;
}

bool find_node(YAML::Node & i_ynNode, const std::string & i_szTag, YAML::Node & o_ynNode)
{
	bool bFound = false;
	o_ynNode = YAML::Node();
	
	bFound = i_ynNode[i_szTag].IsDefined();
	if (bFound)
	{
		o_ynNode = i_ynNode[i_szTag];
	}
	return bFound;
}
bool has_node(YAML::Node & i_ynNode, const std::string & i_szTag)
{
	return i_ynNode[i_szTag].IsDefined();
}

int main( int argc, char* argv[] )
{

    // Command line.

    int         verbose = 0;
    std::string target_file;

    while( 1 )
    {

        static struct option long_options[] =
        {
            { "verbose" ,       no_argument, &verbose, 1   },
            { "help"    ,       no_argument,        0, 'h' },
            { "wl-from" , required_argument,        0, 'w' }
        };

        int option_index = 0;
        std::stringstream ss;

        int c = getopt_long( argc, argv, "h", long_options, &option_index );
        if( c == -1 ) break;

        switch( c )
        {
            case 0 :
                break;
            case 'h' :
                usage( std::cout );
                exit( 0 );
            case 'w' :
                ss << optarg;
                ss >> target_file;
                ss.clear();
                break;
            case '?' :
                usage( std::cerr );
                exit( 137 );
            default :
                usage( std::cerr );
                exit( 137 );
        }

    }

    if( ! ( optind < argc ) )
    {
        std::cerr << "syn++: missing control file" << std::endl;
        usage( std::cerr );
        exit( 137 );
    }

    // Configuration in this application comes from a YAML file.

    YAML::Node yaml = YAML::LoadFile( argv[ optind ++ ] );

    // Output spectrum.  Here we create from range and step size, but
    // other methods are supported.  See the named constructors in
    // ES::Spectrum.

    ES::Spectrum output = ES::Spectrum::create_from_range_and_step(
            yaml[ "output" ][ "min_wl"  ].as<double>(),
            yaml[ "output" ][ "max_wl"  ].as<double>(),
            yaml[ "output" ][ "wl_step" ].as<double>() );

    if( ! target_file.empty() ) output = ES::Spectrum::create_from_ascii_file( target_file.c_str() );

    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( output );

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create(
            yaml[ "output" ][ "min_wl"      ].as<double>(),
            yaml[ "output" ][ "max_wl"      ].as<double>(),
            yaml[ "grid"   ][ "bin_width"   ].as<double>(),
            yaml[ "grid"   ][ "v_size"      ].as<double>(),
            yaml[ "grid"   ][ "v_outer_max" ].as<double>() );

    // Opacity operator.

	std::string strLine_Dir;
	std::string strRef_File;
	char * env_line_dir = getenv("SYNXX_LINES_DATA_PATH");
	char * env_ref_file =  getenv("SYNXX_REF_LINE_DATA_PATH");
	if (env_line_dir != nullptr)
		strLine_Dir = resolve_path(env_line_dir);
	if (env_ref_file != nullptr)
		strRef_File = resolve_path(env_ref_file);
	YAML::Node ynOpacity;
	std::string strForm;
	double dV_Ref = -1;
	double dTau_Min = nan("");
	YAML::Node ynJunk;
	if (find_node(yaml,"junt",ynJunk))
	{
		std::cerr << "erroneously found junt" << std::endl;
	}
	if (find_node(yaml,"opacity",ynOpacity))
	{
		YAML::Node ynLine_Dir;
		YAML::Node ynRef_File;
		YAML::Node ynForm;
		YAML::Node ynV_Ref;
		YAML::Node ynLog_Tau_Min;

		if (find_node(ynOpacity,"line_dir",ynLine_Dir))
		{
			strLine_Dir = resolve_path(ynLine_Dir.as<std::string>());
		}

		if (find_node(ynOpacity,"ref_file",ynRef_File))
		{
			strRef_File = resolve_path(ynRef_File.as<std::string>());
		}
		
		if (find_node(ynOpacity,"form",ynForm))
			strForm = ynForm.as<std::string>();
		else
			std::cerr << "opacity/form not specified in file " << target_file << std::endl;
		if (find_node(ynOpacity,"v_ref",ynV_Ref))
			dV_Ref = ynV_Ref.as<double>();
		else
			std::cerr << "opacity/v_ref not specified in file " << target_file << std::endl;
		if (find_node(ynOpacity,"log_tau_min",ynLog_Tau_Min))
			dTau_Min = ynLog_Tau_Min.as<double>();
		else
			std::cerr << "opacity/log_tau_min not specified in file " << target_file << std::endl;
	}
	else
	{
		std::cerr << "opacity section not specified in file " << target_file << std::endl;
		exit(132);
	}
	if (strLine_Dir.empty())
	{
		std::cerr << "line_dir has not been specified. Ensure that it is in the .yaml file under opacity or the environment variable SYNXX_LINES_DATA_PATH has been set." << std::endl;
		exit(132);
	}
	if (strRef_File.empty())
	{
		std::cerr << "ref_file has not been specified. Ensure that it is in the .yaml file under opacity or the environment variable SYNXX_REF_LINE_DATA_PATH has been set." << std::endl;
		exit(132);
	}

	if (std::isnan(dTau_Min) || dV_Ref == -1 || strForm.empty())
		exit(132);

    ES::Synow::Opacity opacity( grid,
            strLine_Dir,
            strRef_File,
            strForm,
            dV_Ref,
            dTau_Min);

    // Source operator.

    ES::Synow::Source source( grid,
            yaml[ "source" ][ "mu_size" ].as<int>() );

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ].as<int>(),
            yaml[ "spectrum" ][ "flatten" ].as<bool>() );

    // Attach setups one by one.

    int count = 0;

    const YAML::Node& setups = yaml[ "setups" ];
    for( YAML::const_iterator iter = setups.begin(); iter != setups.end(); ++ iter )
    {
        ++ count;
        if( verbose ) std::cerr << "computing spectrum " << count << " of " << setups.size() << std::endl;
        ES::Synow::Setup setup;
        *iter >> setup;
        grid( setup );
        std::cout << output << std::endl;
    }

    return 0;
}

#if __cplusplus <= 199711L
#undef nullptr
#endif

