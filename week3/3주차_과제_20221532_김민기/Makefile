cc=g++
target = main

objects = main.o Str.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.cpp
	$(cc) -c -o $@ $<

main.o Str.o : Str.hpp

.PHONY : clean
clean :
	rm $(target) $(objects)
