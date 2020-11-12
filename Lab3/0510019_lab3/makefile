CXX		=	g++ -std=c++11 -O3
EXE		=  lab3 
OBJ_DIR	=	objs

CFLAGS = -g -O3

all: $(EXE)

$(EXE): $(OBJ_DIR)/main.o $(OBJ_DIR)/fp.o $(OBJ_DIR)/btree.o
	$(CXX) $^ $(CFLAGS) -o $@
	
$(OBJ_DIR)/main.o: main.cpp btree.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/btree.o: btree.cpp btree.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/fp.o: fp.cpp fp.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf *.gch objs/
