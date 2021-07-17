/*
Ethan Leet
s5223103
2801ICT Computing Algorithma
Assignment 2
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <iomanip>

class max_heap
{
    std::vector<int> nodes; // vector containing nodes of the heap
    int size() { return nodes.size(); } // size of the heap
    int parent(const int &i) { return (i - 1) / 2; } // parent node
    int left_child(const int &i) { return ((2 * i) + 1); } // left child
    int right_child(const int &i) { return ((2 * i) + 2); } // right child
    void shift_up(int i); // shift nodes up for scott
    void shift_down(const int &i); // shift nodes down for scott
    void rusty_shift_up(int i); // shift nodes up for rusty - uses get sum
    void rusty_shift_down(const int &i); // shift nodes down for rusty - uses get sum
    int get_sum(int i); // calculates sum of digits

public:
    max_heap() {}; // default constructor
    void insert(const int &i, const std::string name); // inserts nodes into the heap 
    int extract_max(const std::string name); // returns highest priority node
    ~max_heap() {} // default destructor
};

void max_heap::shift_up(int i)
{// checks if passed value is higher (based on integer value) than parent and swaps
    while (i > 0 && nodes[parent(i)] < nodes[i])
    {
        std::swap(nodes[parent(i)], nodes[i]);
        i = parent(i);
    }
}

void max_heap::shift_down(const int &i)
{// recursively checks left and right children to see if they are greater (based on integer value) than passed value and swaps
    int max_index = i;
    int l = left_child(i);
    int r = right_child(i);
    if (l <= size() && nodes[l] > nodes[max_index]) // checks left child against passed value
        max_index = l;
    if (r <= size() && nodes[r] > nodes[max_index]) // checks right child against passed value
        max_index = r;
    if (i != max_index)
    {// if passed value isnt max swap nodes
        std::swap(nodes[i], nodes[max_index]);
        shift_down(max_index); // recur 
    }
}

void max_heap::rusty_shift_up(int i)
{// checks if passed value is higher (based on sum of digits) than parent and swaps
 // also checks that rusty plays optimally
 // if get sum for parent is same as passed value it then compares integer value
    while (i > 0 && get_sum(nodes[parent(i)]) < get_sum(nodes[i]) || // check sum of digits
           i > 0 && get_sum(nodes[parent(i)]) == get_sum(nodes[i]) && // if equal sums
               nodes[parent(i)] < nodes[i]) // check integer value
    {
        std::swap(nodes[parent(i)], nodes[i]); // swap based upon above conditions
        i = parent(i);
    }
}

void max_heap::rusty_shift_down(const int &i)
{// recursively checks left and right children to see if they are greater (based on sum of digits) than the passed value and swaps
 // also checks that rusty plays optimally
 // if get sum for parent is same as passed value it then compares integer value
    int max_index = i;
    int l = left_child(i);
    int r = right_child(i);
    if (l <= size() && get_sum(nodes[l]) > get_sum(nodes[max_index]) || // check sum of digits for left child
        l <= size() && get_sum(nodes[l]) == get_sum(nodes[max_index]) && // if equal sums
            nodes[l] > nodes[max_index]) // check integer value
        max_index = l;
    if (r <= size() && get_sum(nodes[r]) > get_sum(nodes[max_index]) || // check sum of digits for right child
        r <= size() && get_sum(nodes[r]) == get_sum(nodes[max_index]) && // if equal sums
            nodes[r] > nodes[max_index]) // check integer value
        max_index = r;
    if (i != max_index)
    {// if passed value isnt max swap nodes
        std::swap(nodes[i], nodes[max_index]);
        rusty_shift_down(max_index); // recur
    }
}

int max_heap::get_sum(int i)
{// finds sum of digits for a passed value using modulus and division
    int sum = 0;
    while (i != 0)
    {// repeat until every digit checks
        sum = sum + i % 10; // get right most digit and add to sum
        i = i / 10;  // remove right most digit and repeat
    }
    return sum;
}

void max_heap::insert(const int &i, const std::string name)
{// inserts a node into the heap based on the name it is passed
    nodes.push_back(i);
    if (name == "scott") //  insert using shift up
        shift_up(int(nodes.size() - 1));
    else // insert using rusty shift up
        rusty_shift_up(int(nodes.size() - 1));
}

int max_heap::extract_max(const std::string name)
{// pop the highest priority node and shift down according to passed name
    int result = nodes[0]; // used to return thre result
    nodes[0] = nodes.back(); // move max to end of nodes
    nodes.pop_back(); // remove last item (max)
    if (name == "scott") //  shift down for scott
        shift_down(0); // heap is now out of order, re-heap
    else // shift down for rusty
        rusty_shift_down(0); // heap is now out of order, re-heap
    return result;
}

void play_game(max_heap rusty, max_heap scott, std::unordered_map<int, int> chosen,
               int num_balls, int num_turns, bool toss)
{// this function is the main algorithm for the program and performs all the function calls
 // this function is being timed for the output
    long long int scott_score = 0, rusty_score = 0; // long long ints so problems 3, 6 and 9 don't overflow
    int rusty_choice, scott_choice; // Used for extracting max and searching the unordered map
    while (num_balls > 0)
    {// loop until no balls left
        if (toss)
        { // boolean check to see who goes first
          // this block is rusty
            int turn = 0; // initialise a turn counter
            while (turn < num_turns && num_balls > 0)
            {// loop until rusty has had his turns for this round
                rusty_choice = rusty.extract_max("rusty");
                auto it = chosen.find(rusty_choice); // look for rustys choice in the unordered map
                while (it->second == 0)
                {// if the value is 0 (meaning he can not pick that key) pick again until allowed ball is picked
                    rusty_choice = rusty.extract_max("rusty");
                    it = chosen.find(rusty_choice);
                }
                it->second--; // reduce that keys value
                rusty_score += rusty_choice; // increment score
                turn++; // increase turn counter 
                num_balls--; // decrease number of balls left to play
            }
        }
        else
        {// this block is scott
            int turn = 0; // initialise a turn counter
            while (turn < num_turns && num_balls > 0)
            {// loop until scott has had his turns for this round
                scott_choice = scott.extract_max("scott");
                auto it = chosen.find(scott_choice); // look for scotts choice in the unordered map
                while (it->second == 0)
                {// if the value is 0 (meaning he can not pick that key) pick again until allowed ball is picked
                    scott_choice = scott.extract_max("scott");
                    it = chosen.find(scott_choice);
                }
                it->second--; // reduce that keys value
                scott_score += scott_choice; // increment score
                turn++; // increase turn counter 
                num_balls--; // decrease number of balls left to play
            }
        }
        toss = !toss; // switch turns by negating the boolean
    }
    std::cout << "Scott Score = " << scott_score << "\nRusty Score = " << rusty_score << std::endl;
}

int main(int argc, char *argv[])
{
    if (argv[1] == nullptr)
    {// this throws an error and exists if no filename is passed when executing the program
        std::cerr << "Run from command line with filename as arguement" << std::endl;
        return 0;
    }
    int test_cases; // initialise test cases variable
    std::ifstream inputFile(argv[1]); // read filename from command line
    std::string test_cases_line; // string used to record value for test cases
    getline(inputFile, test_cases_line); // read in test cases
    test_cases = stoi(test_cases_line); // save as integer
    while (test_cases > 0)
    {// below block is driving code to save variables and call the play game function
     // it runs until there are no more test cases left
        auto start = std::chrono::system_clock::now(); // start clock
        bool toss; // initialise boolean for the toss (tails = true, heads = false)
        int num_balls, num_turns, val; // initialise the number of balls, turns and values of each ball
        std::vector<int> amounts; // store each ball and its value
        std::unordered_map<int, int> chosen; // key = value of ball, value = number of times it appears in amounts vector
        max_heap scott, rusty; // create max_heaps for each player
        std::string first_line, second_line, third_line; // initialise strings for getline to read values
        getline(inputFile, first_line); // read first line of game
        std::istringstream line_one(first_line); 
        line_one >> num_balls; // assign first value to number of balls
        line_one >> num_turns; // assign second value to number of turns
        getline(inputFile, second_line); // read second line of game
        std::istringstream line_two(second_line);
        std::string buffer; // string to read the value on each pass of second line
        while (line_two >> buffer)
        {
            val = stoi(buffer); // convert buffer to int
            amounts.push_back(val); // push to amounts vector
        }
        for (auto it = amounts.begin(); it != amounts.end(); ++it)
        {// save each item in amounts to map
            int count = 0;
            int it_count = std::count(amounts.begin(), amounts.end(), (*it)); // find how many times each ball appears
            chosen.insert({*it, it_count}); // insert key (value of ball) and value (amount of times key appears) into map
            count++;
            scott.insert(*it, "scott"); // insert each ball into scotts heap
            rusty.insert(*it, "rusty"); // insert each ball into rustys heap
        }
        getline(inputFile, third_line); // read final line for game
        std::istringstream line_three(third_line);
        line_three >> buffer;
        toss = buffer == "TAILS"; // initialise boolean toss
        std::cout << "Test Case " << abs(test_cases - 10) + 1<< std::endl; // print current test case
        play_game(rusty, scott, chosen, num_balls, num_turns, toss); // play the game with this loops game details
        auto elapsed = std::chrono::system_clock::now() - start; // end clock
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count(); // work out microseconds clock ran
        std::cout << "CPU Time (secs): " << std::setprecision(6) << microseconds / 1e+6 << "\n\n"; // print total time converted to seconds
        test_cases--; // decrease test case for next iteration of loop
    }
    return 0;
}
