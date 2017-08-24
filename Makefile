OPENGLLIB	= 	-lGL
GLEWLIB		= 	-lGLEW
GLFWLIB 	= 	-lglfw
LIBS		=	$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS		=	-L/usr/local/lib 
CPPFLAGS	=	-I/usr/local/include

BIN1		=	crafter
SRCS1		=	Main.cpp shader_util.cpp model.cpp crafter.cpp palette.cpp 
INCLUDES	=	shader_util.hpp model.hpp crafter.hpp config.hpp palette.hpp

all: $(BIN1)

$(BIN1): $(SRCS1) $(INCLUDES)
	g++ -std=c++11 $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1) 
