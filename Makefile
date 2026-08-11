# ==========================================
# COMPILER SETTINGS
# ==========================================
CXX = g++
# Added -Iinclude so g++ can locate headers in the include/ directory
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude

# ==========================================
# FILES AND TARGET
# ==========================================
# Look inside the src/ folder for .cpp files
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Added .exe specifically for Cygwin/Windows compatibility
TARGET = game.exe

# ==========================================
# BUILD RULES
# ==========================================
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete! Type 'make run' to play."

%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- THE MISSING RUN COMMAND ---
run: $(TARGET)
	@echo "Launching game..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	rm -f src/*.o $(TARGET)