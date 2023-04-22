#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "computer_club.h"
#include "cc_exceptions.h"


int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cout << "ERROR: Incorrect number of arguments!\n";
        return -1;
    }
    
    clock_t t = clock();
    std::ifstream in;
    in.open(argv[1]);
    if (!in.is_open()) {
        std::cout << "ERROR: File hasn't opened!\n";
        return -1;
    }

    std::string tmp[3];

    std::ofstream output;
    ComputerClub* club;

    try{
        int i =0;
        std::getline(in, tmp[0]);
        if ((tmp[0].at(0) == '0') && (tmp[0].size() > 1)) throw CCExceptionIncorrectInput(tmp[0]);
        while (i < tmp[0].size()){
            if (!std::isdigit(tmp[0].at(i))) throw CCExceptionIncorrectInput(tmp[0]);
            i++;
        }
    
        std::getline(in, tmp[1]);
        std::getline(in, tmp[2]);
        if ((tmp[2].at(0) == '0') && (tmp[2].size() > 1)) throw CCExceptionIncorrectInput(tmp[2]);
        i = 0;
        while (i < tmp[2].size()){
            if (!std::isdigit(tmp[2].at(i))) throw CCExceptionIncorrectInput(tmp[2]);
            i++;
        }

        club = new ComputerClub(std::stoi(tmp[0]), tmp[1], std::stoi(tmp[2]));
    }
    catch(const CCExceptionIncorrectInput& ex) {
        std::cout << ex.what() << '\n';
        return -1;
    }

    try{
        output.open("output.txt");
        output << club->get_opening_time() << '\n';
        while(std::getline(in, tmp[0])) {
            output << tmp[0] << '\n';
            tmp[1] = club->read_event(tmp[0]);
            if(!tmp[1].empty()) output << tmp[1] << '\n';
        }
        tmp[1] = club->close();
        if(!tmp[1].empty()) output << tmp[1];
        output << club->get_closing_time() << '\n';
        output << club->get_seats_stats();
        output.close();
    }
    catch(CCExceptionIncorrectInput& ex){
        std::cout << ex.what() << '\n';
        output.close();
        output.open("output.txt", std::ofstream::trunc);
        output.close();
        return -1;
    }
    catch(...){
        std::cout << "Unknown error" <<"\n";
        output.close();
        output.open("output.txt", std::ofstream::trunc);
        output.close();
        return -1;
    }
    t = clock() - t;
    std::cout << "Program completed for " << ((double)t/CLOCKS_PER_SEC*1000000) << " mcs\n";

    return 0;
}