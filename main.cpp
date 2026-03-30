#include "draw.h"
int main() {
  int n = 1;
  printf("input the disks' number that you need: ");
  scanf_s(" %d", &n);
  render_all(&n);

  return 0;
}
