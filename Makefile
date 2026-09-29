CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

planificador: planificador.cpp
	$(CXX) $(CXXFLAGS) -o planificador planificador.cpp

clean:
	rm -f planificador
