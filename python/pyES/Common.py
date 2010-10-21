
class Grid( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Grid( **params )
        else :
            return Grid( 0.3, 100, 30.0 )

    def __init__( self, bin_width, v_size, v_outer_max ) :
        self.bin_width   = bin_width
        self.v_size      = v_size
        self.v_outer_max = v_outer_max

    def __repr__( self ) :
        output = "grid :\n"
        for attr in "bin_width v_size v_outer_max".split() :
            output += "    %-12s : %s\n" % ( attr, getattr( self, attr ) )
        return output.rstrip()

class Opacity( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Opacity( **params )
        else :
            path = "/project/projectdirs/snfactry/rthomas/local/share/es/"
            return Opacity( path + "lines", path + "refs.dat", "exp", 10.0, -2.0 )            
    
    def __init__( self, line_dir, ref_file, form, v_ref, log_tau_min ) :
        self.line_dir    = line_dir
        self.ref_file    = ref_file
        self.form        = form
        self.v_ref       = v_ref
        self.log_tau_min = log_tau_min

    def __repr__( self ) :
        output = "opacity :\n"
        for attr in "line_dir ref_file form v_ref log_tau_min".split() :
            output += "    %-12s : %s\n" % ( attr, getattr( self, attr ) )
        return output.rstrip()

class Source( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Source( **params )
        else :
            return Source( 10 )

    def __init__( self, mu_size ) :
        self.mu_size = mu_size

    def __repr__( self ) :
        output = "source :\n"
        output += "    %-12s : %s\n" % ( "mu_size", self.mu_size )
        return output.rstrip()

class Spectrum( object ) :

    @classmethod
    def create( c, params = None ) :
        if params :
            return Spectrum( **params )
        else :
            return Spectrum( 60, False )

    def __init__( self, p_size, flatten ) :
        self.p_size  = p_size
        self.flatten = flatten

    def __repr__( self ) :
        output =  "spectrum :\n"
        output += "    %-12s : %s\n" % ( "p_size" , self.p_size )
        output += "    %-12s : %s\n" % ( "flatten", ( "Yes" if self.flatten else "No" ) )
        return output.rstrip()

if __name__ == "__main__" :
    pass
