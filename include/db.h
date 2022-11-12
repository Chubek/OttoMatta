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


typedef struct  {
    char fname[100];
    dbtype_e ftype;
} dbheader_s;

dbheader_s *dbheader_s_parse(char *in);

typedef union listtype_u {
    char c;
    int i;
} listtype_u;

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

typedef struct {
    int *ref_counter;
    dbheader_s header;
    dbcontent_u const *content;
} dbcol_s;

dbcol_s *serialize_db(char *in);
dbcol_s *copy_db(dbcol_s const *in);
char *deserialize_db(dbcol_s *in);
void free_one_db(dbcol_s *in);
