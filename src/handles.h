/* Don't forget to make longjump on error clear handles */

//handle_t *handle_push(cell_t protected);
//void handle_pop(handle_t *handle);
//cell_t *handle_get(handle_t *handle);
//cell_t *handle_set(handle_t *handle, cell_t *value);

typedef cell_t handle_t;
#define handle_push(x) (x)
#define handle_pop(x)
#define handle_get(x) (x)
#define handle_set(x, y) ((x) = (y))
