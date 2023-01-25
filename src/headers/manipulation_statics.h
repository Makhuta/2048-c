static void move_left(Main_obj *self);

static void move_right(Main_obj *self);

static void move_up(Main_obj *self);

static void move_down(Main_obj *self);

static void set_half_distance_left(Main_obj* self);

static void set_half_distance_right(Main_obj* self);

static void set_half_distance_up(Main_obj* self);

static void set_half_distance_down(Main_obj* self);

static void merge_left(Main_obj *self);

static void merge_right(Main_obj *self);

static void merge_up(Main_obj *self);

static void merge_down(Main_obj *self);

static void move_cell_left(Main_obj* self, const int cell_id);

static void move_cell_right(Main_obj* self, const int cell_id);

static void move_cell_up(Main_obj* self, const int cell_id);

static void move_cell_down(Main_obj* self, const int cell_id);

static bool check_box_element(Main_obj* self, const int adder, const int x, const int y);