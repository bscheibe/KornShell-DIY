#include "get_path.h"

main()
{
  char cmd[64];
  struct pathelement *p;
  setenv("TEST", "", 1);
  p = get_path();
  while (p) {
  //  sprintf(cmd, "%s/gcc", p->element);
//    if (access(cmd, F_OK) == 0)
      printf("[%s]\n", p->element);
    p = p->next;
  }

  printf("----------\n");

  p = get_path();
  while (p) {
    sprintf(cmd, "%s/gcc", p->element);
    if (access(cmd, F_OK) == 0) {
      printf("[%s]\n", cmd);
      break;
    }
    p = p->next;
  }
}
