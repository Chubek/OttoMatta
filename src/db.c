#include <../include/ottomatta.h>

/*
When passed an (in) with start and end, it will parse a single field/type 
couple into (header).
*/
void dbheader_parse_single(
        char *in, 
        size_t start, 
        size_t end, 
        dbheader_s *header
    ) {
        int l = end - start;
        char fragment[l + 1];

        size_t j = 0;
        for (size_t i = start; i < end; i++) {
            fragment[j] = in[i];
            ++j;
        }
        fragment[l] = '\0';

        char *scratch, *delim = '\32'; //separating by space
        fragment = strtok_s(fragment, &l, delim, &scratch);
        
        int str_num = 0;
        while (!fragment) {
            switch str_num {
                case 0:
                    if (strlen(fragment) >= 100) {
                        fprintf(stderr, "Length of %s is larger than 100", fragment);
                        exit(1);
                    }
                    memcpy(header->fname, fragment, strlen(fragment));
                    goto delim_null;
                case 1:
                    *header->ftype = GET_FIELD_ENUM(fragment);
                    break;
            }

            delim_null:
            ++str_num;
            fragment = strtok_s(NULL, &l, delim, &scratch);
        }

}   


/*
When passed a strtools list which contains start and length of a 
given string in format `"field1 type1", "field2 type2"` it takes every
piece of string contained within `"` and parses them into the (container).
*/
void dbheader_parse(strtlist_s *in, dbheader_s container[]) {
    for (int i = 0; i < in->count; i++) {
        strt_s *curr_fragment = in[i];

        char *data = *curr_fragment.data;
        size_t start = *curr_fragment.start;
        size_t end = *curr_fragment.end;

        char sans_quote[end - start];
        
        size_t j = 0;
        for (size_t i = start; i < end; i++) {
            if (!IS_QUOTE(data[i])) {
                sans_quote[j] = data[i];

                ++j;
            }
        }

        dbheader_s curr_header;
        dbheader_parse_single(data, start, end, &curr_header);
        container[i] = curr_header;
    }

    strlist_free(in);
}