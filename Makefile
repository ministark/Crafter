OPENGLLIB	= 	-lGL
GLEWLIB		= 	-lGLEW
GLFWLIB 	= 	-lglfw
LIBS		=	$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS		=	-L/usr/local/lib 
CPPFLAGS	=	-I/usr/local/include

BIN1		=	crafter
SRCS1		=	Main.cpp gl_framework.cpp shader_util.cpp crafter.cpp 
INCLUDES	=	gl_framework.hpp shader_util.hpp crafter.hpp

all: $(BIN1)

$(BIN1): $(SRCS1) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1) 
