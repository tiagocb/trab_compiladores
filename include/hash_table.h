
typedef struct {
	char *key;
	void *data;
} _gst_hash_table_item;

typedef struct _gst_hash_table_node {
	_gst_hash_table_item *item;
	struct _gst_hash_table_node *next;
} _gst_hash_table_node;

typedef struct _gst_hash_table {
	int numberOfLists;
	int numberOfElements;
	_gst_hash_table_node **table;
} _gst_hash_table;

int _gst_createTable(_gst_hash_table *ht, int numberOfLists);
int _gst_getNumberOfKeys(_gst_hash_table ht);
unsigned int _gst_hashFunction(int numberOfLists, char *key);
_gst_hash_table_item *_gst_searchKey(_gst_hash_table ht, char *key);
int _gst_insertKey(_gst_hash_table *ht, char *key, void *data);
int _gst_updateKey(_gst_hash_table ht, char *key, void *data);
void *_gst_getData(_gst_hash_table ht, char *key);
void *_gst_deleteKey(_gst_hash_table *ht, char *key);
void _gst_destroyTable(_gst_hash_table *ht);
void _gst_clearTableContent(_gst_hash_table *ht);
void _gst_printTable(_gst_hash_table ht);
