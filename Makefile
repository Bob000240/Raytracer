CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -pthread

TARGET = raytracer

SRCS = main.cc \
       src/vec.cc \
       src/color.cc \
       src/intersections.cc \
       src/camera.cc \
       src/scene.cc \
       src/scene_loader.cc \
       src/renderer.cc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

lint:
	clang-tidy $(SRCS) -- $(CXXFLAGS)

clean:
	rm -f $(TARGET)
