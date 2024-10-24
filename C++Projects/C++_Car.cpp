#include <iostream>
using namespace std;

class MyCar {
    public:
    int year; // repersents year car was produced
    string make; // repersents make of car
    string model; // repersents model of car

    void whatAmIDriving(){
        cout <<  "I am driving a " << year << " " << make << " " <<model << endl;
    }
};

int main(){

    MyCar car;
    
    car.year = 1994;
    car.make = "Porche";
    car.model = "911 Turbo";

    car.whatAmIDriving();
        
    return 0;

}