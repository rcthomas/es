
from distutils.core import setup

setup(  name            = 'pyES',
        version         = '0.10',
        description     = 'ES Python Utilities',
        author          = 'R. C. Thomas',
        author_email    = 'rcthomas@lbl.gov',
        url             = 'http://c3.lbl.gov/es',
        packages        = [ 'pyES' ],
        scripts         = [ 'create_es_yaml', 'random_es_yaml', 'es_diff' ],
     )
