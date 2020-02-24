#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

typedef struct Library
{
    long id;
    long numBooks;
    long signup;
    long booksPerDay;
    std::vector<long> books;
} Library;

typedef struct Instance
{
    std::string filePath;

    long numBooks;
    long numLibraries;
    long numDays;
    std::vector<long> scores;
    std::vector<Library> libraries;
} Instance;

int read_input(Instance *inst, std::string filepath);
int read_solution(Instance *inst, std::string filepath);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Enter valid file path." << std::endl;
        return 1;
    }

    Instance inst;
    read_input(&inst, argv[1]);
    int score = read_solution(&inst, argv[2]);
    std::printf("Instance=%s, Score=%d\n", argv[1], score);

    return 0;
}

int read_input(Instance *inst, std::string filepath)
{
    long i, j;
    std::ifstream fileHandler;
    fileHandler.open(filepath);

    // Intro info
    fileHandler >> inst->numBooks >> inst->numLibraries >> inst->numDays;
    inst->scores = std::vector<long>(inst->numBooks);
    for (i = 0; i < inst->numBooks; ++i)
    {
        fileHandler >> inst->scores[i];
    }

    // Libraries section
    inst->libraries = std::vector<Library>(inst->numLibraries);
    for (j = 0; j < inst->numLibraries; ++j)
    {
        inst->libraries[j].id = j;
        fileHandler >> inst->libraries[j].numBooks >> inst->libraries[j].signup >> inst->libraries[j].booksPerDay;

        // Debug log info
        //std::cout << "id=" << inst->libraries[j].id << ", N=" << inst->libraries[j].numBooks << ", T=" << inst->libraries[j].signup << ", M=" << inst->libraries[j].booksPerDay << std::endl;

        inst->libraries[j].books = std::vector<long>(inst->libraries[j].numBooks);
        for (i = 0; i < inst->libraries[j].numBooks; ++i)
        {
            fileHandler >> inst->libraries[j].books[i];
        }
    }

    fileHandler.close();

    ///////////////////    CHECK INPUT     ////////////////////
    /*std::cout << "B=" << inst->numBooks << ", L=" << inst->numLibraries << ", D=" << inst->numDays << std::endl;
    std::cout << "scores=";
    for (i = 0; i < inst->numBooks; ++i)
    {
        std::cout << inst->scores[i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    for (j = 0; j < inst->numLibraries; ++j)
    {
        std::cout << "id=" << inst->libraries[j].id << ", N=" << inst->libraries[j].numBooks << ", T=" << inst->libraries[j].signup << ", M=" << inst->libraries[j].booksPerDay << std::endl;
        std::cout << "books=";
        for (i = 0; i < inst->libraries[j].numBooks; ++i)
        {
            std::cout << inst->libraries[j].books[i] << " ";
        }
        std::cout << std::endl << std::endl;
    }*/

    return 0;
}

int read_solution(Instance *inst, std::string filepath)
{
    long day, score, numLibraries, i, j, libId, limit, num, bookId, lineCounter;
    std::unordered_set<long> booksPassed;
    std::unordered_set<long> librariesLeft;
    std::ifstream fileHandler;

    fileHandler.open(filepath);

    // number of libraries 
    fileHandler >> numLibraries;
    lineCounter = 1;

    if (numLibraries > inst->numLibraries)
    {
        std::printf("line: %6ld - number of libraries greater than L (%ld > %ld).\n", lineCounter, numLibraries, inst->numLibraries);
        return -1;
    }

    // initialise libraries left
    for (i = 0; i < inst->numLibraries; ++i)
    {
        librariesLeft.insert(i);
    }

    day = 0;
    score = 0;
    for (i = 0; i < numLibraries; ++i)
    {
        fileHandler >> libId;
        fileHandler >> num;
        lineCounter++;

        if (librariesLeft.find(libId) == librariesLeft.end())
        {
            std::printf("line: %6ld - library %6ld not found or already scanned.\n", lineCounter, libId);
            return -1;
        }

        if (inst->libraries[libId].numBooks < num)
        {
            std::printf("line: %6ld - library %6ld: reported number of books is greater than number of books in library (%ld > %ld).\n", lineCounter, libId, num, inst->libraries[libId].numBooks);
            return -1;
        }

        // remove library libId from libraries left
        // ad update day
        librariesLeft.erase(libId);
        day += inst->libraries[libId].signup;

        // end the process id day is greater or equal to D
        if (day >= inst->numDays)
        {
            return score;
        }

        limit = (inst->numDays - day) * inst->libraries[libId].booksPerDay;
        lineCounter++;
        for (j = 0; j < num; ++j)
        {
            fileHandler >> bookId;

            // first time this book is counted (no repeated books)
            // and j must be lower than limit
            if ((booksPassed.find(bookId) == booksPassed.end()) && (j < limit))
            {
                score += inst->scores[bookId];
                booksPassed.insert(bookId);
            }
        }
    }

    return score;
}