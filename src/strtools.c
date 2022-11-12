#include "../include/ottomatta.h"


/*
Creates a new strtools structure and assigns 1 to the refererence counter
of that object. It sets the data for strtools to the (in) data after casting
it to a normal character buffer. Sets start to 0 and end to the length of the
buffer if it's not null and 0 if null
*/
strt_s *strt_new(char const *in) {
    strt_s *out = malloc(sizeof(strt_s));
    *out = (strt_s){.start=0, .ref_counter=malloc(sizeof(int))};
    out->data = (char *)(in);
    out->end = out->data ? strlen(out->data) : 0;
    *out->ref_counter = 1;

    return out;
}

/*
Makes a copy of the stringtools with start and length set. 
Increases reference counter.
*/
strt_s *strt_copy(strt_s const *in, size_t start, size_t len){
    strt_s *out = malloc(sizeof(strt_s));
    *out=*in;
    out->start += start;
    if (in->end > out->start + len)
    out->end = out->start + len;
    (*out->refs)++;

    return out;
}


/*
Decreases the reference counter. If it reaches zero it will free the
reference to data and reference to ref counter. Then it will free the copy.
*/
void strt_free(strt_s *in){
    (*in->refs)--;
    if (!*in->refs) {
        free(in->data);
        free(in->ref_counter);
    }

    free(in);
}

/*
Use regex from Glib to split the string into chunks. Returns a list object.
*/
strtlist_s strt_split (strt_s const *in, gchar const *start_pattern){
    if (!in->data) return (strtlist_s){ };
    strt_s **out=malloc(sizeof(strt_s*));
    int outlen = 1;

    out[0] = strt_copy(in, 0, in->end);
    GRegex *start_regex = g_regex_new (start_pattern, 0, 0, NULL);
    gint mstart=0, mend=0;
    strt_s *remaining = strt_copy(in, 0, in->end);

    do {
        GMatchInfo *start_info;
        g_regex_match(start_regex, &remaining->data[remaining->start],
            0, &start_info);

        g_match_info_fetch_pos(start_info, 0, &mstart, &mend);
        g_match_info_free(start_info);

        if (mend > 0 && mend < remaining->end - remaining->start){
            out = realloc(out, ++outlen * sizeof(strt_s*));
            
            out[outlen-1] = strt_copy(remaining, mend, remaining->end-mend);
            out[outlen-2]->end = remaining->start + mstart;
            remaining->start += mend;
        } else break;
    } while (1);

    strt_free(remaining);
    g_regex_unref(start_regex);

    return (strtlist_s){.strings=out, .count=outlen};
}

/*
Free the list object.
*/
void fstrtlist_s_free(fstrtlist_s in){
    for (int i=0; i< in.count; i++){
        fstr_free(in.strings[i]);
    }

    free(in.strings);
}   