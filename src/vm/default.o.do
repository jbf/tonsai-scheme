redo-ifchange $2.cpp
g++ -g -MD -MF $2.d -c -o $3 $2.cpp
read DEPS <$2.d
redo-ifchange ${DEPS#*:}
