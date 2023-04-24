#include <fstream>
#include <iostream>
#include <sstream>
#include "computer_club.h"
#include "cc_exceptions.h"


int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cout << "ERROR: Incorrect number of arguments!\n";
        return -1;
    }
    
    
    std::ifstream in;
    in.open(argv[1]);
    if (!in.is_open()) {
        std::cout << "ERROR: File hasn't opened!\n";
        return -1;
    }

    std::string tmp[3];
    std::stringstream s;
    ComputerClub* club;
    try{
        std::getline(in, tmp[0]);
        if ((tmp[0].at(0) == '0') && (tmp[0].size() > 1)) throw CCExceptionIncorrectInput(tmp[0]);
        for (int i = 0; i < tmp[0].size(); i++){ if (!std::isdigit(tmp[0].at(i))) throw CCExceptionIncorrectInput(tmp[0]); }
    
        std::getline(in, tmp[1]);
        std::getline(in, tmp[2]);
        if ((tmp[2].at(0) == '0') && (tmp[2].size() > 1)) throw CCExceptionIncorrectInput(tmp[2]);
        for (int i = 0; i < tmp[2].size(); i++){ if (!std::isdigit(tmp[2].at(i))) throw CCExceptionIncorrectInput(tmp[2]); }

        club = new ComputerClub(std::stoi(tmp[0]), tmp[1], std::stoi(tmp[2]));
    }
    catch(const CCExceptionIncorrectInput& ex) {
        std::cout << ex.what() << '\n';
        return -1;
    }

    try{
        s << club->get_opening_time() << '\n';
        while(std::getline(in, tmp[0])) {
            s << tmp[0] << '\n';
            tmp[1] = club->read_event(tmp[0]);
            if(!tmp[1].empty()) {
                s << tmp[1] << '\n';
            }
        }
        tmp[1] = club->close();
        if(!tmp[1].empty()) {
            s << tmp[1];
        }
        s << club->get_closing_time() << '\n';
        s << club->get_seats_stats();
        std::cout << s.str() << '\n';
    }
    catch(CCExceptionIncorrectInput& ex){
        std::cout << ex.what() << '\n';
        return -1;
    }
    catch(...){
        std::cout << "Unknown error" <<"\n";
        return -1;
    }
    return 0;
}