CXX = g++
BINDIR = $$HOME/bin
CPU = march
CPU_TYPE = native

sound: sound.cpp sound.hpp fft.cpp fft.hpp
	$(CXX) -O3 -pipe -Wall -$(CPU)=$(CPU_TYPE) -o $(BINDIR)/sound sound.cpp fft.cpp
