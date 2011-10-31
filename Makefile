CXXFLAGS =	-O2 -Wall -fmessage-length=0

INCLUDES =	process.h parsing.h

OBJS =		main.o parsing.o

LIBS =

TARGET =	processtree

$(TARGET):	$(OBJS) $(INCLUDES)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
