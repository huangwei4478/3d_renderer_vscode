game:
	g++ main.cpp -o play -I include -L lib -l SDL2-2.0.0; ./play

clean:
	rm play