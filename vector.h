#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  char **data;
  int size;
  int capacity;
} Vector;

typedef struct {
  Vector *vectors;
  int count;
  int capacity;
} VectorDatabase;

void vector_init(Vector *vec, int capacity);
void vector_append(Vector *vec, const char *value);
void vector_print(const Vector *vec);
void vector_free(Vector *vec);

void vector_db_init(VectorDatabase *db, int capacity);
void vector_db_add(VectorDatabase *db, const Vector *vec);
void vector_db_print(const VectorDatabase *db);
void vector_db_free(VectorDatabase *db);
void vector_db_save(const VectorDatabase *db, const char *filename);
void vector_db_load(VectorDatabase *db, const char *filename);
void add_word(VectorDatabase *db);
void searchForWord(VectorDatabase *db);

#endif
