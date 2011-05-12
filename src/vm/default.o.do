redo-ifchange $1.cpp
g++ -g -MD -MF $1.d -c -o $3 $1.cpp
read DEPS <$1.d
redo-ifchange ${DEPS#*:}
