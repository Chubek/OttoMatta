/*
Here lie the database code.

dtype_e: types of data enum.
dheader_s: header of the table struct
listtype_u: union that holds the listtype, only two types can be chosen. A single integer and a single char.
dbcontent_u: union that holds the possible content of one column.
dbcol_s: structure for one column.

*/

typedef enum {
    Integer,
    List,
    String,
    Datatime,
    Binary,
} dbtype_e;


#define     GET_FIELD_ENUM(s) {                                 \
                if (strncmp(s, "Integer", 7) == 0) {            \
                    return Integer;                             \
                } else if (strncmp(s, "List[]", 6) == 0) {      \
                    return List;                                \
                } else if (strncmp(s, "String", 6) == 0) {      \
                    return String;                              \
                } else if (strncmp(s, "Datetime", 8) == 0) {    \
                    return Datatime;                            \
                } else if (strncmp(s, "Binary", 6) == 0) {      \
                    return Binary;                              \
                }                                               \                      
            }                                                   


typedef struct  {
    char fname[100];
    dbtype_e ftype;
} dbheader_s;

void dbheader_parse_single(char *in, size_t start, size_t end, dbheader_s *header);
void dbheader_parse(strtlist_s in, dbheader_s container[]);

typedef union listtype_u {
    char c;
    int i;
} listtype_u;

// get a new listtype based on data type
listtype_u new_listtype(char const *in, dtype_e dtype);

typedef union {
    int integer;
    char string[10000];
    time_t datetime;
    char binary[1000000];
    listtype_u list[100];
} dbcontent_u;

// parse an entire column from the given (in) argument.
// *in must be a an array of comma-separated characters and comma itself must be escaped.
void parse_db_col_fields(
    dbcontent_u *db_content, 
    dbcol_s *db_col, 
    char const *in
);

// stuff for dbcol_s class
typedef struct {
    int *ref_counter; //reference counter holding number of copies
    dbheader_s header;
    dbcontent_u const *content;
} dbcol_s;

dbcol_s *serialize_db(char *in);
dbcol_s *copy_db(dbcol_s const *in);
char *deserialize_db(dbcol_s *in);
void free_one_db(dbcol_s *in);
