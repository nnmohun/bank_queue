#include<iostream>
#include<vector>
#include<string>
#include<ranlib.h>
#include<queue>
#include<stack>
#include<list>
#include <map>
#include<iterator>

using namespace std;



class teller 
{
    public:

        teller(){open=true;}

        bool openTeller(bool opn);
        bool getStatus() {return this->open;}


    private:
        bool open;
       
};


bool teller::openTeller(bool opn)
{
    open = opn;
    return open;
}

class customer
{
    public:
        customer();
        customer(int a, int t,int idNum);
        void setArriveTime();
        void modATime(int t);
        void modTTime(int t) {taskTime=t;}
        void setTaskTime();
        int getArriveTime() {return arriveTime;}
        int getTaskTime() {return taskTime;}
        int getCNum(){return cNum;}
        int getTellerID(){return tellerIndexMatch;}
        void matchTeller(int tNum);


    private:
        int arriveTime;
        int taskTime;
        int cNum;// identify customer on their number of entry
        int tellerIndexMatch;
   

};

customer::customer()
{
    arriveTime=0;
    taskTime=0;
    cNum=0;


}
customer::customer(int a, int t, int idNum)
{
    arriveTime=a;
    taskTime=t;
    cNum=idNum;
}
void customer::setTaskTime()
{
    taskTime = rand()%20+1; //task random int 1-20min
}
void customer::setArriveTime()
{
    srand(time(NULL));
    int t = rand()%10+5;

    arriveTime=t;
}
void customer::modATime(int a)
{
    arriveTime=a;
}
void customer::matchTeller(int tNum)
{
    tellerIndexMatch=tNum;
}


class bank
{
    public:

         void operateBank(bank b); //operates the bank will call all functions below
                                //inside here

        int numInLine() {return line.size();} //return num people in line
       
        void setUpNumberTeller(int numT);//set up number tellers working today
                                        //will be size of teller vector

        void customerList(int numC);//set up customer list with num customers that day
        void addCustomerToLine(); //add this to the line queue

        

        void customerinLine();//when task is done pop from queue
        bool fillAllTeller();
   

        double avgWaitTime();
    private:
        vector<teller> tellerData;
        customer c;
        vector<customer> customerLineData; //vector of pointers of customer objs
        queue<customer> line; //line full of customers
        int minWait;
};




void bank::operateBank(bank b)
{
    int nT=0;
    bool open = true;
    cout<<"How many tellers are working today? ";
    cin>>nT;

    b.setUpNumberTeller(nT);

      srand(time(NULL));
    int numC = rand()%80 + 20;

    b.customerList(numC); //random customers per day 20-100num
                            //each customer in list contains random task and arrival time

    b.customerinLine();

}
void bank::addCustomerToLine()
{ 
    //as time runs if the minutes are divisible by the arrival time then push that current 
    //customer to the line 

    // works cout<<customerLineData.size()<<endl;

    int prev = customerLineData[0].getArriveTime();
    line.push(customerLineData[0]);
    

    for(int i=1; i<customerLineData.size(); i++)
    {
        int current = prev + customerLineData[i].getArriveTime();
        customerLineData[i].modATime(current);
        line.push(customerLineData[i]);

        //works cout<<customerLineData[i].getArriveTime()<<endl;

        prev = current;



    }


}
bool bank::fillAllTeller()
{
    int t = tellerData.size()-1;
    for(int i=0;i<tellerData.size();i++)
        if(tellerData[i].getStatus()==true)
            t++;

    if(t<=tellerData.size())
        return true;
    
    return false;

}

void bank::customerList(int num)
{
    customerLineData.resize(num); //size equal to number of customers per day

    cout<<num<<" num customers"<<endl;

    //randomize task time for each customer 
    for(int i=0; i<customerLineData.size(); i++)
    {
        int a = rand()%10+5;
        int t =rand()%20+1; //task random int 1-20min
        customer c (a,t,i);
        
        //works cout<<"customer idNum: "<<c.getCNum() <<" task time: "<<c.getTaskTime()<<" arriving at : "<<c.getArriveTime()<<endl;
        
        customerLineData[i]=c; //works 
        
        
       
        
        
    }
    
    //add all the customers from the vector into the queue of the line
    addCustomerToLine();

    cout<<"customers in line: "<<line.size()<<endl;

}

void bank::setUpNumberTeller(int numT)
{
    //size of teller vector is of numT each teller index value contains the bool value busy
    tellerData.resize(numT);
    
    for(int i=0; i<numT; i++)
        tellerData[i].openTeller(true);
    
    //works cout<<tellerData[0].getStatus()<<endl;

   
    
    
}



void bank::customerinLine()
{
   
    bool tellerOpen =true;
    int tellerId=0;
    int maxT=tellerData.size();

    vector<int> leavesFirst;

    int minElementIndex;
    int minLeave;

   
    
    
   int tellerNum=0;
   bool allOpen;
    for(int i=0; i<480; i++)//num min in 8hr day
    {
        for(int j=0; j<customerLineData.size();j++)//loop thru the line vector
        {
            int leaveBank=customerLineData[j].getArriveTime()+customerLineData[j].getTaskTime();
            
            if(i==customerLineData[j].getArriveTime())
            {
                cout<<customerLineData[j].getCNum()<<" arrives at: "<<customerLineData[j].getArriveTime()<<endl;
                for(int k=0;k<tellerData.size();k++)
                {
                    if(tellerData[k].getStatus()==true)
                    {
                        customerLineData[j].matchTeller(k);
                        cout<<j<<" customer is with teller: "<<customerLineData[j].getTellerID()<<endl;
                        cout<<"customer "<<customerLineData[j].getCNum()<<" leaves at: "<<customerLineData[j].getTaskTime()+i<<endl;
                        tellerData[k].openTeller(false);
                        leavesFirst.push_back(customerLineData[j].getTaskTime()+i);
                        line.pop();

                        break;
                        
                       
                    }
                    
                    
                }
                
                
            
            }
            if(i==leaveBank)
            {
                tellerData[customerLineData[j].getTellerID()].openTeller(true);
                cout<<"customer "<<customerLineData[j].getCNum()<<" has left "<<customerLineData[j].getTellerID()<<" is now open at "<<leaveBank<<endl;
        
            }
            
           

        }
    }



}




int main()
{
    bank b;
    b.operateBank(b);
    

    return 0;
}
