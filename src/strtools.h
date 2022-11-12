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