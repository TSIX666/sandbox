/* client.c             */

/*
 * Connects to the local host at port 1234
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#define NSTRS 3  /* no. of strings */

/*
 * Strings we send to the server.
 */

extern int errno;

char message[] = "Line 1\nLine 2\n";
char pokemsg[] = "POKE\n";

int main() {

  char                c;
  FILE*               fp;
  char                hostname[64];
  int                 ii, s;
  struct hostent*     hp;
  struct sockaddr_in  sin;
  char                buff[1000];

  gethostname(hostname, sizeof(hostname));

  if ( (hp = gethostbyname(hostname)) == NULL ) {
    fprintf(stderr, "%s: unknown host.\n", hostname);
    exit(1);
  }
  
  if ( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("client:  socket");
    exit(1);
  }
  
  sin.sin_family = AF_INET;
  sin.sin_port = htons(1234);
  bcopy(hp->h_addr, &sin.sin_addr, hp->h_length);

  if ( connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0 ) {
    perror("client:  connect");
    exit(1);
  }

  /* **************************************** */

  ii = recv( s, buff, sizeof(buff), 0 );

  if ( ii > -1 ) { 
      printf("Received %d bytes.\n", ii);
  } else {
      printf("Error, didn't receive any data.\n");
  }


  
  shutdown(s, 2);
  exit(0);
}


