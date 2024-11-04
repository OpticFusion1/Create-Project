CC = g++
CFLAGS = -std=c++17 -Wall -O3 -s
LDFLAGS = -s
SRCDIR = src/code

BUILDDIR = build
TARGET = create-project.exe

SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*.o $(TARGET)
