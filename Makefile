CC=g++
AR=ar
RANLIB=ranlib

GL_INC_DIR=/usr/include/GL
GL_LIB_DIR=/usr/lib/
OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
GLUTLIB = -lglut -lGLU
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB) $(GLUTLIB) -lm

BIN=BVHPlayer
OBJ1=error.o matvec.o
OBJ2=bvh_tokens.o bvh_joints.o bvh_hierarchy.o bvh.o SOIL/SOIL.o SOIL/image_DXT.o SOIL/image_helper.o SOIL/stb_image_aug.o
OBJ3=main.o gl_interface.o body_draw.o basics.o gl_framework.o environment.o
OBJ=$(OBJ1) $(OBJ2) $(OBJ3)
LIB1=libUtil.a
LIB2=libBVH.a

CFLAGS=-c -g -D_DEBUG -Wall -Woverloaded-virtual -Winline

#CFLAGS=-c -O -I$(GL_INC_DIR) -L$(GL_LIB_DIR) -Wall

LDFLAGS=-L/usr/local/lib 

.SUFFIXES:.cpp .o .hpp

all: main

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $*.cpp

$(LIB1): $(OBJ1)
	$(AR) rcs $@ $(OBJ1)

$(LIB2): $(OBJ2)
	$(AR) rcs $@ $(OBJ2)

main: $(OBJ3) $(LIB2) $(LIB1)
	$(CC) $(OBJ3) -L./ $(LIB2) $(LIB1) $(LDFLAGS) $(LIBS) -o $(BIN)

docs: doc/Doxyfile
	cd doc; doxygen Doxyfile

clean:
	rm -f $(OBJ) core *~

cleandocs:
	cd doc; rm -rf html

distclean: clean
	rm -f $(BIN) $(LIB1) $(LIB2)

new: clean main	


# DO NOT DELETE

bvh.o: bvh.hpp bvh_hierarchy.hpp bvh_joints.hpp bvh_tokens.hpp matvec.hpp error.hpp
bvh_hierarchy.o: bvh_hierarchy.hpp bvh_joints.hpp bvh_tokens.hpp matvec.hpp error.hpp
bvh_joints.o: bvh_joints.hpp bvh_tokens.hpp matvec.hpp error.hpp
bvh_tokens.o: bvh_tokens.hpp
error.o: error.hpp
main.o: bvh_tokens.hpp bvh.hpp gl_interface.o
matvec.o: matvec.hpp error.hpp
gl_interface.o: gl_interface.hpp
basics.o: basics.hpp
body_draw.o: body.hpp basics.hpp
gl_framework.o: gl_framework.hpp
