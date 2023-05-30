#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "vector.h"


// typedef struct
// {
//   char **data;
//   int size;
//   int capacity;
// } Vector;

// typedef struct
// {
//   Vector *vectors;
//   int count;
//   int capacity;
// } VectorDatabase;


void vector_init(Vector *vec, int capacity)
{
  vec->data = (char **)malloc(capacity * sizeof(char *));
  vec->size = 0;
  vec->capacity = capacity;
}

void vector_append(Vector *vec, const char *value)
{
  if (vec->size >= vec->capacity)
  {
    vec->capacity *= 2;
    vec->data = (char **)realloc(vec->data, vec->capacity * sizeof(char *));
  }

  vec->data[vec->size] = strdup(value);
  vec->size++;
}

void vector_print(const Vector *vec)
{
  printf("[ ");
  for (int i = 0; i < vec->size; i++)
  {
    printf("%s ", vec->data[i]);
  }
  printf("]\n");
}

void vector_free(Vector *vec)
{
  for (int i = 0; i < vec->size; i++)
  {
    free(vec->data[i]);
  }
  free(vec->data);
  vec->size = 0;
  vec->capacity = 0;
}

void vector_db_init(VectorDatabase *db, int capacity)
{
  db->vectors = (Vector *)malloc(capacity * sizeof(Vector));
  db->count = 0;
  db->capacity = capacity;
}

void vector_db_add(VectorDatabase *db, const Vector *vec)
{
  if (db->count >= db->capacity)
  {
    db->capacity *= 2;
    db->vectors = (Vector *)realloc(db->vectors, db->capacity * sizeof(Vector));
  }

  db->vectors[db->count] = *vec;
  db->count++;
}

void vector_db_print(const VectorDatabase *db)
{
  for (int i = 0; i < db->count; i++)
  {
    printf("Vector %d: ", i);
    vector_print(&db->vectors[i]);
  }
}

void vector_db_free(VectorDatabase *db)
{
  for (int i = 0; i < db->count; i++)
  {
    vector_free(&db->vectors[i]);
  }

  free(db->vectors);
  db->count = 0;
  db->capacity = 0;
}

void vector_db_save(const VectorDatabase *db, const char *filename)
{
  FILE *file = fopen(filename, "w");
  if (file == NULL)
  {
    printf("Error opening file!\n");
    return;
  }

  for (int i = 0; i < db->count; i++)
  {
    for (int j = 0; j < db->vectors[i].size; j++)
    {
      fprintf(file, "%s ", db->vectors[i].data[j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

void vector_db_load(VectorDatabase *db, const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Error opening file!\n");
    return;
  }

  char line[100];
  while (fgets(line, sizeof(line), file))
  {
    Vector vec;
    vector_init(&vec, 4);

    char *token = strtok(line, " ");
    while (token != NULL)
    {
      vector_append(&vec, token);
      token = strtok(NULL, " ");
    }

    vector_db_add(db, &vec);
  }

  fclose(file);
}

void add_word(VectorDatabase* db)
{
  char word[100];
  printf("Enter a word to add: ");
  scanf("%s", word);

  // Check if the word partially matches with any existing word in the vectors
  for (int i = 0; i < db->count; i++)
  {
    for (int j = 0; j < db->vectors[i].size; j++)
    {
      if (strstr(db->vectors[i].data[j], word) != NULL || strstr(word, db->vectors[i].data[j]) != NULL)
      {
        // Partial match found, append the word to the existing vector
        vector_append(&db->vectors[i], word);
        vector_db_save(db, "db.txt"); // Save the updated vectors to the file
        return;
      }
    }
  }

  // No partial match found, create a new vector and add it to the database
  Vector newVector;
  vector_init(&newVector, 4);
  vector_append(&newVector, word);
  vector_db_add(db, &newVector);
  vector_db_save(db, "db.txt"); // Save the updated vectors to the file
}



// void searchForWord(VectorDatabase *db)
// {
//   char word[100];
//   printf("Enter a word to search for: ");
//   scanf("%s", word);

//   for (int i = 0; i < db->count; i++)
//   {
//     for (int j = 0; j < db->vectors[i].size; j++)
//     {
//       if (strcmp(db->vectors[i].data[j], word) == 0)
//       {
//         printf("Vector %d: ", i);
//         vector_print(&db->vectors[i]);
//       }
//     }
//   }
// }

void searchForWord(VectorDatabase* db)
{
  char word[100];
  printf("Enter a word to search for: ");
  scanf("%s", word);

  for (int i = 0; i < db->count; i++)
  {
    for (int j = 0; j < db->vectors[i].size; j++)
    {
      if (strcmp(db->vectors[i].data[j], word) == 0)
      {
        printf("Exact match found in Vector %d: ", i);
        vector_print(&db->vectors[i]);
        return; // Break out of the loop once an exact match is found
      }
    }
  }

  printf("No exact match found for the word '%s'.\n", word);
}


void menu()
{
  printf("\n");
  printf("Vector Database in c :D\n");
  printf("\n");
  system("clear");
  printf("1. Add a word\n");
  printf("2. Print the database\n");
  printf("3. Save the database\n");
  printf("4. Search for a word\n");
  printf("5. Exit\n");
  printf("Enter your choice: ");
}

void* add_word_thread(void* arg)
{
  VectorDatabase* db = (VectorDatabase*)arg;
  add_word(db);
  return NULL;
}



int main()
{

  
  VectorDatabase db;
  vector_db_init(&db, 4);
  vector_db_load(&db, "db.txt");

  Vector vec1;
  vector_init(&vec1, 4);
  vector_append(&vec1, "apple");
  vector_append(&vec1, "banana");
  vector_append(&vec1, "cherry");

  int choice = 0;
  while (choice != 4)
  {
    menu();
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      // add_word(&db);
       {
        pthread_t thread;
        pthread_create(&thread, NULL, add_word_thread, &db);
        pthread_join(thread, NULL);
      }

      break;
    case 2:
      vector_db_print(&db);
      break;
    case 3:
      vector_db_save(&db, "db.txt");
      break;
    case 4:
        searchForWord(&db);
      break;
    case 5:
      vector_db_free(&db);
      // vector_db_save(&db, "db.txt");
      break;
      

    default:
      printf("Invalid choice!\n");
      break;
    }
  }
  return 0;
}