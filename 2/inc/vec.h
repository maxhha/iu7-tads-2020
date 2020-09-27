#ifndef __VEC_H__
#define __VEC_H__

#define vec_free(a)         ((a) ? free(vec__raw(a)),0 : 0)
#define vec_push(a,v)       (vec__maybegrow(a,1), (a)[vec__n(a)++] = (v))
#define vec_remove(a,i)     ((a) ? vec__remove((a), (i), sizeof(*(a))) : 0)
#define vec_count(a)        ((a) ? vec__n(a) : 0)
#define vec_add(a,n)        (vec__maybegrow(a,n), vec__n(a)+=(n), &(a)[vec__n(a)-(n)])
#define vec_last(a)         ((a)[vec__n(a)-1])

#define vec__raw(a) ((int *) (void *) (a) - 2)
#define vec__m(a)   vec__raw(a)[0]
#define vec__n(a)   vec__raw(a)[1]

#define vec__needgrow(a,n)  ((a)==0 || vec__n(a)+(n) >= vec__m(a))
#define vec__maybegrow(a,n) (vec__needgrow(a,(n)) ? vec__grow(a,n) : 0)
#define vec__grow(a,n)      (*((void **)&(a)) = vec__growf((a), (n), sizeof(*(a))))

#include <stdlib.h>

void * vec__growf(void *arr, int increment, int itemsize);
int vec__remove(void *arr, int index, int itemsize);

#endif // __VEC_H__
