#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>

typedef struct Library
{
    long id;
    long numBooks;
    long signup;
    long booksPerDay;
    long score;
    std::vector<long> books;
} Library;

typedef struct Instance
{
    long numBooks;
    long numLibraries;
    long numDays;
    std::vector<long> scores;
    std::vector<Library> libraries;
} Instance;

int read_input(Instance *inst, std::string filepath);
int print_output(std::vector<Library> output, std::string filepath);

int strategy_01(Instance *inst, std::vector<Library> &output);
int strategy_02(Instance *inst, std::vector<Library>& output);
int strategy_03(Instance *inst, std::vector<Library>& output);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Enter strategy (1, 2 or 3) and input file path." << std::endl;
        std::cout << "Example: ./prog 2 data/example_a.txt" << std::endl;
        return 1;
    }

    int strategy = std::atol(argv[1]);
    Instance inst;
    std::vector<Library> output;

    std::cout << "Reading instance from " << argv[2] << std::endl;
    read_input(&inst, argv[2]);
    
    if (strategy == 1)
    {
        std::cout << "Applying STRATEGY 01" << std::endl;
        strategy_01(&inst, output);
    } else
    if (strategy == 2)
    {
        std::cout << "Applying STRATEGY 02" << std::endl;
        strategy_02(&inst, output);
    } else
    if (strategy == 3)
    {
        std::cout << "Applying STRATEGY 03" << std::endl;
        strategy_03(&inst, output);
    } else
    {
        std::cout << "Strategy " << strategy << " not found." << std::endl;
        return 1;
    }

    std::cout << "Printing output" << std::endl;
    print_output(output, argv[2]);

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

int print_output(std::vector<Library> output, std::string filepath)
{
    std::vector<std::string> tokens;

    // get output file from input file path
    boost::split(tokens, filepath, boost::is_any_of("/"));
    *(tokens.end() - 1) = "solution_" + *(tokens.end() - 1);

    filepath = boost::join(tokens, "/");

    std::ofstream fileHandler;
    fileHandler.open(filepath);

    std::cout << "File opened at " << filepath << std::endl;

    // write number of libraries
    fileHandler << output.size() << std::endl;
    
    for (auto it = output.cbegin(); it != output.cend(); ++it)
    {
        fileHandler << (*it).id << " " << (*it).books.size() << std::endl;
        for (auto jt = (*it).books.cbegin(); jt != (*it).books.cend(); ++jt)
        {
            fileHandler << *jt << " ";
        }
        fileHandler << std::endl;
    }

    fileHandler.close();

    return 0;
}

/**
 * Sort libraries by increasing signup time.
 * 
 */
int strategy_01(Instance *inst, std::vector<Library>& output)
{
    long i, j;
    std::unordered_set<long> scannedBooks;

    // sort libraries by increasing signup time
    std::sort(inst->libraries.begin(), inst->libraries.end(), [] (Library const& l1, Library const& l2) -> bool { return l1.signup < l2.signup; });

    for (i = 0; i < (long)inst->libraries.size(); ++i)
    {
        // sort books by decreasing score in current library
        std::sort(inst->libraries[i].books.begin(), inst->libraries[i].books.end(), [inst] (long const& b1, long const& b2) -> bool { return inst->scores[b1] > inst->scores[b2]; });

        // initialise output library
        Library outLib;
        outLib.id = inst->libraries[i].id;

        for (j = 0; j < (long)inst->libraries[i].books.size(); ++j)
        {
            // put in output library only books not scanned
            if (scannedBooks.find(inst->libraries[i].books[j]) == scannedBooks.end())
            {
                outLib.books.push_back(inst->libraries[i].books[j]);
                scannedBooks.insert(inst->libraries[i].books[j]);
            }
        }

        output.push_back(outLib);
    }
    return 0;
}

int strategy_02(Instance *inst, std::vector<Library>& output)
{
    long iterCnt, day, i, j, left, limit, maxScore, posIndex;
    Library *pos;

    std::unordered_set<int> scannedBooks;

    std::printf("\n");
    std::printf("Days      = %8ld\n", inst->numDays);
    std::printf("Books     = %8ld\n", inst->numBooks);
    std::printf("Libraries = %8ld\n", inst->numLibraries);
    std::printf("%8s | %8s | %8s | %8s | %8s\n", "DAY", "DAYS LEF", "LIB ID", "LIB LEFT", "SCANNED");
    std::printf("----------------------------------------------------\n");

    day = 0;
    iterCnt = 0;
    while (inst->libraries.size() > 0)
    {
        left = inst->numDays - day;

        // Iterate through each library, update books removing
        // books already scanned and calculate scores (given the
        // remaining numbers of days left).
        // In the process, keep the library with maximum score.
        posIndex = -1;
        maxScore = 0;
        for (i = 0; i < (int)inst->libraries.size(); ++i)
        {
            // remove scanned books
            for (auto book = inst->libraries[i].books.cbegin(); book != inst->libraries[i].books.cend(); ++book)
            {
                if (scannedBooks.find(*book) != scannedBooks.end())
                {
                    inst->libraries[i].books.erase(book--);
                }
            }

            limit = (left - inst->libraries[i].signup) * inst->libraries[i].booksPerDay;

            // sort books by score
            std::sort(inst->libraries[i].books.begin(), inst->libraries[i].books.end(), [inst] (long const& b1, long const& b2) -> bool { return inst->scores[b1] > inst->scores[b2]; });

            // calculate total score for current library
            inst->libraries[i].score = 0;
            for (j = 0; (j < limit) && (j < (long)inst->libraries[i].books.size()); ++j)
            {
                inst->libraries[i].score += inst->scores[inst->libraries[i].books[j]];
            }

            // update maximum score library
            if (inst->libraries[i].score > maxScore)
            {
                maxScore = inst->libraries[i].score;
                posIndex = i;
                pos = &inst->libraries[i];
            }
        }

        if (posIndex < 0)
        {
            break;
        }

        if (iterCnt % 100 == 0)
        {
            std::printf("%8ld | %8ld | %8ld | %8lu | %8lu\n", day, left, (*pos).id, inst->libraries.size(), scannedBooks.size());
        }

        // update output structure
        day += (*pos).signup;
        limit = (left - (*pos).signup) * (*pos).booksPerDay;
        
        Library lib;
        lib.id = (*pos).id;
        lib.books = std::vector<long>(std::min(limit, (long)(*pos).books.size()));
        for (j = 0; (j < limit) && (j < (int)(*pos).books.size()); ++j)
        {
            lib.books[j] = (*pos).books[j];
        }

        output.push_back(lib);

        // update scanned books
        for (j = 0; (j < limit) && (j < (int)(*pos).books.size()); ++j)
        {
            scannedBooks.insert((*pos).books[j]);
        }

        // remove library
        inst->libraries.erase(inst->libraries.begin() + posIndex);

        iterCnt++;
    }

    return 0;
}

/**
 * Sort all books by decreasing score putting them in a heap.
 * Pop a book from the heap and find the library containing that book
 * with the minimum signup time.
 * Push the library in the output vector and update.
 */
int strategy_03(Instance *inst, std::vector<Library>& output)
{
    long left, day, i, j, book, minimum, pos, limit;

    std::vector<long> sortedBooks = std::vector<long>(inst->numBooks);
    std::unordered_set<long> scannedBooks;

    // initialise sorted books and sort by increasing scores
    // make access from last element
    for (i = 0; i < inst->numBooks; ++i)
    {
        sortedBooks[i] = i;
    }
    std::sort(sortedBooks.begin(), sortedBooks.end(), [inst](long const &b1, long const &b2) -> bool { return inst->scores[b1] < inst->scores[b2]; });

    day = 0;
    while ((sortedBooks.size() > 0) && (inst->libraries.size() > 0))
    {
        // get first book not already scanned
        while (sortedBooks.size() > 0)
        {
            book = sortedBooks.back();
            sortedBooks.pop_back();

            // book not already scanned: ok, stop
            if (scannedBooks.find(book) == scannedBooks.end())
            {
                break;
            }
        }

        // scan libraries and find the one containing book
        // with the lowest signup time
        minimum = 100000000;
        pos = -1;
        for (i = 0; i < (long)inst->libraries.size(); ++i)
        {
            // signup is lower and book found
            if ((inst->libraries[i].signup < minimum) && (std::find(inst->libraries[i].books.begin(), inst->libraries[i].books.end(), book) != inst->libraries[i].books.end()))
            {
                minimum = inst->libraries[i].signup;
                pos = i;
            }
        }

        if (pos < 0)
        {
            break;
        }

        // update output structure
        day += inst->libraries[pos].signup;
        left = inst->numDays - day;
        limit = left * inst->libraries[pos].booksPerDay;

        Library lib;
        lib.id = inst->libraries[pos].id;
        lib.books = std::vector<long>(std::min(limit, (long)inst->libraries[pos].books.size()));
        for (j = 0; j < (long)lib.books.size(); ++j)
        {
            lib.books[j] = inst->libraries[pos].books[j];
            scannedBooks.insert(inst->libraries[pos].books[j]);
        }

        output.push_back(lib);

        // remove library
        inst->libraries.erase(inst->libraries.begin() + pos);

    }

    return 0;
}