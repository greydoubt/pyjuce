JUCE_ROOT := /path/to/juce

TARGET := MyPlugin
BUILD_DIR := Build

CXX := clang++
CXXFLAGS := -Wall -O2 -std=c++17
LDFLAGS := -L$(JUCE_ROOT)/bin -ljuce_audio_plugin_client

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(TARGET).vst3

$(BUILD_DIR)/$(TARGET).vst3: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(JUCE_ROOT)/modules -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
