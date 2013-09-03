typedef struct _comp_tree_t {
	int key;
	int value;
	struct _comp_tree_t *parent;
	struct _comp_tree_t *brother;
	struct _comp_tree_t *child;
} comp_tree_t;


comp_tree_t *getKeyNode (comp_tree_t *tree, int key);

void createTree			(comp_tree_t **tree);
void printTree			(comp_tree_t *tree);
void destroyTree		(comp_tree_t **tree);

int countTreeNodes		(comp_tree_t *tree);
int containsValue		(comp_tree_t *tree, int value);
int countLeafs			(comp_tree_t *tree);
int countDepth			(comp_tree_t *tree);
int isTreeEmpty			(comp_tree_t *tree);
int containsKey			(comp_tree_t *tree, int key);
int insert				(comp_tree_t **tree, int value, int key, int parentKey);
int updateValue			(comp_tree_t *tree, int key, int newValue);
int removeTreeNode		(comp_tree_t **tree, int keyNode);
