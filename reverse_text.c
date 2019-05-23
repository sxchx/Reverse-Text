#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


/*
 *	Maps a text file into memory
 *	Reverses the file in place
 *	Saves the reversed fie
 */
int main (int argc, char *argv[]) 
{
	// check if we have the correct number of args passed
	if(argc != 2)
	{
		printf("Please specify the text file to be reversed\n");
		exit(1);
	}
	// get the file name
	char *textfile = argv[1];
	// this is the file descriptor, file name, read and write abilities
	// and set read and write writes for the owner to true.
	int fd = open(textfile, O_RDWR, S_IRUSR | S_IWUSR);
	// if the file does not exist, return
	if (fd == -1) {
		perror("File does not exist\n");
		exit(1);
	}
	// create a struct to hold file information
	struct stat fileStats;
	// check if an error has occured (if fstat returns -1)
	if (fstat(fd, &fileStats) == -1) 
	{
		// print out an error message
		perror("Cannot get the size of the file. \n");
		close(fd);
		exit(1);
	}

	// get array holding all file contents using mmap 
	// (addr is null, file size, memory protection, fd and offset)
	char *the_file = mmap(NULL, fileStats.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// pointers for start and end of file
	int x = 0;
	int y = fileStats.st_size - 2;
	// reverse the file
	while (x < y) 
	{
		// store value at pos x in temp var
		char tmp = the_file[x];
		// put value at pos y at pos x
		the_file[x] = the_file[y];
		// put value in temp var at pos y
		the_file[y] = tmp;
		// increment x and decrement y
		x++;
		y--;
	}
	//delete this mapping
	munmap(the_file, fileStats.st_size);
	close(fd);
}
