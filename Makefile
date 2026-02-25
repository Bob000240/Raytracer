CXX = g++
CXXFLAGS = -std=c++20 -Iinclude

TARGET = raytracer1b

SRCS = hw1b.Bob.Su.cc \
       src/vec.cc \
       src/color.cc \
       src/intersections.cc \
       src/data.cc \
       src/raytracer.cc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	del /Q $(TARGET).exe 2>NUL || rm -f $(TARGET)