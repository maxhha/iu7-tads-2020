#include "tree.h"

#define IF(cond) if ((++(*(cmprs))) && (cond))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
    root->height = 1;

    return root;
}

void free_tree(tree_t *root)
{
    if (!root)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int copy_tree(tree_t *root, tree_t **new_root)
{
    *new_root = create_tree(root->val);
    if (*new_root == NULL)
        return 1;

    if (root->left != NULL)
        if (copy_tree(root->left, &(*new_root)->left))
        {
            free_tree(*new_root);
            *new_root = NULL;
            return 1;
        }

    if (root->right != NULL)
        if (copy_tree(root->right, &(*new_root)->right))
        {
            free_tree(*new_root);
            *new_root = NULL;
            return 1;
        }

    return 0;
}

tree_t *tree_add(tree_t *root, int x)
{
    if (root == NULL)
        return create_tree(x);

    LOG_DEBUG("cmp %d %d", p->val, x);

    if (x < root->val)
    {
        root->left = tree_add(root->left, x);
        root->height = MAX(1 + root->left->height, root->height);
    }
    else if (x > root->val)
    {
        root->right = tree_add(root->right, x);
        root->height = MAX(1 + root->right->height, root->height);
    }

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

#define VAL_FORMAT "[ %*d ]"

size_t get_max_print_val_len(tree_t *root)
{
    if (root == NULL)
        return 0;

    size_t max = snprintf(NULL, 0, VAL_FORMAT, 0, root->val);
    size_t x = get_max_print_val_len(root->left);

    if (x > max)
        max = x;

    x = get_max_print_val_len(root->right);

    if (x > max)
        max = x;

    return max;
}

void print_tree_node(tree_t *root, size_t max_val_len, int side, char **buf, size_t *buf_size)
{
    size_t val_len = max_val_len;
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

    for (int i = buf_len; i < buf_len + val_len; i++)
        (*buf)[i] = ' ';

    (*buf)[buf_len + val_len] = '\0';

    if (buf_len > 0)
        (*buf)[buf_len - 1] = side > 0 ? '|' : ' ';

    if (root->left)
        print_tree_node(root->left, max_val_len, -1, buf, buf_size);

    if (buf_len > 0)
        (*buf)[buf_len - 1] = '+';

    (*buf)[buf_len] = '\0';

    int add_spaces = (int) max_val_len - snprintf(NULL, 0, VAL_FORMAT, 0, 0) + 1;
    printf("%s" VAL_FORMAT "\n", *buf, add_spaces, root->val);

    if (buf_len > 0)
        (*buf)[buf_len - 1] = side < 0 ? '|' : ' ';

    for (int i = buf_len; i < buf_len + val_len; i++)
        (*buf)[i] = ' ';

    (*buf)[buf_len + val_len] = '\0';

    if (root->right)
        print_tree_node(root->right, max_val_len, 1, buf, buf_size);

    (*buf)[buf_len] = '\0';
}

void print_tree(tree_t *root)
{
    #define DEFAULT_BUF_SIZE 16
    char *buf = malloc(sizeof(char) * DEFAULT_BUF_SIZE);
    size_t buf_size = DEFAULT_BUF_SIZE;
    buf[0] = 0;

    print_tree_node(root, get_max_print_val_len(root), 0, &buf, &buf_size);

    if (buf)
        free(buf);
}

tree_t *tree_into_bamboo(tree_t *root)
{
    if (root == NULL || (root->left == NULL && root->right == NULL))
        return root;

    root->right = tree_into_bamboo(root->right);

    if (root->left == NULL)
        return root;

    tree_t *new_root = tree_into_bamboo(root->left);
    tree_t *p = new_root;

    while (p->right != NULL)
        p = p->right;

    p->right = root;
    root->left = NULL;

    return new_root;
}

tree_t *split_bamboo(tree_t *root)
{
    if (root == NULL || root->right == NULL || root->right->right == NULL)
        return root;

    tree_t *turtle = root, *hare = root->right->right;

    LOG_DEBUG("turtle    hare%s", "");

    while (hare != NULL)
    {
        LOG_DEBUG("%6d  %6d", turtle->val, hare->val);

        hare = hare->right;
        if (hare == NULL)
            break;

        LOG_DEBUG("        %6d", hare->val);
        turtle = turtle->right;
        hare = hare->right;
    }

    LOG_DEBUG("%6d", turtle->val);

    tree_t *new_root = turtle->right;
    turtle->right = NULL;
    new_root->left = root;

    return new_root;
}

int get_height(tree_t *root)
{
    return root == NULL ? 0 : root->height;
}

tree_t *balance_bamboo(tree_t *root)
{
    if (root == NULL)
        return NULL;

    #ifdef DEBUG
    LOG_DEBUG("balancing:%s", "");
    print_tree(root);
    #endif

    root = split_bamboo(root);

    #ifdef DEBUG
    LOG_DEBUG("splitted bamboo:%s", "");
    print_tree(root);
    #endif

    root->left = balance_tree(root->left);
    root->right = balance_tree(root->right);

    root->height = 1 + MAX(get_height(root->left), get_height(root->right));

    return root;
}

tree_t *balance_tree(tree_t *root)
{
    if (root == NULL)
        return NULL;

    #ifdef DEBUG
    LOG_DEBUG("balancing:%s", "");
    print_tree(root);
    #endif

    root = tree_into_bamboo(root);

    #ifdef DEBUG
    LOG_DEBUG("bamboo:%s", "");
    print_tree(root);
    #endif

    root = split_bamboo(root);

    #ifdef DEBUG
    LOG_DEBUG("bamboo:%s", "");
    print_tree(root);
    #endif

    root->left = balance_bamboo(root->left);
    root->right = balance_bamboo(root->right);

    root->height = 1 + MAX(get_height(root->left), get_height(root->right));

    return root;
}

tree_t *find_tree_val(tree_t *root, int val)
{
    while (root)
    {
        if (root->val == val)
            return root;
        else if (val < root->val)
            root = root->left;
        else
            root = root->right;
    }

    return NULL;
}

tree_t *get_min_value_element(tree_t *root)
{
    tree_t *p = root;
    while (p && p->left != NULL)
       p = p->left;
    return p;
}

int get_tree_balance(tree_t *root)
{
    if (root == NULL)
        return 0;
    return get_height(root->left) - get_height(root->right);
}

tree_t *delete_element_from_tree(tree_t* root, int val, int *cmprs)
{
    IF (root == NULL)
        return root;

    IF (val < root->val)
        root->left = delete_element_from_tree(root->left, val, cmprs);
    else IF (val > root->val)
        root->right = delete_element_from_tree(root->right, val, cmprs);
    else
    {
        IF (root->left == NULL)
        {
            tree_t *temp = root->right;
            free(root);
            return temp;
        }
        else IF (root->right == NULL)
        {
             tree_t *temp = root->left;
             free(root);
             return temp;
        }
        tree_t *temp = get_min_value_element(root->right);
        root->val = temp->val;
        root->right = delete_element_from_tree(root->right, temp->val, cmprs);
    }

    root->height = 1 + MAX(get_height(root->left), get_height(root->right));

    return root;
}

tree_t *rotate_tree_right(tree_t *root)
{
    tree_t *p = root->left;
    tree_t *t = p->right;

    p->right = root;
    root->left = t;

    root->height = MAX(get_height(root->left), get_height(root->right)) + 1;
    p->height = MAX(get_height(p->left), get_height(p->right)) + 1;

    return p;
}

tree_t *rotate_tree_left(tree_t *root)
{
    tree_t *p = root->right;
    tree_t *t = p->left;

    p->left = root;
    root->right = t;

    root->height = MAX(get_height(root->left), get_height(root->right)) + 1;
    p->height = MAX(get_height(p->left), get_height(p->right)) + 1;

    return p;
}

tree_t *delete_element_from_balanced_tree(tree_t* root, int val, int *cmprs)
{
    IF (root == NULL)
        return root;

    IF (val < root->val)
        root->left = delete_element_from_balanced_tree(root->left, val, cmprs);
    else IF(val > root->val)
        root->right = delete_element_from_balanced_tree(root->right, val, cmprs);
    else
    {
        IF(root->left == NULL || root->right == NULL)
        {
            tree_t *temp = root->left ?
                root->left :
                root->right;

            IF (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            tree_t* temp = get_min_value_element(root->right);

            root->val = temp->val;

            root->right = delete_element_from_balanced_tree(root->right, temp->val, cmprs);
        }
    }

    IF (root == NULL)
        return root;

    root->height = 1 + MAX(get_height(root->left), get_height(root->right));

    int balance = get_tree_balance(root);

    IF (balance > 1 && get_tree_balance(root->left) >= 0)
        return rotate_tree_right(root);

    IF (balance > 1 && get_tree_balance(root->left) < 0)
    {
        root->left = rotate_tree_left(root->left);
        return rotate_tree_right(root);
    }

    IF (balance < -1 && get_tree_balance(root->right) <= 0)
        return rotate_tree_left(root);

    IF (balance < -1 && get_tree_balance(root->right) > 0)
    {
        root->right = rotate_tree_right(root->right);
        return rotate_tree_left(root);
    }

    return root;
}
