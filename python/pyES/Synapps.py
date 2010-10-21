
import Common

class Region( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Region( **params )
        else :
            return Region( None, None, None, None )

    def __init__( self, apply, weight, lower, upper ) :
        self.apply  = apply
        self.weight = weight
        self.lower  = lower
        self.upper  = upper

class Evaluator( object ) :

    @classmethod
    def create( c, params = None ) :
        if not params :
            return Evaluator( None, 2, [] )
        regions = []
        for apply, weight, lower, upper in zip( *[ params[ "regions" ][ attr ] for attr in "apply weight lower upper".split() ] ) :
            regions.append( Region( apply, weight, lower, upper ) )
        params[ "regions" ] = regions
        return Evaluator( **params )

    def __init__( self, target_file, vector_norm, regions ) :
        self.target_file = target_file
        self.vector_norm = vector_norm
        self.regions     = regions

    def __repr__( self ) :
        output =  "evaluator :\n"
        output += "    %-12s : %s\n" % ( "target_file", self.target_file )
        output += "    %-12s : %s\n" % ( "vector_norm", self.vector_norm )
        output += "    %-12s :\n" % "regions"
        for attr in "apply weight lower upper".split() :
            output += "        %-8s : [ " % attr
            for region in self.regions :
                value = getattr( region, attr )
                if type( value ) is bool :
                    output += "%8s," % ( "Yes" if value else "No" )
                else :
                    output += "%8.2f," % value
            output = output.rstrip( "," )                
            output += " ]\n"
        return output.rstrip()

class Variable( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Variable( **params )
        else :
            return Variable( False, 1.0, 0.0, 10.0, 1.0 )

    def __init__( self, fixed, start, lower, upper, scale ) :
        self.fixed = fixed
        self.start = start
        self.lower = lower
        self.upper = upper
        self.scale = scale

class Ion( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Ion( **params )
        else :
            return Ion( None, None, None, Variable.create(), Variable.create(), Variable.create(), Variable.create(), Variable.create() )

    def __init__( self, ion, active, detach, log_tau, v_min, v_max, aux, temp ) :
        self.ion     = ion
        self.active  = active
        self.detach  = detach
        self.log_tau = log_tau
        self.v_min   = v_min
        self.v_max   = v_max
        self.aux     = aux
        self.temp    = temp

class Config( object ) :

    @classmethod
    def create( c, params = None ) :
        if not params :
            return Config( None, None, 
                    Variable.create(), Variable.create(), Variable.create(), Variable.create(), Variable.create(), Variable.create(), [] )
        for var_name in "a0 a1 a2 v_phot v_outer t_phot".split() :
            params[ var_name ] = Variable.create( params[ var_name ] )
        ions = []
        for i, ion in enumerate( params[ "ions" ] ) :
            ion_params = { "ion" : ion, "active" : params[ "active" ][ i ], "detach" : params[ "detach" ][ i ] }
            for var_name in "log_tau v_min v_max aux temp".split() :
                ion_params[ var_name ] = Variable( *[ params[ var_name ][ attr ][ i ] for attr in "fixed start lower upper scale".split() ] )
            ions.append( Ion.create( ion_params ) )
        for var_name in "ions active detach log_tau v_min v_max aux temp".split() :
            del params[ var_name ]
        params[ "ions" ] = ions
        return Config( **params )
    
    def __init__( self, fit_file, cache_file, a0, a1, a2, v_phot, v_outer, t_phot, ions ) :
        self.fit_file   = fit_file
        self.cache_file = cache_file
        self.a0         = a0
        self.a1         = a1
        self.a2         = a2
        self.v_phot     = v_phot
        self.v_outer    = v_outer
        self.t_phot     = t_phot
        self.ions       = ions

    def __repr__( self ) :
        output =  "config :\n"
        output += "    %-12s : %s\n" % ( "fit_file"  , self.fit_file   )
        output += "    %-12s : %s\n" % ( "cache_file", self.cache_file )
        output += "\n"
        for var_name in "a0 a1 a2 v_phot v_outer t_phot".split() :
            output += "    %-12s : {" % var_name
            for attr in "fixed start lower upper scale".split() :
                value = getattr( getattr( self, var_name ), attr )
                if type( value ) is bool :
                    output += " %5s: %3s," % ( attr, ( "Yes" if value else "No" ) )
                else :
                    output += " %5s: %6.2f," % ( attr, value )
            output = output.rstrip( "," )
            output += " }\n"
        output += "\n"
        for var_name in "ion active detach".split() :
            output += "    %-12s : [ " % ( "ions" if var_name == "ion" else var_name )
            for value in [ getattr( ion, var_name ) for ion in self.ions ] :
                if type( value ) is bool :
                    output += " %6s," % ( "Yes" if value else "No" )
                else :
                    output += " %6s," % value
            output = output.rstrip( "," )
            output += " ]\n"
        output += "\n"
        for var_name in "log_tau v_min v_max aux temp".split() :
            output += "    %-12s :\n" % var_name
            for attr in "fixed start lower upper scale".split() :
                output += "        %-8s : [ " % attr
                for value in [ getattr( getattr( ion, var_name ), attr ) for ion in self.ions ] :
                    if type( value ) is bool :
                        output += " %6s," % ( "Yes" if value else "No" )
                    else:
                        output += " %6.2f," % value
                output = output.rstrip( "," )
                output += " ]\n"
        return output.rstrip()

class Synapps( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            grid      = Common.Grid.create    ( params[ "grid"      ] )
            opacity   = Common.Opacity.create ( params[ "opacity"   ] )
            source    = Common.Source.create  ( params[ "source"    ] )
            spectrum  = Common.Spectrum.create( params[ "spectrum"  ] )
            evaluator = Evaluator.create      ( params[ "evaluator" ] )
            config    = Config.create         ( params[ "config"    ] )
            return Synapps( grid, opacity, source, spectrum, evaluator, config )
        else :
            grid      = Common.Grid.create    ()
            opacity   = Common.Opacity.create ()
            source    = Common.Source.create  ()
            spectrum  = Common.Spectrum.create()
            evaluator = Evaluator.create      ()
            config    = Config.create         ()
            return Synapps( grid, opacity, source, spectrum, evaluator, config )

    def __init__( self, grid, opacity, source, spectrum, evaluator, config ) :
        self.grid      = grid
        self.opacity   = opacity
        self.source    = source
        self.spectrum  = spectrum
        self.evaluator = evaluator
        self.config    = config

    def __repr__( self ) :
        output = ""
        for attr in "grid opacity source spectrum evaluator config".split() :
            output += "%s\n" % getattr( self, attr )
        return output.rstrip()

if __name__ == "__main__" :

    import sys
    import yaml

    if sys.argv[ 1 : ] :
        print Synapps.create( yaml.load( open( sys.argv[ 1 ], "r" ) ) )
    else :
        print Synapps.create()
