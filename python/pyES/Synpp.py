
import Common

class Output( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Output( **params )
        else :
            return Output( 2500.0, 10000.0, 5.0 )

    def __init__( self, min_wl, max_wl, wl_step ) :
        self.min_wl  = min_wl
        self.max_wl  = max_wl
        self.wl_step = wl_step

    def __repr__( self ) :
        output =  "output :\n"
        for attr in "min_wl max_wl wl_step".split() :
            output += "    %-12s : %s\n" % ( attr, getattr( self, attr ) )
        return output.rstrip()

class Ion( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Ion( **params )                
        else :
            return Ion( None, None, None, None, None, None, None )

    def __init__( self, ion, active, log_tau, v_min, v_max, aux, temp ) :
        self.ion     = ion
        self.active  = active
        self.log_tau = log_tau
        self.v_min   = v_min
        self.v_max   = v_max
        self.aux     = aux
        self.temp    = temp

class Setup( object ) :

    @classmethod
    def create( c, params = None ) :
        if not params :
            return Setup( 1.0, 0.0, 0.0, 10.0, 30.0, 10.0, [] )
        ions = []
        for ion, active, log_tau, v_min, v_max, aux, temp in zip( *[ params[ attr ] for attr in "ions active log_tau v_min v_max aux temp".split() ] ) :
            ions.append( Ion( ion, active, log_tau, v_min, v_max, aux, temp ) )
        for attr in "ions active log_tau v_min v_max aux temp".split() :
            del params[ attr ]
        params[ "ions" ] = ions
        return Setup( **params )

    @classmethod
    def create_list( c, params = None ) :
        if params :
            return [ Setup.create( entry ) for entry in params ]
        else :
            return [ Setup.create() ]

    def __init__( self, a0, a1, a2, v_phot, v_outer, t_phot, ions ) :
        self.a0      = a0
        self.a1      = a1
        self.a2      = a2
        self.v_phot  = v_phot
        self.v_outer = v_outer
        self.t_phot  = t_phot
        self.ions    = ions

    def __repr__( self ) :
        output =  ""
        for attr in "a0 a1 a2 v_phot v_outer t_phot".split() :
            output += "    "
            output += "-   " if attr == "a0" else "    "
            output += "%-8s : %-6.2f\n" % ( attr, getattr( self, attr ) ) 
        for attr in "ion active log_tau v_min v_max aux temp".split() :
            output += "        %-8s : [" % ( "ions" if attr == "ion" else attr )
            for ion in self.ions :
                value = getattr( ion, attr )
                if type( value ) is float :
                    output += " %6.2f," % value
                elif type( value ) is bool :
                    output += " %6s," % ( "Yes" if value else "No" )
                else :
                    output += " %6s," % value
            output = output.rstrip( "," )
            output += " ]\n"
        return output.rstrip()

class Synpp( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            output   = Output.create         ( params[ "output"   ] )
            grid     = Common.Grid.create    ( params[ "grid"     ] )
            opacity  = Common.Opacity.create ( params[ "opacity"  ] )
            source   = Common.Source.create  ( params[ "source"   ] )
            spectrum = Common.Spectrum.create( params[ "spectrum" ] )
            setups   = Setup.create_list ( params[ "setups"   ] )
            return Synpp( output, grid, opacity, source, spectrum, setups )
        else :
            output   = Output.create         ()
            grid     = Common.Grid.create    ()
            opacity  = Common.Opacity.create ()
            source   = Common.Source.create  ()
            spectrum = Common.Spectrum.create()
            setups   = Setup.create_list ()
            return Synpp( output, grid, opacity, source, spectrum, setups )

    def __init__( self, output, grid, opacity, source, spectrum, setups ) :
        self.output   = output
        self.grid     = grid
        self.opacity  = opacity
        self.source   = source
        self.spectrum = spectrum
        self.setups   = setups

    def __repr__( self ) :
        output = ""
        for attr in "output grid opacity source spectrum".split() :
            output += "%s\n" % getattr( self, attr )
        output += "setups :\n"
        for setup in self.setups :
            output += "%s\n" % setup
        return output.rstrip()

if __name__ == "__main__" :

    import sys
    import yaml

    if sys.argv[ 1 : ] :
        print Synpp.create( yaml.load( open( sys.argv[ 1 ], "r" ) ) )
    else :
        print Synpp.create()
