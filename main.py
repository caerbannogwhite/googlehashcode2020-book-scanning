
import time
import numpy as np

from collections import namedtuple
from operator import attrgetter

#Library = namedtuple("Library", ["id", "num_books", "signup", "books_per_day", "score", "books"])
class Library(object):
    def __init__(self, i, n, s, bpd, score, b):
        self.id = i
        self.num_books = n
        self.signup = s
        self.books_per_day = bpd
        self.score = score
        self.books = b

LibOut = namedtuple("LibOut", ["id", "books"])

def read_input(filename):

    fh = open(filename, "r")

    # intro section
    B, L, D = [int(i) for i in fh.readline().split()]
    scores = [int(i) for i in fh.readline().split()]

    # libraries section
    libraries = []
    for j in range(L):
        b, sign, bpd = [int(i) for i in fh.readline().split()]
        books = set([int(i) for i in fh.readline().split()])

        # calculate the total score for this library
        lib_score = sum([scores[book] for book in books])

        libraries.append(Library(j, b, sign, bpd, lib_score, books))

    fh.close()

    return B, L, D, scores, libraries


def print_output(filename, output):

    spt = filename.split("/")
    spt[-1] = "solution_" + spt[-1]
    filename = "/".join(spt)

    fh = open(filename, "w")
    fh.write("%d\n" % (len(output)))
    for lib in output:
        fh.write("%d %d\n" % (lib.id, len(lib.books)))
        fh.write("%s\n" % (" ".join(map(str, lib.books))))

    fh.close()


######################      INPUT       #################
filename = input()
filename = "data/" + filename + ".txt"
B, L, D, scores, libs = read_input(filename)


# ############    STRATEGY 1  ###############
# # sort libraries by score (reverse) OR by signup

#libs.sort(key=attrgetter("score"), reverse=True)
# OR
libs.sort(key=attrgetter("signup"))
scanned_books = set()
output = []

for lib in libs:

    sorted_books = sorted(lib.books.difference(scanned_books), key=lambda x: scores[x], reverse=True)

    lib_out = LibOut(lib.id, sorted_books)
    if len(lib_out.books) == 0:
        continue

    output.append(lib_out)

    scanned_books.update(lib.books)


# ################    STRATEGY 2  #############
# # boh

# libs.sort(key=attrgetter("score"))
# scanned_books = set()
# output = []

# tlim = 10**40
# tstart = time.time()

# print(B, L, D)

# d = 0
# while d <= D and len(libs) > 0 and len(scanned_books) < L:

#     left = D - d
#     print("left: %d, D: %d, scan: %d, L: %d" % (left, D, len(scanned_books), L))
#     for lib in libs:
#         lib.books.difference_update(scanned_books)
#         limit = (left - lib.signup) * lib.books_per_day
#         lib.score = sum([scores[b] for b in sorted(lib.books, key=lambda x: -scores[x])[:limit]])

#     maximum = 0
#     pos = -1
#     for i, lib in enumerate(libs):
#         if lib.score > maximum:
#             maximum = lib.score
#             pos = i

#     lib = libs.pop(i)
#     limit = (left - lib.signup) * lib.books_per_day
#     sorted_books = sorted(lib.books, key=lambda x: -scores[x])[:limit]

#     if len(sorted_books) == 0:
#         print("no sorted books")
#         continue

#     output.append(LibOut(lib.id, sorted_books))

#     d += lib.signup
#     scanned_books.update(sorted_books)

#     if time.time() - tstart > tlim:
#         break


####################    OUTPUT      #######################
print_output(filename, output)

