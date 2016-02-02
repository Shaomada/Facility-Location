CC=g++ -std=c++11 -g
CFLAGS=-c -Wall -pedantic-errors
LDFLAGS=-Wall -pedantic-errors

SOURCES=main.cpp common.cpp instance.cpp point.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=optimal_facility_location

OBJDIR=_obj
OBJPATHS=$(addprefix $(OBJDIR)/,$(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJPATHS)
	$(CC) $(LDFLAGS) $(OBJPATHS) -o $@

$(OBJDIR)/%.o: %.cpp %.hpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)
	rm -f $(EXECUTABLE)
