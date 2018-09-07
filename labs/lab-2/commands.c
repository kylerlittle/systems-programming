#include "commands.h"

/*
 * Define global externs.
 * NOTE: These two arrays must retain the same ordering.
 */
int (*cmd_ptrs[])(char*) = {(int (*)())mkdir, rmdir, ls, cd, pwd, 
                            creat, rm, reload, save, menu, 
                            quit};
char *cmds[] = {"mkdir", "rmdir", "ls", "cd", "pwd",
                "creat", "rm", "reload", "save", "menu", 
                "quit", NULL};
NODE *root = NULL, *cwd = NULL, *start = NULL;
char *tok_list[MAX_INPUT_LEN] = {NULL};
char* some_path = "/dir2";

int init(void) {
    root = cwd = start = create_node("/", 'd');
    printf("root name: %s\troot type: %c\n", root->name, root->type);
    printf("cwd name: %s\tcwd type: %c\n", cwd->name, cwd->type);
    printf("start name: %s\tstart type: %c\n", start->name, start->type);
    NODE *chld = create_node("dir1", 'd'), *sblng = create_node("dir2", 'd');
    chld->sibling = sblng;
    chld->parent = sblng->parent = root;
    root->child = chld;
    
    NODE *srch_for_node = path_to_node(some_path);
    printf("found dir2: %s\n", srch_for_node->name);

    mkdir("/hi/wassup/up/sdfl/sdf");
    mkdir("/dir2/hello");
}

int tokenize(char *pathname) {
    char *s, path_copy[MAX_PATH_LEN], *delimiter = "/";
    int n = 0;

    /* Make copy of pathname, so as not to destroy it. */
    strcpy(path_copy, pathname);

    /* Clear tok_list */
    clear_tok_list();

    s = strtok(path_copy, delimiter);
    while (s) {
        if (DEBUG_MODE)
            printf("%s ", s);
        tok_list[n] = (char*)malloc(strlen(s) + 1);
        strcpy(tok_list[n++], s);
        s = strtok(NULL, delimiter);
    }
    /* Mark the end of the tok list */
    tok_list[n] = NULL;

    if (DEBUG_MODE)
        printf("\n");
    return n;
}

int clear_tok_list(void) {
    int i = 0;
    while (tok_list[i]) {
        free(tok_list[i]);
        ++i;
    }
}

NODE *search_for_child(NODE *parent, char *name) {
    NODE *list = parent->child;

    while (list) {
        if (!strcmp(list->name, name)) {
            if (DEBUG_MODE) printf("***ASSERT*** %s == %s\n", list->name, name);
            break;
        }
        list = list->sibling;
    }
    /* If here, list is NULL or points to child with correct name. */
    return list;
}

NODE *path_to_node(char *pathname) {
    // if (!strcmp(pathname, ".")) {

    // }
    start = (pathname[0] == '/') ? root : cwd;
    NODE *p = start;

    int n = tokenize(pathname), i;
    for (i = 0; i < n; ++i) {
        p = search_for_child(p, tok_list[i]);
        if (!p) break;
    }
    return p;
}

int find_cmd(char *cmd) {
    int i = 0;

    /* Loop while cmds[i] isn't equal to NULL */
    while (cmds[i]) {
        if (!strcmp(cmd, cmds[i]))
            return i;
        i++;
    }
    return INVALID_INDEX;
}

int mkdir(char path[]) {
    int len = strlen(path), i;
    char dirname[MAX_PATH_LEN], basename[MAX_PATH_LEN], err_msg[MAX_ERROR_MSG_LEN];
    bool err = false;

    /* Break path into absolute & relative bits. */
    for (i = len - 1; path[i] != '/'; --i);
    strcpy(basename, &path[i+1]);
    strncpy(dirname, path, i);
    dirname[i] = '\0';
    if (DEBUG_MODE) printf("path: %s\tdirname: %s\t basename: %s\n", path, dirname, basename);

    /* Search for dirname node. */
    NODE *d = path_to_node(dirname);
    if (d) {
        if (d->type != 'd') {
            err = true;
            strcpy(err_msg, "Not a directory");
        } else {
            NODE *p = search_for_child(d, basename);
            if (p) {
                err = true;
                strcpy(err_msg, "File exists");
            } else {
                p = create_node(basename, 'd');
                /* Now, insert p at the end of linked list which starts at d->child. Pass in double ptr. */
                insert_end(&d->child, p);
            }
        }
    } else {
        err = true;
        strcpy(err_msg, "No such file or directory");
    }

    if (err)
        printf("mkdir: cannot create directory '%s': %s\n", path, err_msg);
        return false;
}

int rmdir(char* path) {
    printf("path: %s\n", path);
}

int ls(char* path) {
    NODE* p = path_to_node(path);

    /* Now look at p's children. Add extra if statement to avoid printing 
    newline when p has no children. */
    p = p->child;
    if (p) {
        while (p) {
            printf("[%c]%s\t", p->type, p->name);
            p = p->sibling;
        }
        printf("\n");
    }
}

int cd(char* path) {
    NODE* p = path_to_node(path);
    if (p) {
        if (p->type == 'd') cwd = p;
        else printf("%s: Not a directory\n", p->name);
    } else {
        printf("%s: No such file or directory\n", path);
    }
}

int pwd(char* path) {
    rpwd(cwd);
    printf("\n");
}

int rpwd(NODE* curr) {
    if (curr != root) {
        char cwd_name[MAX_PATH_LEN] = "\0";
        strcpy(cwd_name, curr->name);
        rpwd(curr->parent);
        printf("%s/", cwd_name);
    } else {
        printf("/");
    }
}

int creat(char* path) {
    printf("path: %s\n", path);
}

int rm(char* path) {
    printf("path: %s\n", path);
}

int reload(char* path) {
    printf("path: %s\n", path);
}

int save(char* path) {
    printf("path: %s\n", path);
}

int menu(char* path) {
    printf("path: %s\n", path);
}

int quit(char* path) {
    clear_tok_list();
    kill_tree(root);
}