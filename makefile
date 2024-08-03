# use g++ compiler
CXX = g++

# C++ version 23, -g is for debugging, -Wall and -Wextra are for compile warning messages
CXXFLAGS = -std=c++23 -g -Wall -Wextra
CXXFLAGS_EXTRA = -Wno-sign-compare -Wno-maybe-uninitialized
SRLFLAGS = -L/usr/lib -lboost_serialization -lboost_system

DEPENDENCIES_DIR = dependencies
BUILD_DIR = build

# Builds all files in the current directory
SRCS = $(wildcard $(DEPENDENCIES_DIR)/*.cpp)

# Creates a copy of each cpp file and places them in the build directory with the .o extensions
OBJS = $(SRCS:$(DEPENDENCIES_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiles all the object files together into one executable
train_model: $(OBJS) $(BUILD_DIR)/train_model.o
	$(CXX) $(CXXFLAGS) -o train_model $(OBJS) $(BUILD_DIR)/train_model.o $(SRLFLAGS)

$(BUILD_DIR)/train_model.o: train_model.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compiles all the object files together into one executable
user: $(OBJS) $(BUILD_DIR)/user.o
	$(CXX) $(CXXFLAGS) -o user $(OBJS) $(BUILD_DIR)/user.o $(SRLFLAGS)

$(BUILD_DIR)/user.o: user.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compiles the .o files in the build directory
$(BUILD_DIR)/%.o: $(DEPENDENCIES_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Removes the build directory, all object files, and the executable
clean:
	rm -f train_model user $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)
