#include "computer_club.h"
#include "cc_exceptions.h"

ComputerClub::Seat::Seat(){
    client = "EMPTY";
    earnings = 0;
    total_minutes = 0;
    start_using = std::make_pair(0,0);
}

std::string ComputerClub::Seat::get_client(){return client;}
uint16_t ComputerClub::Seat::get_earnings() {return earnings;}
uint16_t ComputerClub::Seat::get_time() {return total_minutes;}

void ComputerClub::Seat::set_client(const std::string& client, int hours, int minutes){
    this->client = client;
    start_using = std::make_pair(hours, minutes);
}

void ComputerClub::Seat::remove_client(int hours, int minutes, int price) {
    client = "EMPTY";
    total_minutes = total_minutes + hours*60+minutes - (start_using.first*60+start_using.second);
    if ((hours*60+minutes - (start_using.first*60+start_using.second) % 60) != 0)  
        earnings += ((hours*60+minutes - (start_using.first*60+start_using.second))/60+1)*price;
    else earnings += ((hours*60+minutes - (start_using.first*60+start_using.second))/60)*price;
}



ComputerClub::ComputerClub(int seats_num, const std::string& time, int price) {
    if (time.size() != 11) throw CCExceptionIncorrectInput(time);

    for (int i = 0; i < 11; i++){
        if ((i == 2) || (i == 8)) {
            if (time.at(i) != ':') throw CCExceptionIncorrectInput(time);
            continue;
        }
        if (i == 5) {
            if (time.at(i) != ' ') throw CCExceptionIncorrectInput(time);
            continue;
        }
        if (!std::isdigit(time.at(i))) throw CCExceptionIncorrectInput(time);
    }
    int hours;
    int minutes;
    std::string tmp;
    std::stringstream s;
    s << time;

    std::getline(s, tmp, ':');
    hours = std::stoi(tmp);
    if ((hours > 23) || (hours < 0)) throw CCExceptionIncorrectInput(time);
    std::getline(s, tmp, ' ');
    minutes = std::stoi(tmp);
    if ((minutes > 59) || (minutes < 0)) throw CCExceptionIncorrectInput(time);

    opening_time = std::make_pair(hours, minutes);
    prev_event_time = std::make_pair(-1, -1);

    std::getline(s, tmp, ':');
    hours = std::stoi(tmp);
    if ((hours > 23) || (hours < 0)) throw CCExceptionIncorrectInput(time);
    std::getline(s, tmp);
    minutes = std::stoi(tmp);
    if ((minutes > 59) || (minutes < 0)) throw CCExceptionIncorrectInput(time);

    closing_time = std::make_pair(hours, minutes);

    if (seats_num < 1) throw CCExceptionIncorrectInput(time);
    if (price < 1) throw CCExceptionIncorrectInput(time);

    this->price = price;
    seats = std::vector<Seat>(seats_num);
}

std::string ComputerClub::set_client(std::string client, int table, int hours, int minutes){
    int tmp = client_on_seat(client);
    if ((seats[table].get_client() != "EMPTY") || (tmp == table)) return make_msg(hours, minutes, EventsOut::Error, std::string("PlaceIsBusy"));
    if (tmp != -1) {
        seats[tmp].remove_client(hours, minutes, price);
        seats[table].set_client(client, hours, minutes);
        return "";
    }
    else {
        queue.remove(client);
        seats[table].set_client(client, hours, minutes);
        return "";
    }
}

bool ComputerClub::hasFree(){
    for (int i = 0; i < seats.size(); i++) {
        if (seats[i].get_client() == "EMPTY") return true;
    }
    return false;
}

bool ComputerClub::client_in_queue(std::string client){
    for (std::string queue_client : queue){
        if (queue_client == client) return true;
    }
    return false;
}

int ComputerClub::client_on_seat(std::string client){
    for (int i = 0; i < seats.size(); i++) {
        if (seats[i].get_client() == client) return i+1;
    }
    return -1;
}

std::string ComputerClub::make_msg(int hours, int minutes, EventsOut id, std::string msg, int table) {
    std::stringstream s;
    if (hours < 10) s << '0';
    s << hours << ':';
    if (minutes < 10) s << '0';
    s << minutes << ' ' << (int)id << ' ' << msg;
    if (table > 0) { s << ' ' << table;}
    return s.str();
}

std::string ComputerClub::get_opening_time() {
    std::stringstream s;
    if (opening_time.first < 10) s << '0';
    s << (int)opening_time.first << ':';
    if (opening_time.second < 10) s << '0';
    s << (int)opening_time.second;
    return s.str();
}

std::string ComputerClub::get_closing_time() {
    std::stringstream s;
    if (closing_time.first < 10) s << '0';
    s << (int)closing_time.first << ':';
    if (closing_time.second < 10) s << '0';
    s << (int)closing_time.second;
    return s.str();
}

std::string ComputerClub::read_event(const std::string& event) {
    if (event.size() < 9) throw CCExceptionIncorrectInput(event);
    for (int i = 0; i < 5; i++){
        if (i == 2) {
            if (event.at(i) != ':') throw CCExceptionIncorrectInput(event);
            continue;
        }
        if (i == 5) {
            if (event.at(i) != ' ') throw CCExceptionIncorrectInput(event);
            continue;
        }
        if ((event.at(i) < '0') || (event.at(i) > '9')) throw CCExceptionIncorrectInput(event);
    }

    int hours, minutes, id, seat = 0;
    std::string tmp, client;
    std::stringstream s;
    s << event;
    
    std::getline(s, tmp, ':');
    hours = std::stoi(tmp);
    if ((hours < 0)|| (hours > 23)) throw CCExceptionIncorrectInput(event);

    std::getline(s, tmp, ' ');
    minutes = std::stoi(tmp);
    if ((minutes > 59) || (minutes < 0)) throw CCExceptionIncorrectInput(event);
    if (hours < prev_event_time.first) throw CCExceptionIncorrectInput(event);
    else if ((hours == prev_event_time.first) && (minutes < prev_event_time.second)) throw CCExceptionIncorrectInput(event);
    
    std::getline(s, tmp, ' ');
    id = std::stoi(tmp);

    std::getline(s, client, ' ');
    for (int i = 0; i < client.size(); i++){
        if (((client.at(i) < 'a') || (client.at(i) > 'z')) && ((client.at(i) < '0') || (client.at(i) > '9'))
            && ((client.at(i) != '_') && (client.at(i) != '-'))) throw CCExceptionIncorrectInput(event);
    }
    if (s.tellg() != -1) {
        std::getline(s, tmp, ' ');
        seat = std::stoi(tmp);
    }

    if ((seat < 0) || (seat > seats.size())) throw CCExceptionIncorrectInput(event);


    switch(id){
        case (int)Events::Come:
            if ((hours < opening_time.first) || ((hours = opening_time.first)&& (minutes < opening_time.second)))
                return make_msg(hours, minutes, EventsOut::Error, std::string("NotOpenYet"));
            
            if (client_in_queue(client) || (client_on_seat(client) != -1)) return make_msg(hours, minutes, EventsOut::Error, std::string("YouShallNotPass"));

            queue.push_back(client);
            return "";
        

        case (int)Events::TakeSeat:
            if (!client_in_queue(client) && (client_on_seat(client) == -1)) return make_msg(hours, minutes, EventsOut::Error, std::string("ClientUnknown"));
            return set_client(client, seat-1, hours, minutes);
        
        
        case (int)Events::Wait:
            if (!client_in_queue(client) && (client_on_seat(client) == -1)) return make_msg(hours, minutes, EventsOut::Error, std::string("ClientUnknown"));
            if (queue.size() > seats.size()) {
                queue.remove(client);
                return make_msg(hours, minutes, EventsOut::Leave, client);
            }
            if (hasFree()) return make_msg(hours, minutes, EventsOut::Error, std::string("ICanWaitNoLonger!"));
            else return "";
        
        
        case (int)Events::Leave:
        {
            int tmp = client_on_seat(client);
            if (!client_in_queue(client) && (tmp == -1)) return make_msg(hours, minutes, EventsOut::Error, std::string("ClientUnknown"));
            seats[tmp-1].remove_client(hours, minutes, price);

            if (!queue.empty())  {
                std::string tmp1 = std::string(queue.front());
                queue.pop_front();
                set_client(tmp1, tmp-1, hours, minutes);
                return make_msg(hours, minutes, EventsOut::TakeSeat, tmp1);
            }
            else return "";
        }
        default:
            throw CCExceptionIncorrectInput(event);
            break;
    }
}

std::string ComputerClub::close(){
    std::set<std::string> clients;
    std::string tmp = "";
    for (ComputerClub::Seat s : seats) {
        if (s.get_client() != "EMPTY") clients.insert(s.get_client());
    }
    for (std::string str : queue) clients.insert(str);
    queue.clear();

    for (std::string client : clients) {
        int i = client_on_seat(client);
        if (i != -1) seats[i-1].remove_client(closing_time.first, closing_time.second, price);
        tmp = tmp + make_msg(closing_time.first, closing_time.second, EventsOut::Leave, client) + '\n';
    }
    prev_event_time = std::make_pair(24, 60);
    return tmp;
}

std::string ComputerClub::get_seats_stats(){
    std::stringstream s;
    for (int i = 0; i < seats.size(); i++){
        s << i+1 << ' ' << seats[i].get_earnings() << ' ';
        int tmp = seats[i].get_time();
        if ((tmp/60) < 10) s << '0';
        s << (tmp/60) << ':';
        if ((tmp%60) < 10) s << '0';
        s << (tmp%60);
        if (i != (seats.size()-1)) s << '\n';
    }
    return s.str();
}