# Book Scanning
### Google HashCode 2020 - Online Qualification Round

Here we are at the HashCode 2020! In this repository, I suggest some solutions to the Book Scanning problem proposed during the online qualification round.

### First Attempt: Sorting libraries
The first thing I tried is also the dumbest one (but why not?!). It starts calculating a score for each library (sum up the score of each book in the library) and sorting libraries by decreasing scores.
Then I put everything in the solution file: who cares, the scoring software will deal with not counting repeated books or limiting the number of libraries due to the deadline.
Ok, with python this was so easy and the program was only 15 lines long (well, not including input and output procedures).

|Instance|Score|
|------|-------:|
|A - example | 21|
|B - read on | 4,126,100|
|C - incunabula | 1,413,940|
|D - tough choices | 4,841,980|
|E - so many books | 1,114,118|
|F - libraries of the world | 1,210,345|
|**Total** | 12,706,504|

### Second Attempt: Sorting libraries (by signup time)
Ok, not a big deal, I just changed 1 line from the previous attempt.

|Instance|Score|
|------|-------:|
|A - example | 21|
|B - read on | 5,822,900|
|C - incunabula | 5,467,966|
|D - tough choices | 4,150,120|
|E - so many books | 4,179,197|
|F - libraries of the world | 2,430,950|
|**Total** | 22,051,154|

### Third Attempt: IDK
This was quite hard for the poor Python interpreter, so I switched to C++.

|Instance|Score|
|------|-------:|
|A - example | 21|
|B - read on | 5,822,900|
|C - incunabula | 1,480,105|
|D - tough choices | 5,028,530|
|E - so many books | 3,327,240|
|F - libraries of the world | 1,832,987|
|**Total** | 17,491,783|