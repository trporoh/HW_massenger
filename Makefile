binfirst := server
binsecond := client
binthird := sys5
obj1 := server.o
obj2 := client.o
obj3 := sys5.o

all: $(binfirst) $(binsecond) 

$(binfirst): $(obj1) 
	gcc $< -o $(binfirst) -lpthread -lrt

$(binsecond): $(obj2) 
	gcc $< -o $(binsecond) -lpthread -lrt 

%.o: %.c
	gcc -c $< -o $@ -g

clean:
	rm $(binfirst) $(obj1) $(binsecond) $(obj2)