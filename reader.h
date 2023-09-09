
typedef void (*read_pair_cb)(int key, char *value);

int read_pair(const char *input_file, read_pair_cb read_pair);
