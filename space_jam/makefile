CXX = g++
CXXFLAGS = -std=c++0x

output: main.o Space.o filmRoom.o teamGym.o courtsideSeats.o Player.o menu.o Game.o
	$(CXX) $(CXXFLAGS) main.o Space.o filmRoom.o teamGym.o courtsideSeats.o Player.o menu.o Game.o -o output

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Space.o: Space.cpp Space.hpp 
	$(CXX) $(CXXFLAGS) -c Space.cpp

filmRoom.o: filmRoom.cpp filmRoom.hpp 
	$(CXX) $(CXXFLAGS) -c filmRoom.cpp

teamGym.o: teamGym.cpp teamGym.hpp 
	$(CXX) $(CXXFLAGS) -c teamGym.cpp

courtsideSeats.o: courtsideSeats.cpp courtsideSeats.hpp 
	$(CXX) $(CXXFLAGS) -c courtsideSeats.cpp

Player.o: Player.cpp Player.hpp 
	$(CXX) $(CXXFLAGS) -c Player.cpp

Game.o: Game.cpp Game.hpp 
	$(CXX) $(CXXFLAGS) -c Game.cpp

menu.o: menu.cpp menu.hpp
	$(CXX) $(CXXFLAGS) -c menu.cpp

clean:
	rm *.o output
