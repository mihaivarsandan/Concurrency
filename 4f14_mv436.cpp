//
//  main.cpp
//  4F14_Coursework
//
//  Created by Mihai Varsandan on 03/03/2020.
//  Copyright © 2020 Mihai Varsandan. All rights reserved.
//
// Comments on the code: when the program is compiled and run there might an error, please try again as sometimes that error comes about 1 in 5 runs. If it still will not run please uncomment the delays in lines 126 and 137.
#include <iostream>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <thread>
#include <mutex>
using namespace std;

//Creating a class which will define the items in the queue
class item {
private:
    //size of the string
    int size = rand() % 4+3;
public:
    
    int number= rand() % 255;
    string name= "";
    //define a constructor to alocate the random characters to the string
    item ()
    {
    for (int i=1; i<= size; i++)
        name += 'a' + rand()%26;
    }
};

// creating a class which defines the queue
class queue{
private:
    //create an iterator for the list
    list<item>::iterator it;
    int Size=80;
    //create a mutex which will try to solve the printing issues so that one thread
    //at a single time
    mutex print;
public:
    // create a mutex which will ensure that the list will be modified
    // one at a time by a thread
    mutex list_mutex;
    // a list item was chosen to express the queue due the simplicity in the methods
    list<item> List;
    // create a constructor which populates the queue
    queue()
    {
        for(int i=1;i<=Size;i++)
        {
            item d;
            List.push_front(d);
        }
    }
bool check_empty()
    //create a function which will check if the list is empty
    {
    if(List.empty())
        return true;
    else
        return false;
    }
    
void displayQueue()
    // this is the task D
{
    for(it = List.begin(); it != List.end(); it++)
    {
        // whenever the printing of the value is required the mutex is console mutex is locked
        lock_guard<mutex> lock_list(print);
        item val=*it;
        cout<<"Number: "<<val.number<<"\t "<<"Name: "<<val.name<<"\t"<<"\n";
    }
    
}

void Sum_and_display()
    //this is task C
{
    int Sum=0;
    list<item>::iterator it;
    List.reverse();
    for(it = List.begin(); it != List.end(); it++)
    {

        item val=*it;
        Sum += val.number;
    }
    // whenever the printing of the sum is required the mutex is console mutex is locked
    lock_guard<mutex> lock_list(print);
    cout<<"Sum of the integers in the list: "<<Sum<<"\n";
}
void remove_random()
    // this is Task E
{
    // define which random number will be erased
    int random_num = rand() % Size + 1;

    int i=1;
    for(it = List.begin(); it != List.end(); it++)
    {
        if (i==random_num)
        {
            List.erase(it);
            Size--;
        }
        i++;
    }
    // whenever the printing to confirm the erasure is required the mutex is console mutex is locked
    lock_guard<mutex> lock_list(print);
    cout<<"Item erased"<<"\n";
}
};

void Task_C(queue &Q)
{
    //first continously check if the list is empty
    while(Q.check_empty() == false)
    {
        //lock the list using the unqiue mutex
        unique_lock<mutex>lock_list(Q.list_mutex,defer_lock);
        Q.Sum_and_display();
        //this_thread::sleep_for(chrono::milliseconds(1));
    }
}
void Task_D(queue &Q)
{
    //first continously check if the list is empty
    while(Q.check_empty() ==false)
    {
        //lock the list using the unqiue mutex
        unique_lock<mutex>lock_list(Q.list_mutex,defer_lock);
        Q.displayQueue();
        //this_thread::sleep_for(chrono::milliseconds(1));
        
    }
}
void Task_E(queue &Q)
{
    //first continously check if the list is empty
    while(Q.check_empty() ==false)
    {
        //lock the list using the unqiue mutex
        unique_lock<mutex>lock_list(Q.list_mutex,defer_lock);
        Q.remove_random();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}


int main(int argc, const char * argv[]) {
    // insert code here...
    queue Queue;
    thread t1(Task_C, ref(Queue));
    thread t2(Task_D, ref(Queue));
    thread t3(Task_E, ref(Queue));
    t1.join();
    t2.join();
    t3.join();
    

    
    return 0;
}

