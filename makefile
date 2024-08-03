# use g++ compiler
CXX = g++

# C++ version 23, -g is for debugging, -Wall and -Wextra are for compile warning messages
CXXFLAGS = -std=c++23 -g -Wall -Wextra
CXXFLAGS_EXTRA =  -Wno-sign-compare -Wno-maybe-uninitialized
SRLFLAGS = -L/usr/lib -lboost_serialization -lboost_system

DEPENDENCIES_DIR = dependencies
BUILD_DIR = build
EXEC_NAME = main

# Builds all files in the current directory
SRCS = $(wildcard $(DEPENDENCIES_DIR)/*.cpp) train_model.cpp

# Creates a copy of each cpp file and places them in the build directory with the .o extensions
OBJS = $(SRCS:$(DEPENDENCIES_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiles all the object files together into one executable
$(EXEC_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC_NAME) $(OBJS) $(SRLFLAGS)

# Compiles the .o files in the build directory
$(BUILD_DIR)/%.o: $(DEPENDENCIES_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Removes the build directory, all object files, and the executable
clean:
	rm -f $(EXEC_NAME) $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)