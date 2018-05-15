#pragma once



#define ELONA_APPEND_PROMPT(text, key, value) \
    do \
    { \
        promptl(0, promptmax) = (text); \
        promptl(1, promptmax) = (key); \
        promptl(2, promptmax) = (value); \
        ++promptmax; \
    } while (0)


#define ELONA_YES_NO_PROMPT() \
    do \
    { \
        ELONA_APPEND_PROMPT(i18n::_(u8"ui", u8"yes"), u8"y"s, u8"0"s); \
        ELONA_APPEND_PROMPT(i18n::_(u8"ui", u8"no"), u8"n"s, u8"1"s); \
    } while (0)



#define ELONA_TALK_SCENE_CUT() \
    do \
    { \
        talk_window_query(); \
        if (scenemode) \
        { \
            if (scene_cut == 1) \
            { \
                return talk_result_t::talk_end; \
            } \
        } \
    } while (0)


#define ELONA_APPEND_RESPONSE(x, y) \
    do \
    { \
        list(0, listmax) = (x); \
        listn(0, listmax) = (y); \
        ++listmax; \
    } while (0)



#define ELONA_GET_SELECTED_ITEM_BASE(var, ret, proc) \
    do \
    { \
        var = -1; \
        for (int i = 0; i < keyrange; ++i) \
        { \
            if (key == key_select(i)) \
            { \
                var = (ret); \
                (void)(proc); \
                break; \
            } \
        } \
    } while (0)


#define ELONA_GET_SELECTED_ITEM(var, proc) \
    ELONA_GET_SELECTED_ITEM_BASE(var, list(0, pagesize* page + i), (proc))

#define ELONA_GET_SELECTED_INDEX(var) \
    ELONA_GET_SELECTED_ITEM_BASE(var, pagesize* page + i, 0)

#define ELONA_GET_SELECTED_INDEX_THIS_PAGE(var) \
    ELONA_GET_SELECTED_ITEM_BASE(var, i, 0)
