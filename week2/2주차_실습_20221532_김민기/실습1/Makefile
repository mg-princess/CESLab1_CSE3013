cc = gcc
target = animal.exe
objects = main.o dog.o blackcow.o turtle.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

.PHONY : clean
clean :
	rm *.o animal.exe