typedef struct
{
    void *in;
    const void **array;
} arrayqueue_t;


arrayqueue_t* arrayqueue_new();

bool arrayqueue_is_empty( arrayqueue_t * qu);

void *arrayqueue_peek( arrayqueue_t * qu);

void *arrayqueue_peektail( arrayqueue_t * qu);

void *arrayqueue_poll( arrayqueue_t * qu);

int arrayqueue_offer( arrayqueue_t * qu, void *item);

void arrayqueue_empty( arrayqueue_t * qu);

void arrayqueue_free( arrayqueue_t * qu);

int arrayqueue_count( arrayqueue_t * qu);

