CCX=g++

CXXFLAGS= -g

EXEC= cbc
OBJS = cbc.cpp
all: 
	@echo "Compilando el programa..."
	$(CCX) $(CXXFLAGS) $(OBJS) -o $(EXEC) 

.PHONY: clean
clean:
	@echo "Borrando el ejecutable..."
	rm -r $(EXEC)
