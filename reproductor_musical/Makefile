CXX = g++
CXXFLAGS = -Wall -std=c++17

all: abc2midi_pdf

abc2midi_pdf: script.cpp
	$(CXX) $(CXXFLAGS) script.cpp -o abc2midi_pdf

reproduction: abc2midi_pdf

clean:
	rm -f *.midi *.pdf *.ps *.mid *.abc abc2midi_pdf output.ogg

.PHONY: all reproduction clean 