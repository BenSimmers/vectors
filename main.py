import ctypes
import threading

class Vector(ctypes.Structure):
    _fields_ = [
        ('data', ctypes.POINTER(ctypes.c_char_p)),
        ('size', ctypes.c_int),
        ('capacity', ctypes.c_int)
    ]

class VectorDatabase(ctypes.Structure):
    _fields_ = [
        ('vectors', ctypes.POINTER(Vector)),
        ('count', ctypes.c_int),
        ('capacity', ctypes.c_int)
    ]

libvector = ctypes.CDLL('./libvector.so')

# Initialize Vector
libvector.vector_init.argtypes = [ctypes.POINTER(Vector), ctypes.c_int]

# Append an element to Vector
libvector.vector_append.argtypes = [ctypes.POINTER(Vector), ctypes.c_char_p]

# Print Vector
libvector.vector_print.argtypes = [ctypes.POINTER(Vector)]

# Free Vector
libvector.vector_free.argtypes = [ctypes.POINTER(Vector)]

# Initialize VectorDatabase
libvector.vector_db_init.argtypes = [ctypes.POINTER(VectorDatabase), ctypes.c_int]

# Add Vector to VectorDatabase
libvector.vector_db_add.argtypes = [ctypes.POINTER(VectorDatabase), ctypes.POINTER(Vector)]

# Print VectorDatabase
libvector.vector_db_print.argtypes = [ctypes.POINTER(VectorDatabase)]

# Free VectorDatabase
libvector.vector_db_free.argtypes = [ctypes.POINTER(VectorDatabase)]

# Save VectorDatabase to file
libvector.vector_db_save.argtypes = [ctypes.POINTER(VectorDatabase), ctypes.c_char_p]

# Load VectorDatabase from file
libvector.vector_db_load.argtypes = [ctypes.POINTER(VectorDatabase), ctypes.c_char_p]

# Add a word to VectorDatabase
libvector.add_word.argtypes = [ctypes.POINTER(VectorDatabase)]

# Search for a word in VectorDatabase
libvector.searchForWord.argtypes = [ctypes.POINTER(VectorDatabase)]

# Define the add_word function as a separate thread

def add_word_thread(db):
    libvector.add_word(db)

# Create the vector and vector database instances

vec = Vector()
libvector.vector_init(ctypes.byref(vec), 4)
libvector.vector_append(ctypes.byref(vec), b"apple")
libvector.vector_append(ctypes.byref(vec), b"banana")
libvector.vector_print(ctypes.byref(vec))

db = VectorDatabase()
libvector.vector_db_init(ctypes.byref(db), 4)
libvector.vector_db_load(ctypes.byref(db), b"db.txt")
libvector.vector_db_print(ctypes.byref(db))

choice = 0
while choice != 5:
    print("\nVector Database in C :D\n")
    print("1. Add a word")
    print("2. Print the database")
    print("3. Save the database")
    print("4. Search for a word")
    print("5. Exit")
    try:
        choice = int(input("Enter your choice: "))
    except ValueError:
        print("Invalid input. Please enter a valid integer choice.")
        continue

    if choice == 1:
        thread = threading.Thread(target=add_word_thread, args=(ctypes.byref(db),))
        thread.start()
        thread.join()
    elif choice == 2:
        libvector.vector_db_print(ctypes.byref(db))
    elif choice == 3:
        libvector.vector_db_save(ctypes.byref(db), b"db.txt")
    elif choice == 4:
        libvector.searchForWord(ctypes.byref(db))
    elif choice == 5:
        libvector.vector_db_free(ctypes.byref(db))
    else:
        print("Invalid choice!")