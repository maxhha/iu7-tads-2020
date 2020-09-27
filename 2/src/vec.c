#include "../inc/vec.h"

void * vec__growf(void *arr, int increment, int itemsize)
{
   int dbl_cur = arr ? 2*vec__m(arr) : 0;
   int min_needed = vec_count(arr) + increment;
   int m = dbl_cur > min_needed ? dbl_cur : min_needed;
   int *p = (int *) realloc(arr ? vec__raw(arr) : 0, itemsize * m + sizeof(int)*2);
   if (p) {
      if (!arr)
         p[1] = 0;
      p[0] = m;
      return p+2;
   } else {
      return (void *) (2*sizeof(int)); // try to force a NULL pointer exception later
   }
}

int vec__remove(void *arr, int index, int itemsize)
{
    int i = index < 0 ? vec__n(arr) + index : index;

    if (i < 0 || i >= vec__n(arr))
        return 0;

    char *last = (char *) arr + itemsize * (--vec__n(arr));

    for (char *p = (char *) arr + index * itemsize; p < last; p++)
        *p = *(p + itemsize);

    return 1;
}
