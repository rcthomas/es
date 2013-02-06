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

#include <config.h>

#ifdef USE_GUI
#include <gtkmm.h>
#include <Python.h>
#include <fstream>
#endif

#include <fstream>
#include <getopt.h>
#include <cstdlib>
#include <sstream>

void operator >> ( const YAML::Node& node, ES::Synow::Setup& setup )
{
    node[ "a0"      ] >> setup.a0;
    node[ "a1"      ] >> setup.a1;
    node[ "a2"      ] >> setup.a2;
    node[ "v_phot"  ] >> setup.v_phot;
    node[ "v_outer" ] >> setup.v_outer;
    node[ "t_phot"  ] >> setup.t_phot;
    for( size_t i = 0; i < node[ "ions"    ].size(); ++ i ) setup.ions.push_back( node[ "ions" ][ i ] );
    for( size_t i = 0; i < node[ "active"  ].size(); ++ i ) setup.active.push_back( node[ "active" ][ i ] );
    for( size_t i = 0; i < node[ "log_tau" ].size(); ++ i ) setup.log_tau.push_back( node[ "log_tau" ][ i ] );
    for( size_t i = 0; i < node[ "v_min"   ].size(); ++ i ) setup.v_min.push_back( node[ "v_min" ][ i ] );
    for( size_t i = 0; i < node[ "v_max"   ].size(); ++ i ) setup.v_max.push_back( node[ "v_max" ][ i ] );
    for( size_t i = 0; i < node[ "aux"     ].size(); ++ i ) setup.aux.push_back( node[ "aux" ][ i ] );
    for( size_t i = 0; i < node[ "temp"    ].size(); ++ i ) setup.temp.push_back( node[ "temp" ][ i ] );
}

void usage( std::ostream& stream )
{
    stream << "usage: syn++ [--verbose] control.yaml" << std::endl;
}

#ifdef USE_GUI
/* pointers for all the widgets */
Gtk::Window *pWindow = 0;
Gtk::Button *pQuitButton = 0, *pPlotSpecBtn = 0;
Gtk::SpinButton *pv_phot_spinbtn = 0, *pT_phot_spinbtn = 0, *pv_outer_spinbtn = 0,
                *pmin_wl_spinbtn = 0, *pmax_wl_spinbtn = 0, *pwl_step_spinbtn = 0;
Gtk::SpinButton *p_s01_kid_spinbtn    = 0, *p_s02_kid_spinbtn    = 0,
                *p_s01_logtau_spinbtn = 0, *p_s02_logtau_spinbtn = 0,
                *p_s01_v_min_spinbtn  = 0, *p_s02_v_min_spinbtn  = 0,
                *p_s01_v_max_spinbtn  = 0, *p_s02_v_max_spinbtn  = 0,
                *p_s01_aux_spinbtn    = 0, *p_s02_aux_spinbtn    = 0,
                *p_s01_T_ex_spinbtn   = 0, *p_s02_T_ex_spinbtn   = 0;
Gtk::CheckButton *p_s01_active_checkbtn = 0, *p_s02_active_checkbtn = 0;

static void quit_syngui() {
    if (pWindow) {
        pWindow->hide();
    }
}

static void plot_spectrum() {
    ES::Spectrum output = ES::Spectrum::create_from_range_and_step(
            pmin_wl_spinbtn->get_value(),
            pmax_wl_spinbtn->get_value(),
            pwl_step_spinbtn->get_value() );

    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( output );

    ES::Synow::Grid grid = ES::Synow::Grid::create(
            pmin_wl_spinbtn->get_value(),
            pmax_wl_spinbtn->get_value(),
            0.3,  // TODO: fix this later
            100,  // TODO: fix this later
            60.0 ); // TODO: fix this later

    ES::Synow::Opacity opacity( grid,
            "/home/brian/es-data/lines", // TODO: fix this later
            "/home/brian/es-data/refs.dat", // TODO: fix this later
            "exp", // TODO: fix this later
            6.0, // TODO: fix this later
            -2.0 ); // TODO: fix this later

    ES::Synow::Source source( grid,
            10 ); // TODO: fix this later

    ES::Synow::Spectrum spectrum( grid, output, reference,
            60, // TODO: fix this later
            true ); // TODO: fix this later

    ES::Synow::Setup setup;
    setup.a0 = 1.0; // TODO: fix this later
    setup.a1 = 0.0; // TODO: fix this later
    setup.a2 = 0.0; // TODO: fix this later
    setup.v_phot = pv_phot_spinbtn->get_value();
    setup.v_outer = pv_outer_spinbtn->get_value();
    setup.t_phot = pT_phot_spinbtn->get_value();

    setup.ions.push_back(p_s01_kid_spinbtn->get_value());
    setup.ions.push_back(p_s02_kid_spinbtn->get_value());

    setup.active.push_back(p_s01_active_checkbtn->get_active());
    setup.active.push_back(p_s02_active_checkbtn->get_active());

    setup.log_tau.push_back(p_s01_logtau_spinbtn->get_value());
    setup.log_tau.push_back(p_s02_logtau_spinbtn->get_value());

    setup.v_min.push_back(p_s01_v_min_spinbtn->get_value());
    setup.v_min.push_back(p_s02_v_min_spinbtn->get_value());

    setup.v_max.push_back(p_s01_v_max_spinbtn->get_value());
    setup.v_max.push_back(p_s02_v_max_spinbtn->get_value());

    setup.aux.push_back(p_s01_aux_spinbtn->get_value());
    setup.aux.push_back(p_s02_aux_spinbtn->get_value());

    setup.temp.push_back(p_s01_T_ex_spinbtn->get_value());
    setup.temp.push_back(p_s02_T_ex_spinbtn->get_value());

    grid( setup );

    std::ofstream tmp_spec_output;
    tmp_spec_output.open("tmp.spec");
    tmp_spec_output << output << std::endl;

    PyRun_SimpleString("wl, f, err = np.loadtxt('tmp.spec', unpack=True)");
    PyRun_SimpleString("plt.plot(wl, f)");
    PyRun_SimpleString("plt.show()");

    tmp_spec_output.close();
}
#endif

int main( int argc, char* argv[] )
{
#ifdef USE_GUI
 /* make sure to call this FIRST, otherwise you'll get spammed with errors about
  * 'you forgot to call g_type_init()!' */
    Gtk::Main kit(argc, argv);

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    try {
        builder->add_from_file("syn++-gui-gtk2.glade");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    builder->get_widget("syn++-gui_window", pWindow);

    builder->get_widget("quit_btn", pQuitButton);
    if (pQuitButton) {
        pQuitButton->signal_clicked().connect (sigc::ptr_fun(quit_syngui));
    }

    builder->get_widget("plot_spec_btn", pPlotSpecBtn);
    if (pPlotSpecBtn) {
        pPlotSpecBtn->signal_clicked().connect (sigc::ptr_fun(plot_spectrum));
    }

    builder->get_widget("s01_kid_spinbtn", p_s01_kid_spinbtn);
    builder->get_widget("s01_active_checkbtn", p_s01_active_checkbtn);
    builder->get_widget("s01_logtau_spinbtn", p_s01_logtau_spinbtn);
    builder->get_widget("s01_v_min_spinbtn", p_s01_v_min_spinbtn);
    builder->get_widget("s01_v_max_spinbtn", p_s01_v_max_spinbtn);
    builder->get_widget("s01_aux_spinbtn", p_s01_aux_spinbtn);
    builder->get_widget("s01_T_ex_spinbtn", p_s01_T_ex_spinbtn);

    builder->get_widget("s02_kid_spinbtn", p_s02_kid_spinbtn);
    builder->get_widget("s02_active_checkbtn", p_s02_active_checkbtn);
    builder->get_widget("s02_logtau_spinbtn", p_s02_logtau_spinbtn);
    builder->get_widget("s02_v_min_spinbtn", p_s02_v_min_spinbtn);
    builder->get_widget("s02_v_max_spinbtn", p_s02_v_max_spinbtn);
    builder->get_widget("s02_aux_spinbtn", p_s02_aux_spinbtn);
    builder->get_widget("s02_T_ex_spinbtn", p_s02_T_ex_spinbtn);

    builder->get_widget("v_phot_spinbtn", pv_phot_spinbtn);
    builder->get_widget("T_phot_spinbtn", pT_phot_spinbtn);
    builder->get_widget("v_outer_spinbtn", pv_outer_spinbtn);
    builder->get_widget("min_wl_spinbtn", pmin_wl_spinbtn);
    builder->get_widget("max_wl_spinbtn", pmax_wl_spinbtn);
    builder->get_widget("wl_step_spinbtn", pwl_step_spinbtn);

    Py_Initialize();
    PyRun_SimpleString("import pylab as plt");
    PyRun_SimpleString("import numpy as np");

    kit.run(*pWindow);
#endif

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

    YAML::Node yaml;

#ifndef USE_GUI
    {
        std::ifstream  stream( argv[ optind ++ ] );
        YAML::Parser   parser( stream );
        parser.GetNextDocument( yaml );
        stream.close();
    }

    // Output spectrum.  Here we create from range and step size, but
    // other methods are supported.  See the named constructors in
    // ES::Spectrum.

    ES::Spectrum output = ES::Spectrum::create_from_range_and_step(
            yaml[ "output" ][ "min_wl"  ],
            yaml[ "output" ][ "max_wl"  ],
            yaml[ "output" ][ "wl_step" ] );

    if( ! target_file.empty() ) output = ES::Spectrum::create_from_ascii_file( target_file.c_str() );

    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( output );

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create(
            yaml[ "output" ][ "min_wl"      ],
            yaml[ "output" ][ "max_wl"      ],
            yaml[ "grid"   ][ "bin_width"   ],
            yaml[ "grid"   ][ "v_size"      ],
            yaml[ "grid"   ][ "v_outer_max" ] );

    // Opacity operator.

    ES::Synow::Opacity opacity( grid,
            yaml[ "opacity" ][ "line_dir"    ],
            yaml[ "opacity" ][ "ref_file"    ],
            yaml[ "opacity" ][ "form"        ],
            yaml[ "opacity" ][ "v_ref"       ],
            yaml[ "opacity" ][ "log_tau_min" ] );

    // Source operator.

    ES::Synow::Source source( grid,
            yaml[ "source" ][ "mu_size" ] );

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ],
            yaml[ "spectrum" ][ "flatten" ] );

    // Attach setups one by one.

    int count = 0;

    const YAML::Node& setups = yaml[ "setups" ];
    for( YAML::Iterator iter = setups.begin(); iter != setups.end(); ++ iter )
    {
        ++ count;
        if( verbose ) std::cerr << "computing spectrum " << count << " of " << setups.size() << std::endl;
        ES::Synow::Setup setup;
        *iter >> setup;
        grid( setup );
        std::cout << output << std::endl;
    }
#endif

#ifdef USE_GUI
    delete pWindow;
    delete pQuitButton;
    delete pPlotSpecBtn;
    delete pv_phot_spinbtn;
    delete pT_phot_spinbtn;
    delete pv_outer_spinbtn;
    delete pmin_wl_spinbtn;
    delete pmax_wl_spinbtn;
    delete pwl_step_spinbtn;

    delete p_s01_kid_spinbtn;
    delete p_s01_active_checkbtn;
    delete p_s01_logtau_spinbtn;
    delete p_s01_v_min_spinbtn;
    delete p_s01_v_max_spinbtn;
    delete p_s01_aux_spinbtn;
    delete p_s01_T_ex_spinbtn;

    delete p_s02_kid_spinbtn;
    delete p_s02_active_checkbtn;
    delete p_s02_logtau_spinbtn;
    delete p_s02_v_min_spinbtn;
    delete p_s02_v_max_spinbtn;
    delete p_s02_aux_spinbtn;
    delete p_s02_T_ex_spinbtn;
#endif

    return 0;
}
