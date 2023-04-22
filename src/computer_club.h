#ifndef _COMPUTER_CLUB_
#define _COMPUTER_CLUB_

#include <vector>
#include <string>
#include <list>
#include <set>
#include <sstream>

class ComputerClub {
private: 
    class Seat {
    private:
        std::string client;
        uint16_t earnings;
        uint16_t total_minutes;
        std::pair<uint8_t, uint8_t> start_using;
    public:
        Seat(); //+
        void remove_client(int, int, int);
        void set_client(const std::string&, int, int);
        std::string get_client();
        uint16_t get_earnings();
        uint16_t get_time();
    };
    
    enum class Events {Come = 1, TakeSeat = 2, Wait = 3, Leave = 4};
    enum class EventsOut {Leave = 11, TakeSeat = 12, Error = 13};
    std::pair<uint8_t, uint8_t> opening_time;
    std::pair<uint8_t, uint8_t> closing_time;
    std::pair<int8_t, int8_t> prev_event_time;
    uint16_t price;
    std::vector<Seat> seats;
    std::list<std::string> queue;

    std::string set_client(std::string, int, int, int);
    std::string make_msg(int, int, EventsOut, std::string, int table = 0);
    bool hasFree();
    bool client_in_queue(std::string);
    int client_on_seat(std::string);
public:
    std::string get_opening_time();
    std::string get_seats_stats();
    std::string get_closing_time();
    std::string close();
    ComputerClub(int, const std::string&, int); //+
    std::string read_event(const std::string&);
};

#endif