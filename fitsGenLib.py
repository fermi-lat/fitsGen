#$Id: fitsGenLib.py,v 1.2 2008/02/26 03:18:10 glastrm Exp $
def generate(env, **kw):
    if not kw.get('depsOnly',0):
        env.Tool('addLibrary', library = ['fitsGen'])
    env.Tool('facilitiesLib')
    env.Tool('tipLib')
    env.Tool('astroLib')
    env.Tool('st_facilitiesLib')
    env.Tool('dataSubselectorLib')
    env.Tool('st_appLib')
    env.Tool('st_streamLib')
    env.Tool('embed_pythonLib')
    env.Tool('evtUtilsLib')

def exists(env):
    return 1
