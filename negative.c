#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int lireligne(int fd, char *buffer, int size) {
	ssize_t nbread = read(fd, buffer, size);
	if (nbread == -1) {
		return -1;
	}

	int i;
	for (i = 0; i < nbread; i++) {
		if (buffer[i] == '\n') {
			i++;
			break;
		}
	}
	lseek(fd, i - nbread, SEEK_CUR);
	return i;
}


int main(int argc, char **argv) {
    // descripteur de fichier du fichier ouvert en lecture
    int fd_in = open("test.pgm",O_RDONLY);
    if(fd_in < 0){
      perror(argv[1]);
      exit(1);
    }
    // descripteur de fichier du fichier ouvert en écriture
    int fd_out = open("reverse.pgm",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int nbread;
    // buffer de lecture
    unsigned char *buffer = malloc(4096 * sizeof(unsigned char));    
    //Lecture de l'en tête
    for(int i = 0;i < 3; i++){
      nbread = lireligne(fd_in,buffer,4096);
      printf(" nombre d'octet copié : %i \n",nbread);
      write(fd_out,buffer,nbread);
    }
    //Lecture de chaque pixel
    nbread = 1;
    while(nbread != 0){
      nbread = read(fd_in,buffer,4096);
      for(int i = 0; i < nbread ;i++){
        buffer[i] = 255 - buffer[i];
      }
      write(fd_out,buffer,nbread);
    }
    




    close(fd_in);
    close(fd_out);
    return 0;
}