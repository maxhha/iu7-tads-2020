#include "../inc/main.h"

int main(void) {

    int rc;

    rc = action_mul_smatrix();

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
