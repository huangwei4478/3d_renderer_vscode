game:
	gcc -Wall -std=c99 ./src/*.c -o play -I include -L lib -l SDL2-2.0.0; ./play

debug:
	gcc -Wall -std=c99 -g ./src/*.c -o play -I include -L lib -l SDL2-2.0.0; lldb play
clean:
	rm play