/*
 *Author: ZhangHaiba
 *Date: 2014-2-1
 *File: btree_rotate.c
 *
 *a demo shows how to rotate binary tree while repair link relationship
 *and parent, size information.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 1024
#define CMD_LEN 128

typedef struct btree * btlink;

typedef struct btree
{
    int num;
    btlink par;
    btlink left;
    btlink right;
    int size;
}btree;

//public
btlink NODE(int num, btlink par, btlink left, btlink right, int size);
btlink bt_create(int root_num, btlink par);
btlink bt_left_rotate(btlink root);
btlink bt_right_rotate(btlink root);
void bt_show_by_tree(btlink root);
btlink* bt_search(btlink root, int num);
//private
void tree_print(btlink root, FILE* fd);

//private for bt_create
int left_num[N]; //the number of left tree root
int right_num[N]; //the number of right tree root
btlink num_map_root[N];

int main(void)
{
    
    int n, i;
    printf("input--->n\n");
    scanf("%d", &n);
    for (i = 1; i <= n; ++i)
        scanf("%d%d", left_num+i, right_num+i);
    printf("input--->end\n");
    btlink sentinel = NODE(0, sentinel, NULL, NULL, -1);
    btlink btree_a = bt_create(1, sentinel);
    sentinel->left = sentinel->right = btree_a;
    bt_show_by_tree(btree_a);
    
    int op_times;
    char cmd[CMD_LEN];
    int root_num;
    printf("input--->op_times\n");
    scanf("%d", &op_times);
    while (op_times--) {
        printf("input--->cmd\n");
        scanf("%s", cmd);
        if (strcmp(cmd, "size") == 0) {
            printf("input-size-->root_num\n");
            scanf("%d", &root_num);
            printf("%d\n", num_map_root[root_num]->size);
        } else if (strcmp(cmd, "rotate") == 0) {
            printf("input-rotate-->root_num\n");
            scanf("%d", &root_num);
            btlink x = num_map_root[root_num];
            btlink par = x->par;
            if (par == sentinel) //x is root
                continue;
            if (par->right == x) {
                if (par->par->left == par) //left first
                    par->par->left = bt_left_rotate(par);
                else if (par->par->right == par)
                    par->par->right = bt_left_rotate(par);
            } else if (par->left == x) {
                if (par->par->left == par) //left first
                    par->par->left = bt_right_rotate(par);
                else if (par->par->right == par)
                    par->par->right = bt_right_rotate(par);
            }
            bt_show_by_tree(sentinel->left); //becuase left is first choice
        } else if (strcmp(cmd, "parent") == 0) {
            printf("input-parent-->root_num\n");
            scanf("%d", &root_num);
            btlink root = num_map_root[root_num];
            printf("%d\n", root->par == sentinel ? -1 : root->par->num);
        } else if (strcmp(cmd, "balance") == 0) {
            printf("input-balance-->root_num\n");
            scanf("%d", &root_num);
            btlink root = num_map_root[root_num];
            int left_size = root->left != NULL ? root->left->size : 0;
            int right_size = root->right != NULL ? root->right->size : 0;
            printf( abs(left_size - right_size) <= 1 ? "yes\n" : "no\n");
        }
    }
    return 0;
}

btlink NODE(int num, btlink par, btlink left, btlink right, int size)
{
    btlink born = malloc(sizeof (btree));
    born->par = par;
    born->num = num;
    born->left = left;
    born->right = right;
    born->size = size;
    return born;
}

btlink bt_create(int num, btlink par) //number begin with 1
{
    if (num == -1)
        return NULL;
    btlink root = NODE(num, par, NULL, NULL, 1);
    num_map_root[num] = root;
    root->par = par;
    root->left = bt_create(left_num[num], root);
    root->right = bt_create(right_num[num], root);
    if (root->left != NULL && root->right != NULL)
        root->size = 1 + root->left->size + root->right->size;
    else if (root->left != NULL)
        root->size = 1 + root->left->size;
    else if (root->right != NULL)
        root->size = 1 + root->right->size;
    return root;
}

btlink bt_left_rotate(btlink root)
{
    btlink new_root = root->right;
    new_root->par = root->par;  //change par
    root->right = new_root->left;
    if (new_root->left != NULL)
        new_root->left->par = root; //change par
    new_root->left = root;
    root->par = new_root;       //change par
    //modify size
    new_root->size += root->left != NULL ? (root->left->size + 1) : 1;
    root->size -= new_root->right != NULL ? (new_root->right->size + 1) : 1;
    return new_root;
}

btlink bt_right_rotate(btlink root)
{
    btlink new_root = root->left;
    new_root->par = root->par;   //change par
    root->left = new_root->right;
    if (new_root->right != NULL)
        new_root->right->par = root; //change par
    new_root->right = root;
    root->par = new_root;        //change par
    //modify size
    new_root->size += root->right != NULL ? (root->right->size + 1) : 1;
    root->size -= new_root->left != NULL ? (new_root->left->size + 1) : 1;
    return new_root;
}

void tree_print(btlink root, FILE *fd)
{
    fprintf(fd, "(");
    if (root != NULL) {
        fprintf(fd, "%d", root->num);
        tree_print(root->left, fd);
        tree_print(root->right, fd);
    }
    fprintf(fd, ")");
}

void bt_show_by_tree(btlink root)
{
    char cmd[CMD_LEN];
    
    sprintf(cmd, "rm -f ./tree_src.txt");
    system(cmd);
    
    FILE *fd = fopen("./tree_src.txt", "a+");
    fprintf(fd, "\n\t\\tree");
    tree_print(root, fd);
    fprintf(fd, "\n\n");
    fclose(fd);
    
    sprintf(cmd, "cat ./tree_src.txt | ~/tree/tree");
    system(cmd);
}
