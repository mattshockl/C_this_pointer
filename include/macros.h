#ifndef THIS_MACROS_H
#define THIS_MARCOS_H

#define THIS_KEYWORD this

#define class_decl(classname) \
typedef struct classname classname

#define method_decl(classname, rettype, methodname, args) \
typedef rettype (*classname ## methodname) args; \
rettype classname ## _ ## methodname ## _decl args

#define method_def(classname, rettype, methodname, args, body) \
rettype classname ## _ ## methodname ## _decl args { \
    register int *__this asm("rax"); \
    classname *THIS_KEYWORD = (classname*)__this; \
    return ({ \
        rettype __fn ## classname ## _ ## methodname ## _decl__ (void) body \
        __fn ## classname ## _ ## methodname ## _decl__; \
    })(); \
}

#define class_def(classname, body, init_args, init, dest) \
struct classname body; \
void classname ## _destroy(classname *THIS_KEYWORD) \
{ \
    ({ \
        void __fn ## classname ## destroy__ (void) dest \
        __fn ## classname ## destroy__; \
    })(); \
    free(THIS_KEYWORD); \
} \
classname* classname ## _init init_args \
{ \
    classname *THIS_KEYWORD = calloc(1, sizeof(classname)); \
    if (NULL == THIS_KEYWORD) return NULL; \
    THIS_KEYWORD->destroy = classname ## _destroy; \
    ({ \
        void __fn ## classname ## init__ (void) init \
        __fn ## classname ## init__; \
    })(); \
    return THIS_KEYWORD; \
} \

#define method(classname, methodname) \
classname ## methodname methodname

#define method_init(classname, methodname) \
THIS_KEYWORD->methodname = (classname ## methodname)__this_create_trampoline(THIS_KEYWORD, & classname ## _ ## methodname ## _decl)

#define new(classname, ...) \
classname ## _init (__VA_ARGS__)

#define destroy(object) \
object->destroy(object)

#define class_destroy(classname) \
void (*destroy) (classname *THIS_KEYWORD)

#define method_destroy(classname, methodname) \
__this_destroy_trampoline(THIS_KEYWORD->methodname)

#endif