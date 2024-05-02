#RedId:825730146

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Output executable name
PROGRAM = countsubstrings

# Source file
SOURCE = countSubstrings.cpp

# Rules to build the executable
$(PROGRAM): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(SOURCE)

# Clean target to remove the executable
clean:
	rm -f $(PROGRAM)