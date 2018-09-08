#include "commands.h"

/*
 * Define global externs and global variables.
 * NOTE: cmd_ptrs[] and cmds[] MUST retain the same ordering.
 */
int (*cmd_ptrs[])(char*) = {(int (*)())mkdir, rmdir, ls, cd, pwd, 
                            creat, rm, reload, save, menu, 
                            quit};
char *cmds[] = {"mkdir", "rmdir", "ls", "cd", "pwd",
                "creat", "rm", "reload", "save", "menu", 
                "quit", NULL};
NODE *root = NULL, *cwd = NULL, *start = NULL;
char *tok_list[MAX_INPUT_LEN] = {NULL};
FILE *fp = NULL;
bool is_saving = false;

int init(void) {
    root = cwd = start = create_node("/", 'd');
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
        tok_list[n] = (char*)malloc(strlen(s) + 1);
        strcpy(tok_list[n++], s);
        s = strtok(NULL, delimiter);
    }
    /* Mark the end of the tok list */
    tok_list[n] = NULL;
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

    if (!strncmp(name,  "..", 2)) return parent->parent;
    else if (!strncmp(name,  ".", 1)) return parent;

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

int mkdir(char *path) {
    _mk(path, 'd', "mkdir");
}

int rmdir(char* path) {
    _rm(path, 'd', "rmdir");
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
    is_saving = false;
    if (cwd == root) { printf("/\n"); }
    else { rpwd(cwd); printf("\n"); }
}

int rpwd(NODE* curr) {
    if (curr != root) {
        char cwd_name[MAX_PATH_LEN] = "\0";
        strcpy(cwd_name, curr->name);
        rpwd(curr->parent);
        fprintf((is_saving) ? fp : stdout, "/%s", cwd_name);
    }
}

int creat(char* path) {
    _mk(path, 'f', "creat");
}

int rm(char* path) {
    _rm(path, 'f', "rm");
}

int reload(char* filename) {
    kill_tree(root);
    init();

    /* Open file for read. */
    fp = fopen(filename, "r");

    /* Load file. */
    char line[MAX_INPUT_LEN], path[MAX_PATH_LEN], type;
    bool err = false;
    while (fgets(line, MAX_INPUT_LEN - 1, fp)) {
        line[strlen(line) - 1] = '\0';
        sscanf(line, "%c,%s", &type, path);
        switch (type) {
            case 'd': mkdir(path); break;
            case 'f': creat(path); break;
            default: err = true; break;
        }
        if (err) break;
    }

    if (err) printf("reload: error create file system tree\n");

    /* Close file handle. */
    fclose(fp);
}

int save(char* filename) {
    /* Open file for write. */
    fp = fopen(filename, "w");

    /* Write to file in preorder. MUST BE FULL PATHNAME... utilize r_pwd */
    is_saving = true;
    preorder_to_file(root);

    /* Close file handle. */
    fclose(fp);
}

int menu(char* path) {
    int i = 0;
    printf("=======================MENU=========================\n");
    while (cmds[i]) {
        printf("%s ", cmds[i++]);
    }
    printf("\n====================================================\n");
}

int quit(char* path) {
    clear_tok_list();
    kill_tree(root);
}

int _mk(char *path, char type, char *mk_cmd) {
    int len = strlen(path), i;
    char dirname[MAX_PATH_LEN] = "", basename[MAX_PATH_LEN] = "", err_msg[MAX_ERROR_MSG_LEN] = "";
    bool err = false;

    /* Break path into absolute & relative bits. */
    for (i = len - 1; path[i] != '/' && i>=0; --i);
    strcpy(basename, &path[i+1]);
    int correct_i = i < 0 ? 0 : i;
    strncpy(dirname, path, correct_i);
    dirname[correct_i] = '\0';
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
                p = create_node(basename, type);
                p->parent = d;
                /* Now, insert p at the end of linked list which starts at d->child. Pass in double ptr. */
                insert_end(&d->child, p);
            }
        }
    } else {
        err = true;
        strcpy(err_msg, "No such file or directory");
    }

    if (err)
        printf("%s: cannot create directory '%s': %s\n", mk_cmd, path, err_msg);
        return false;
}

int _rm(char *path, char type, char *mk_cmd) {
    NODE *p = path_to_node(path);
    char err_msg[MAX_ERROR_MSG_LEN] = "";
    bool err = false;
    if (p) {
        if (p->type != type) {
            err = true;
            strcpy(err_msg, "Incorrect type");
        } else {
            if (p->child && p->type == 'd') {
                err = true;
                strcpy(err_msg, "Directory not empty");
            } else {
                remove_from_list(p);
            }
        }
    } else {
        err = true;
        strcpy(err_msg, "No such file or directory");
    }

    if (err)
        printf("%s: cannot remove '%s': %s\n", mk_cmd, path, err_msg);
        return false;
}

int preorder_to_file(NODE *tree) {
    if (tree) {
        /* We'll use a comma delimiter since whitespace delimiters can be weird.
        Also, ignore saving the root. It's pointless to store, since it must exist. */
        if (tree != root) {
            fprintf(fp, "%c,", tree->type);
            rpwd(tree); // CODE REUSE
            fprintf(fp, "\n");
        }
        preorder_to_file(tree->child);
        preorder_to_file(tree->sibling);
    }
}