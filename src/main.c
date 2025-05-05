#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "network.h"
#include "user.h"

static bool running = true;

void handle_signal(int signum) {
  printf("\nGot signal %d...\n", signum);
  running = false;
  cleanup_network();
  exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[]) {
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  init_users();
  init_network();

  while (running) {
    printf("RUNNING\n");
    process_network_event(1);
  }

  return EXIT_SUCCESS;
}
