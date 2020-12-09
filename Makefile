CC = gcc
CFLAGS = 	-Wall
OBJECTS = 	main
ALLFILE = 	main.o \
			poker.o

all : $(OBJECTS)

$(OBJECTS): $(ALLFILE)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	@-rm *.o $(OBJECTS)



#
#
#编译所有文件方式 %.o: %.c $(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)
#编译单个文件写法 main.o:main.c  $(CC) $(CFLAGS)  -c $<
#
# -ldl -lpthread