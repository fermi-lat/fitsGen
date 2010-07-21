#$Id: fitsGenLib.py,v 1.3 2010/06/16 22:43:42 jchiang Exp $
def generate(env, **kw):
    if not kw.get('depsOnly',0):
        env.Tool('addLibrary', library = ['fitsGen'])
    env.Tool('facilitiesLib')
    env.Tool('tipLib')
    env.Tool('astroLib')
    env.Tool('embed_pythonLib')
    env.Tool('evtUtilsLib')
    env.Tool('addLibrary', library = env['rootGuiLibs'])

def exists(env):
    return 1
