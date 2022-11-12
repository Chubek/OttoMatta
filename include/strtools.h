/*
strtools is responsible for splitting a string into chunks using a regex
pattern.
*/

typedef struct {
    char *data;
    size_t start, end;
    int *ref_counter;
} strt_s;

strt_s *strt_new(char const *in);
strt_s *strt_copy(strt_s const *in, size_t start, size_t len);
void strt_free(strt_s *in);


typedef struct {
    strt_s **strings;
    int count;
} strtlist_s;

strtlist_s strt_split(strt_s const *in, gchar const *start_pattern);
void strtlist_free(strtlist_s in);

