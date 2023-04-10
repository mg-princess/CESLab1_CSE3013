cc=g++
target = main

objects = main.o array.o rangearray.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.cpp
	$(cc) -c -o $@ $<

main.o array.o : Array.h
main.o rangearray.o : RangeArray.h

.PHONY : clean
clean :
	rm $(target) $(objects)
