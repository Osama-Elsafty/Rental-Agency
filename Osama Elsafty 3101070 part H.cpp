#include<iostream>
#include<iomanip>
using namespace std;

class DayOfYear {//tested
private:
    short unsigned int dayOfYear; //e.g: 31 of december has the number 365 as a value of dayOfYear
    short unsigned int year; // storing the year
    const static int days[12]; // storing the number of days passed from the year until the end each month
public:
    DayOfYear(int a=1, int b=2021 ) : dayOfYear(a), year(b){
    };
    //default constructor

    DayOfYear(int dd,int mm,int yy) :  year(yy),  dayOfYear(days[mm-1]+dd){
    };
    //overloaded constructor

int get_dayOfYear(){
    return dayOfYear;}
//returning dayOfYear

void operator ++ ( ){

    if (dayOfYear==365)
    {
        dayOfYear=1;
        year ++;
    }
          else
            dayOfYear++;
}
// moving to next day

friend istream& operator >> (istream& in, DayOfYear& ourDay){
    int inYear,inMonth,inDay;
    char dash;
    in>>inYear>>dash>>inMonth>>dash>>inDay;
    ourDay= DayOfYear (inDay , inMonth , inYear);
    return in;
}
 // inputing a date in the form yyyy-mm-dd

friend ostream& operator << (ostream& out, const DayOfYear& ourDay){

    int i;
   for(i=1; i<12; i++){
        if(ourDay.dayOfYear < days[i])
            break;
   }

    out<<ourDay.year<< "-"<<setfill('0')<<setw(2)<<i<<"-"<<setfill('0')<<setw(2)<<ourDay.dayOfYear-days[i-1];
    return out;
}
// printing a date in the form yyyy-mm-dd
int get_year () // to control the user input
{
    return year;
}
};
//end of class DayOfYear

class Vehicle {//tested
private:
    const int no; //storing a unique number for each vehicle
    string model; // storing the model of the vehicle
    float price24h; // storing the rental price of one day
public:
    Vehicle(int vehicle_num, string vehicle_model, float vehicle_price_per_day)
         :no(vehicle_num), model(vehicle_model), price24h(vehicle_price_per_day) {
    };
    // constructor

    virtual ~Vehicle(){
        // The { } are just to keep the shape of the code
        };
    // virtual destructor

    int get_no(){
        return no;}
    // returning no

    string get_model(){
        return model;}
    //returning model

    float get_price(int numOfDays){
        return numOfDays*price24h;}
    // return the price of rent for a given duration

    virtual void print()=0;
    //pure virtual function
    };
//end of class Vehicle

class Bike : public Vehicle {//tested
public:
        Bike (int bike_num, string bike_model): Vehicle(bike_num, bike_model, 9.99){
        }
        //constructor

        virtual void print(){
            cout<<get_no()<<": "<<get_model()<<" (Bike)"<<endl;
        }
        //overloading the pure function of the base class
};
//end of class Bike

class EMotorVehicle : public Vehicle{//tested
public:
    EMotorVehicle(int Emotor_num, string Emotor_model , float Emotor_price_per_day)
    : Vehicle(Emotor_num, Emotor_model, Emotor_price_per_day) {
    };
    // constructor

    virtual bool is_streetLegal ()=0;
    // Pure virtual function

     virtual void print(){
             cout<<get_no()<<": " <<get_model();
            if (!is_streetLegal())
                cout<<"(not street legal)";

        }
    //virtual function

};
//end of class EMotorVehicle

class Ebike : public EMotorVehicle{//tested
public:
    Ebike (int ebike_num, string ebike_model): EMotorVehicle(ebike_num, ebike_model,29.99){
    }
    //constructor

    virtual bool is_streetLegal(){
        return true;}//returns true if the Ebike is street legal
        //overloading the pure function of the base class

    virtual void print(){
        EMotorVehicle::print(); //writing print() only leads to an open loop
        cout<<" (EBike)"<<endl;
    }
    //overloading the function of the base class
};
//end of class Ebike

class EScooter: public EMotorVehicle{//tested
private:
    bool streetLegal; //to store wheather the Escooter is street legal
public:
    EScooter (int escooter_num, string escooter_model)
    : EMotorVehicle(escooter_num, escooter_model,19.99), streetLegal(false){
        // all Escooters are not street legal
    }
    //Constructor

    virtual bool is_streetLegal(){
        return streetLegal;}
    //overloading the pure function of the base class

     virtual void print(){
        EMotorVehicle::print();
        cout<<" (EScooter)"<<endl;
    }
    //overloading the pure function of the base class

};
//end of class EScooter

class Rental {//tested
private:
    const int no; //storing a unique reservation number for each rental
    DayOfYear from; //storing the starting day of rental
    int days; // storing the duration of Rental
    string customer; //storing the name of customer
    static int last_no; //to keep the reservation numbers of all Rentals in sequence
public:

    Rental(string person_name, DayOfYear starting_day, int num_of_days=1 ) :
        customer(person_name) , from(starting_day), days(num_of_days),no(last_no++){
            // the incrementation of last_no happens after storing the current value inside no
    };
    //constructor

    int get_days(){
        return days;}
        // Returning the number of rental days

    DayOfYear get_from() {
        return from;}
    // returning the starting day of rental

    DayOfYear get_until(){
        DayOfYear to=from;
        for(int i=0; i<days ; i++)
            ++to;
        return to;
    }
    //returning the last day of rental

    void print(){
        cout<<from<<" to "<< get_until() <<", rental no. "<<no<<", for: "<<customer;
    }
    //prints the Rental details

};
//end of class Rental

class Schedule {//tested
private:
    Vehicle * vehicle; // pointer to the Vehicle that this Schedule belongs to
    Rental * year[365];// stores  a pointer to every day that has rental for maximum one year ahead (will point to NULL when no Rental on that day)
    Schedule *next; // to make a list of several vehicles to be rented each one of them has its own Schedule and points to the next one's Schedule
public:
    Schedule (Vehicle *vehicle_to_point_to): vehicle(vehicle_to_point_to)
    {
        for(int i=0; i<365; i++)
        year[i]=NULL;
        next=NULL;
    };
    // constructor
    //assigns the Schedule to a specific given vehicle and sets all other pointers to NULL

    Vehicle* get_vehicle(){
        return vehicle;}
        //returning a pointer to the vehicle

    Schedule* get_next(){
        return next;}
    //returning a pointer to the next schedule

    void  set_next(Schedule* next_schedule){
        next=next_schedule;}
         //making the pointer next points to the next vehicle's Schedule

    bool isFree(DayOfYear day_to_start, int duration){
        if(duration>28)
        {
            duration=28;
            cout<<"duration has been set to the max duration is (28 days) "<<endl;
        }
        //max rental duration is 28 days as mentioned in the question
        int i=day_to_start.get_dayOfYear();
        int rep=i+duration; //till which day should the function check
        for(i; i<rep; i++)
        {
            if(year[i]!=NULL)
                return false;
        }
    return true;

    }
    //checks whether the vehicle is free in a qiven period (

    float book (string person, DayOfYear starting_date, int duration ){

        //max rental duration is 28 days as mentioned in the question
        year[starting_date.get_dayOfYear()-1]=new Rental(person, starting_date, duration);
        int i=starting_date.get_dayOfYear();
        int rep=i+duration;
        for(i;i<rep; i++ )
            year[i]=year[starting_date.get_dayOfYear()-1];
      //using this method we only make one new Rental and make all wanted pointers point to it
      // putting the declaration of new Rental in the for loop would make many new identical Rentals(more memory)
        return vehicle->get_price(duration);
    }
    // stores an address to anew Rental in the arrays year in the indeces corresponding to Rental days
    // returns the Rental price

    void print(DayOfYear one_day){
        vehicle->print();
        if(year[one_day.get_dayOfYear()]!=NULL)
        {
          year[one_day.get_dayOfYear()]->print();
            cout<<", total: "<<vehicle->get_price(year[one_day.get_dayOfYear()]->get_days())<<endl;
        }
    }
    //prints the Rental of a specific day(if exist)

    void print(){
        cout<<"SCHEDULE FOR ";
        vehicle->print();
       int i=0;
       while(i<365)
       {
            if(year[i]==NULL)
            i++;//check next day
            else
            {
                year[i]->print();
                cout<<", total: "<<vehicle->get_price(year[i]->get_days())<<endl;

                i+=year[i]->get_days()+1; //print each rental one time only and then jump to the index next to last day of this rental

            }

       }

    }
    //prints the Rental of the whole year
};
//end of class Schedule

class Agency{//tested
private:
    float profitPercent; //storing the profit percentage of the agency for each rental
    float profit; //storing the summed up profit of the agency
    Schedule* head;//pointer to the first element of a list of schedules for all the vehicles of the agency.
    Schedule* last;//pointer to the last element of the list.
public:

    Agency(): profitPercent(0.20), profit(0.00), head(NULL), last(NULL){
    }
    //constructor

    float get_profit(){
    return profit;}
    //returning profit

    void set_profitPercentage(float set_percentage_to){
        profitPercent=set_percentage_to/100.00;
    }
    //to change the profit percent

    void add(Vehicle* add_new){//tested
        if(head==NULL)
        {
            head=new Schedule(add_new);
            last=head;
        }
        else
        {
            last->set_next(new Schedule(add_new));
            last=last->get_next();

        }

    }
    // assigning a Schedule to the given Vehicle
    //setting it to be the last Schedule

     bool isFree(int vehicle_number, DayOfYear start_day, int duration){
    return searchFor(vehicle_number)->isFree(start_day,duration);
   }
   //checks whether the vehicle which has the no number is free in a specific duration

   float book(int vehicle_number, string customer, DayOfYear start_day, int duration){
        float price= searchFor(vehicle_number)->book(customer, start_day, duration);
        profit+=price*profitPercent;
        return price;
   }
   // makes a new booking and returns its price

   int chooseVehicle (){ //tested
       int chosen;
       if(head!=NULL)
       {
           Schedule* pointer=head;
            pointer->print();//to print the head
           while(pointer!=last)
           {
               pointer=pointer->get_next(); //move the pointer to next
               pointer->print();//print the next

           }
       } // previous lines are the same as print() but we don't want to show the Profit here
       cout<<"choose vehicle no:";
        cin>>chosen;
        return chosen;
   }
   // let the user choose a vehicle from all

   void print(DayOfYear date){//tested

        if(head!=NULL)
       {
           Schedule* pointer=head;
            pointer->print(date);//to print the head
           while(pointer!=last)
           {
               pointer=pointer->get_next();
               pointer->print(date);

           }
       }
       else
        cout<<"SCHEDULE FOR "<<date<<endl;
   }
   // print all Schedules of a day

   void print(){ //tested
       if(head!=NULL)
       {
           Schedule* pointer=head;
            pointer->print();//to print the head
           while(pointer!=last)
           {
               pointer=pointer->get_next();
               pointer->print();

           }
       }
       cout<<"profit: "<<profit<<" EUR"<<endl; // if head=NULL then it is empty,skip the f body only print the profit
   }
   // print all Schedules and the profit


protected:
   Schedule* searchFor(int num){//tested
       Schedule* pointer=head;
       while(pointer!=NULL)
       {
            if(pointer->get_vehicle()->get_no()==num)
                return pointer;
           pointer=pointer->get_next();
       }
       return NULL;
   }
   //searh for a Vehicle with no=mum
};
//end of class Agency

int Rental::last_no=1; // Defining a static int from the class Rental
const int DayOfYear::days[12]={0,31,59,90,120,151,181,212,243,273,304,334}; //Defining a static int from the class DayOfYear

int main(void)
{
    cout<<fixed<<setprecision(2);
   char choice;
    Agency My_Agency;
    DayOfYear today;
    DayOfYear tomorrow;
    int vehicles_num; // to take the users input in part c
    int duration; // to take the users input in part c
    string customer;// to take the users input in part c
    DayOfYear day_of_rent; // to take the users input in part c
    int add=0,total=0; //to control the sequence of the vehicles
    int a=1,b=1,c=1; //to control the models

   while(1)
   {
    tomorrow=today;
    ++tomorrow;
        cout<<"________________________________________"<<endl
        <<"MENUE"<<endl
        <<"A end of simulation"<<endl //tested
        <<"B set new simulation date"<<endl //tested
        <<"C new rental manually"<<endl //tested
        <<"D print rental of today "<<today<<endl //tested
        <<"E print rental of tomorrow "<<tomorrow<<endl //tested
        <<"F print all rentals"<<endl //tested
        <<"G print agency profit"<<endl //tested
        <<"H set agency profit percent"<<endl //tested
        <<"I add bikes"<<endl //tested
        <<"J add E-bikes"<<endl//tested
        <<"K add E-scooters"<<endl//tested
        <<"l new rental simulation"<<endl//tested
        <<"your choice: ";
        cin>> choice;
        switch(choice)
            {

            case 'a':
            case 'A':
                return 0;
                break;//end of a
            //-------------------------------------------------------------------

            case 'b':
            case 'B':
                cout<<"input date of today:";
                cin>>today;
                break; //end of b
            //-------------------------------------------------------------------

            case 'c':
            case 'C':
                if(total !=0)// cant choose from nothing!
                {
                   vehicles_num= My_Agency.chooseVehicle(); // user input will be taken
                cout<<"day of rental:";
                cin>>day_of_rent;
                int days_allowed; //to control the max duration
                if(!(day_of_rent.get_year()==today.get_year() && day_of_rent.get_dayOfYear()>=today.get_dayOfYear()
                || day_of_rent.get_year()==today.get_year()+1 && day_of_rent.get_dayOfYear()<(today.get_dayOfYear()-1) )) //check if the day_of_rent is within a year
                {
                        cout<<"only bookings within the upcoming year are allowed "<<endl;
                    break;

                } //  break and don't allow to book if the user gave a date which is not in the upcoming year

                if(day_of_rent.get_year()== (today.get_year()+1) ) //when booking something next year (today's value as a day > day of the rent)
                    days_allowed= (today.get_dayOfYear()-1) - day_of_rent.get_dayOfYear(); //today - rent date -1
                else // when booking something this year
                    days_allowed=365-day_of_rent.get_dayOfYear();

                if (days_allowed>28) //max allowed is 28
                    days_allowed=28;

               // cout<<"days allowed " <<days_allowed<<endl; //activate to test the input booking control
                cout<<"number of days:";
                cin>>duration;
                if(duration>days_allowed)
                {
                    cout<<"only bookings within the upcoming year and for maximum 28 days are allowed "<<endl;
                    break;
                }

                if(My_Agency.isFree(vehicles_num,day_of_rent,duration))//check first is it is free
                   {
                    cout<<"input data vehicle renter name:";
                     cin>>customer;
                    cout<<"Booked,price for this Rental: "<<My_Agency.book(vehicles_num,customer,day_of_rent,duration)<<endl;
                   }
                   else
                    cout<<"sorry vehicle already booked"<<endl;
                     break;//end of c
                }
                 else
                    cout<<"no Vehicles to be rented "<<endl;
                 break;//end of c
            //-------------------------------------------------------------------

            case 'd':
            case 'D':
               My_Agency.print(today);
                break;//end of d
            //-------------------------------------------------------------------

            case 'e':
            case 'E':
                My_Agency.print(tomorrow);
                break;//end of e
            //-------------------------------------------------------------------

            case 'f':
            case 'F':
                My_Agency.print();
                break;//end of f
            //-------------------------------------------------------------------

            case 'g':
            case 'G':
                cout<<"agency profit: "<<My_Agency.get_profit()<<" EUR"<<endl;
                break;//end of g
            //-------------------------------------------------------------------

            case 'h':
            case 'H':
                cout<<"agency profit percent:";
                float set_percent_to;
                cin>>set_percent_to;
                My_Agency.set_profitPercentage(set_percent_to);
                break;//end of h
            //-------------------------------------------------------------------

            case 'i':
            case 'I':
                cout<<"how many Bikes in this simulation? ";
                cin>>add;
                for(int i=total+1; i<=(add+total); i++)
                {
                    My_Agency.add(new Bike(i,"City"+to_string(a)));
                    a++;
                }
                total+=add;
                break;//end of i
            //-------------------------------------------------------------------

            case 'j':
            case 'J':
                cout<<"how many EBikes in this simulation? ";
                cin>>add;
                for(int i=total+1; i<=(add+total); i++)
                {
                    My_Agency.add(new Ebike(i,"Trekky"+to_string(b)));
                    b++;
                }
                 total+=add;
                break;//end of j
            //-------------------------------------------------------------------

            case 'k':
            case 'K':
                cout<<"how many EScooters in this simulation? ";
                cin>>add;
                for(int i=total+1; i<=(add+total); i++)
                {
                    My_Agency.add(new EScooter(i,"Scooty"+to_string(c)));
                    c++;
                }
                 total+=add;
                break;//end of k
            //-------------------------------------------------------------------

            case 'l':
            case 'L':
                 if(total !=0) //can't choose from nothing!
                {
                    cout<<"how many reservations to simulate? ";
                    int seq;
                    cin>>seq;
                    srand(seq);
                    for(seq; seq>=1; seq--)
                    {
                        DayOfYear rand_day=today;
                        int rand_increment=(1+rand()%365);
                        int rand_duration=(1+rand()%7);
                        int rand_vehicle_no= (1+rand()%total);
                        string rand_customer="customer "+ to_string(seq);
                        for(int i=0; i<rand_increment; i++)
                            ++rand_day;
                        if(My_Agency.isFree(rand_vehicle_no, rand_day, rand_duration))
                           {
                            cout<<"Booked,price for this Rental: "<<My_Agency.book(rand_vehicle_no,rand_customer,rand_day,rand_duration)<<endl;
                           }
                           else
                            cout<<"sorry vehicle already booked"<<endl;
                    }//end of generating random reservations
                }
                 else
                    cout<<"no Vehicles to be rented "<<endl;
                break;
            default:
                cout<<"\nwrong input "<<endl;
                //end of l
            }//end of the switch
   }//The open loop

   return 0;
}
//testing inputs
/*
b 2021-01-08 h 35 i 3 j 4 i 1 k 3
f
c 3 2021-02-12 3
Dilbert
f
c 5 2021-01-22 1
wally
f
c 3 2021-02-14
2

// booking input control test
// activate line 501 to check

b 2020-12-31
i 5 c 1
2020-12-30 // previous day(in the past) ,not allowed to book
c 1
2022-1-1 // not within the range(far future) , not allowed to book
c 1
2021-1-1 // nest day, 28 days allowed
c 1
2021-12-30 // last day of the upcoming year, not allowed to book (0 days)
c 1
2021-12-29 // 1 day allowed

*/

