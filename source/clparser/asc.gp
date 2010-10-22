/* 
   This is the file from which the command line parsing code of ASC is 
   generated by genparse ( http://genparse.sourceforge.net ) 
*/
#description "Advanced Strategic Command: a turn based strategy game"
x / xresolution int -1 [800...] "Set horizontal resolution to <X>"
y / yresolution int -1  [600...] "Set vertical resolution to <Y>"
l / load        string           "Load a map, save game, or email game on startup"
c / configfile  string           "Use given configuration file"
r / verbose     int 0 [0..10]    "Set verbosity level to x (0..10)"
w / window      flag             "Disable fullscreen mode"
f / fullscreen  flag             "Enable fullscreen mode (overriding config file)"
q / nosound     flag             "Disable sound"
NONE / testrun  flag             "tests loading the data, but does not open any windows"
