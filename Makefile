CC 	= gcc
LD	= ld
DEL	= rm

CFLAG 	= 

res	= inodewatch


edit:$(res)



$(res): %:%.c
	$(CC) $(CFLAG) $< -o $@

.PHONY: clean
clean:
	$(DEL) -f $(res) *.o
