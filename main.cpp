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

int strategy_02(Instance *inst, std::vector<Library>& output);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Enter valid file path." << std::endl;
        return 1;
    }

    Instance inst;
    std::vector<Library> output;

    std::cout << "Reading instance from " << argv[1] << std::endl;
    read_input(&inst, argv[1]);
    
    std::cout << "Applying STRATEGY 02" << std::endl;
    strategy_02(&inst, output);

    std::cout << "Printing output" << std::endl;
    print_output(output, argv[1]);

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

int strategy_02(Instance *inst, std::vector<Library>& output)
{
    long iterCnt, d, i, j, left, limit, maxScore, posIndex;
    Library *pos;

    std::unordered_set<int> scannedBooks;

    std::printf("\nDays      = %8ld\n", inst->numDays);
    std::printf("Books     = %8ld\n", inst->numBooks);
    std::printf("Libraries = %8ld\n", inst->numLibraries);
    std::printf("%8s | %8s | %8s | %8s | %8s\n", "DAY", "DAYS LEF", "LIB ID", "LIB LEFT", "SCANNED");
    std::printf("----------------------------------------------------\n");

    d = 0;
    iterCnt = 0;
    //while ((d < inst->numDays) && (inst->libraries.size() > 0))
    while (inst->libraries.size() > 0)
    {
        left = inst->numDays - d;

        // iterate through each library, update books removing
        // books already scanned and calculate scores (given the
        // remaining numbers of days left)
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

            // sort books
            std::sort(inst->libraries[i].books.begin(), inst->libraries[i].books.end(), [inst] (int const& b1, int const& b2) -> bool { return inst->scores[b1] > inst->scores[b2]; });

            // calculate score
            inst->libraries[i].score = 0;
            for (int j = 0; (j < limit) && (j < (int)inst->libraries[i].books.size()); ++j)
            {
                inst->libraries[i].score += inst->scores[inst->libraries[i].books[j]];
            }

            // update maximum score
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
            std::printf("%8ld | %8ld | %8ld | %8lu | %8lu\n", d, left, (*pos).id, inst->libraries.size(), scannedBooks.size());
        }

        // update output structure
        d += (*pos).signup;
        limit = (left - (*pos).signup) * (*pos).booksPerDay;
        
        Library lib;
        lib.id = (*pos).id;
        //lib.books = std::vector<int>(std::min(limit, (int)(*pos).books.size()));
        for (j = 0; (j < limit) && (j < (int)(*pos).books.size()); ++j)
        {
            //lib.books[j] = (*pos).books[j];
            lib.books.push_back((*pos).books[j]);
        }

        output.push_back(lib);

        // update scanned books
        for (j = 0; (j < limit) && (j < (int)(*pos).books.size()); ++j)
        {
            scannedBooks.insert((*pos).books[j]);
        }

        //std::printf("8\n");
        //std::cout << " " << posIndex << " " << inst->libraries.size() << std::endl;
        // remove library
        inst->libraries.erase(inst->libraries.begin() + posIndex);
        //std::printf("9\n");
        iterCnt++;
    }

    return 0;
}