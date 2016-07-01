CC=g++ -std=c++11 -g
CXX=g++ -std=c++11 -c -g
CFLAGS=-Wall -Wpedantic -Werror
LDFLAGS=-Wall -Wpedantic -Werror

SOURCES=main.cpp solver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=optimal_facility_location

OBJDIR=_obj
OBJPATHS=$(addprefix $(OBJDIR)/,$(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJPATHS)
	$(CC) $(LDFLAGS) $(OBJPATHS) -o $@

$(OBJDIR)/%.o: %.cpp *.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)
	rm -f $(EXECUTABLE)
