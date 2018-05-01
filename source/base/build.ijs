
S=: jpath '~Addons/stats/jserver4r/source/'
PA=: jpath '~Addons/stats/jserver4r/'
Pa=: jpath '~addons/stats/jserver4r/'

mkdir_j_ PA,'lib'
mkdir_j_ Pa,'lib'

f=: 3 : 0
(PA,y) fcopynew S,y
(Pa,y) fcopynew S,y
)

f each 'lib/libjserver4r.'&, each ;: 'dll dylib so'
f 'lib/libjserver4r-raspi64.so'
f 'lib/libs.txt'

f=: 3 : 0
(PA,y) fcopynew S,'base/',y
(Pa,y) fcopynew S,'base/',y
)

f each cutopen 0 : 0
manifest.ijs
readme.txt
jserver.R
jserverdemo.R
jservertest.R
)
