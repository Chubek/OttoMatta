/*
strtools is responsible for splitting a string into chunks using a regex
pattern.
*/

typedef struct {
    char *data;
    size_t start, end;
    int *ref_counter; //pointer to the integer holding number of copies
} strt_s;

strt_s *strt_new(char const *in);
strt_s *strt_copy(strt_s const *in, size_t start, size_t len);
void strt_free(strt_s *in);

//the list containing separated substrs
typedef struct {
    strt_s **strings;
    int count;
} strtlist_s;

strtlist_s strt_split(strt_s const *in, gchar const *start_pattern);
void strtlist_free(strtlist_s in);


#define     IS_SPACE(c)        ((c == 32) ? 1 : 0)
#define     IS_QUOTE(c)         ((c == 34) ? 1 : 0)