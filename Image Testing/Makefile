main: main.o
	gcc -std=c11 -pedantic -Wall -Wextra -o main main.o -lm

main.o: main.c stb_image.h stb_image_write.h
	gcc -std=c99 -pedantic -Wall -Wextra -c main.c stb_image.h stb_image_write.h -lm

clean:
	rm -f *.o *.gch main