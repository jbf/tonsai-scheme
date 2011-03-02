DEPS="cpp_vm.o test/test_context.o"
redo-ifchange $DEPS
g++ -o ../../test/test_context $DEPS