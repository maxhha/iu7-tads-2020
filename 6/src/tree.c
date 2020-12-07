#include "tree.h"

tree_t *create_tree(int x)
{
    tree_t *root = malloc(sizeof(tree_t));

    if (!root)
    {
        LOG_ERROR("нет памяти%s", "");
        return NULL;
    }

    root->left = NULL;
    root->right = NULL;
    root->val = x;

    return root;
}

tree_t *tree_add(tree_t *root, int x)
{
    tree_t *p = root;
    tree_t **target = &root;

    while (p)
    {
        LOG_DEBUG("cmp %d %d", p->val, x);

        if (x < p->val)
        {
            target = &p->left;
            p = p->left;
        }
        else
        {
            target = &p->right;
            p = p->right;
        }
    }

    *target = create_tree(x);

    return root;
}

tree_t *read_to_tree(FILE *f)
{
    int x;
    tree_t *root = NULL;

    while (fscanf(f, "%d", &x) == 1)
    {
        LOG_DEBUG("read %d", x);

        root = tree_add(root, x);
    }

    return root;
}

void print_tree_node(tree_t *root, int side, char **buf, size_t *buf_size)
{
    #define VAL_FORMAT "[" YEL "%d" RESET "]"
    #define SUB_COLORS YEL RESET
    size_t val_len = snprintf(NULL, 0, VAL_FORMAT, root->val);
    size_t buf_len = strlen(*buf);
    size_t min_size = buf_len + val_len + 1;

    if (min_size > *buf_size)
    {
        char *tmp = realloc(*buf, sizeof(char) * min_size);

        if (!tmp)
        {
            LOG_ERROR("нет памяти%s", "");
            return;
        }

        if (*buf_size == 0)
            *tmp = '\0';

        *buf = tmp;
        *buf_size = min_size;
    }

    val_len -= snprintf(NULL, 0, SUB_COLORS);

    int i = buf_len;

    for (; i < buf_len + val_len; i++)
        (*buf)[i] = ' ';

    (*buf)[i] = '\0';

    if (buf_len > 0)
        (*buf)[buf_len - 1] = side > 0 ? '|' : ' ';

    if (root->left)
        print_tree_node(root->left, -1, buf, buf_size);

    if (buf_len > 0)
        (*buf)[buf_len - 1] = '+';

    (*buf)[buf_len] = '\0';
    printf("%s" VAL_FORMAT "\n", *buf, root->val);

    if (buf_len > 0)
        (*buf)[buf_len - 1] = side < 0 ? '|' : ' ';

    i = buf_len;

    for (; i < buf_len + val_len; i++)
        (*buf)[i] = ' ';

    (*buf)[i] = '\0';

    if (root->right)
        print_tree_node(root->right, 1, buf, buf_size);

    (*buf)[buf_len] = '\0';
}

void print_tree(tree_t *root)
{
    char *buf = malloc(sizeof(char));
    size_t buf_size = 1;

    print_tree_node(root, 0, &buf, &buf_size);

    if (buf)
        free(buf);
}
