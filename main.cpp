#include <cstdio>
#include <iostream>
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

static int p = 0;
static char buf[500];
static int get_index_of_Bus_with_bus_number(char number[]);
static void initialiseP();
static bool loadBusDataFromFile();
static void AllocateTheSeat(int row,int col);

class Buses
{
    public:
        char busn[5], driver[30], arrival[6], depart[6], from[50], to[50], seat[8][4][10];

    public:

      void install();

      void allotment();

      void empty();

      void show();

      void avail();

      void position(int i);

      void addThisBusToFile();
}bus[100];

static Buses Bus;

char number[5];

static void PutBusNumberInIndexFile(char no[])
{
    string num(no);
    string b = to_string(p);

    fstream file;
    file.open("bus_number_index.rec",ios::app);

    file<<num+"|"+b+"\n";
    file.close();
}

static bool checkTheBusNumber()
{
    string line,temp(number);
    ifstream file("bus_number_index.rec");

    while(getline(file,line,'|'))
    {
        if(strcmp(line.c_str(),temp.c_str()) == 0)
        {
            file.close();
            return 0;
        }
    }

    return 1;
}

void vline(char ch)
{
  for (int i=80;i>0;i--)
    cout<<ch;
}

void Buses::addThisBusToFile()
{
    cout<<"our bus is "<<Bus.busn<<" "<<Bus.driver<<" "<<Bus.arrival<<" "<<Bus.depart<<" "<<Bus.from<<" "<<Bus.to<<endl;
    fstream file;
    file.open("busData.rec",ios::out|ios::app);

    string busn_(busn);
    string driver_(driver);
    string arrival_(arrival);
    string depart_(depart);
    string from_(from);
    string to_(to);
    string seats = to_string(p);

    string Buf = busn_+"|"+driver_+"|"+arrival_+"|"+depart_+"|"+from_+"|"+to+"|"+seats+"\n\n";
    file<<Buf;
    file.close();
}

void Buses::install()
{
  cout<<"Enter bus no: ";
  cin>>number;

  if(!checkTheBusNumber())
  {
      cout<<"OOPs!!!!! bus already exists please retry......."<<endl;
      return;
  }

  strcpy(Bus.busn ,number);
  PutBusNumberInIndexFile(Bus.busn);

  cout<<"\nEnter Driver's name: ";
  cin>>Bus.driver;

  cout<<"\nArrival time: ";
  cin>>Bus.arrival;

  cout<<"\nDeparture time: ";
  cin>>Bus.depart;

  cout<<"\nFrom: \t\t\t";
  cin>>Bus.from;

  cout<<"\nTo: \t\t\t";
  cin>>Bus.to;

  Bus.empty();
  Bus.addThisBusToFile();
  p++;
}

void Buses::allotment()
{
  int seat;

  cout<<"Bus no: ";
  cin>>number;

  cout<<"\nSeat Number: ";
  cin>>seat;

     if(!loadBusDataFromFile())
     {
         cout<<"OOPs wrong bus number"<<endl;
         return;
     }
     if(seat>32)
     {
         cout<<"\nThere are only 32 seats available in this bus.";
     }
       else
         {
             int row = seat/4;
             int col = (seat%4)-1;

             if(seat%4 == 0)
             {
               row = seat/4 - 1;
               col = 3;
             }
            //  cout<<"row = "<<row<<"col is "<<col<<endl;
             if (strcmp(Bus.seat[row][col], "Empty") == 0)
               {
                 cout<<"Enter passanger's name: ";
                 cin>>Bus.seat[row][col];
                 AllocateTheSeat(row,col);
                 return;
               }
             else
               cout<<"The seat no. is already reserved.\n";
         }
}

static void AllocateTheSeat(int row,int col)
{
    // cout<<"allocating seat.... at row = "<<row<<" col = "<<col<<endl;
    int index = get_index_of_Bus_with_bus_number(number);
    // cout<<"bus with index "<<index<<endl;

    if(index == -1)
    {
        cout<<"no such bus"<<endl;
        return;
    }

    fstream file;
    file.open("seats.rec",ios::in|ios::out);
    string line;

    char q;
    file.clear();
    int linenumber = 0;
    while(!file.eof())
    {
        // file.get(q);
        getline(file,line);
        q = line[0];
        //cout<<q<<endl;
        int k = (int)q - '0';
        if(k == index){
            linenumber++;
            cout<<"breaking"<<endl;
          break;
        }
        cout<<q<<endl;
        linenumber++;
    }

    linenumber = 0;
    while(linenumber < row-1 && getline(file,line) )
    {
      linenumber++;
    }

    line = "";
    for(int i = 0;i < 4;i++)
    {
        line = line + Bus.seat[row][i]+"|";
    }

    line = line+"\n";
    
    file<<line;
}

void Buses::empty()
{
    fstream file;
    file.open("seats.rec",ios::out|ios::app);
    file<<p<<"*********\n";
    for(int i=0; i<8;i++)
    {
        for(int j=0;j<4;j++)
        {
            file<<"Empty|";
        }
        file<<"\n";
    }
    file.close();
}

void Buses::show()
{
  int n;

  cout<<"Enter bus no: ";
  cin>>number;


  int index = get_index_of_Bus_with_bus_number(number);


  loadBusDataFromFile();
      string line;
      vline('*');
      cout<<"\nBus no: "<<Bus.busn
      <<"\nDriver: "<<Bus.driver<<"\t\tArrival time: "
      <<Bus.arrival<<"\t\tDeparture time: "<<Bus.depart
      <<"\nFrom: "<<Bus.from<<"\t\tTo: "<<
      Bus.to<<"\n";
      vline('*');

      cout<<"seats are *************************************************************"<<endl;

      int a=1;
      for (int i=0; i<8; i++)
      {
        for(int j=0;j<4;j++)
        {
          if(strcmp(Bus.seat[i][j], "Empty")==0)
            {
              cout.width(5);
              cout.fill(' ');
              cout<<a<<".";
              cout.width(10);
              cout.fill(' ');
              cout<<Bus.seat[i][j];

            }
            else
            {
                cout.width(5);
                cout.fill(' ');
                cout<<a<<".";
                cout.width(10);
                cout.fill(' ');
                cout<<Bus.seat[i][j];
            }
            a++;
        }
        cout<<"\n";
      }
}

static bool loadBusDataFromFile()
{
    ifstream file("busData.rec");
    string line;
    int index = get_index_of_Bus_with_bus_number(number);
    if(index == -1)
    {
        cout << "wrong bus number please retry...."<<endl;
        return 0;
    }

    int fieldnumber = 0;
    while(getline(file,line))
    {
        string busnumber;
        std::string::size_type pos = line.find('|');
        if (pos != std::string::npos)
        {
            busnumber = line.substr(0, pos);
        }

        if(strcmp(busnumber.c_str(),number) == 0)
        {
            string word = "";
            for(int i = 0;i < line.size();i++)
            {
                if(line[i] != '|')
                {
                    word = word + line[i];
                }
                else
                {
                    switch(fieldnumber)
                    {
                          case 0: strcpy(Bus.busn, word.c_str());
                                  fieldnumber++;
                                  break;
                          case 1: strcpy(Bus.driver, word.c_str());
                                  fieldnumber++;
                                  break;
                          case 2: strcpy(Bus.arrival, word.c_str());
                                  fieldnumber++;
                                  break;
                          case 3: strcpy(Bus.depart, word.c_str());
                                  fieldnumber++;
                                  break;
                          case 4: strcpy(Bus.from, word.c_str());
                                  fieldnumber++;
                                  break;
                          case 5: strcpy(Bus.to, word.c_str());
                                  fieldnumber++;
                                  break;
                    }
                    word = "";
                }
            }
            break;
        }
    }

    ifstream files;
    files.open("seats.rec");
    char q;
    files.clear();
    while(!files.eof())
    {
        files.get(q);
        //cout<<q<<endl;
        int k = (int)q - '0';
        if(k == index){
          break;
        }
    }
    if(getline(files,line))
    {
        cout<<line<<" sovos"<<endl;
    }

    for(int i = 0;i < 8;i++)
    {
        getline(files,line);

        for(int j = 0;j < 4;j++)
        {
            int pos = line.find('|');
            string aseat = line.substr(0,pos);

            strcpy(Bus.seat[i][j],aseat.c_str());

            line = line.substr(pos+1);

        }
    }
    files.close();
    return 1;
}

void Buses::position(int l)
{
  int s=0;p=0;
  for (int i =0; i<8;i++)
  {
    cout<<"\n";
    for (int j = 0;j<4; j++)
    {
      s++;
      if(strcmp(bus[l].seat[i][j], "Empty")==0)
        {
          cout.width(5);
          cout.fill(' ');
          cout<<s<<".";
          cout.width(10);
          cout.fill(' ');
          cout<<bus[l].seat[i][j];
          p++;
        }
        else
        {
            cout.width(5);
            cout.fill(' ');
            cout<<s<<".";
            cout.width(10);
            cout.fill(' ');
            cout<<bus[l].seat[i][j];
        }
      }
    }

  cout<<"\n\nThere are "<<p<<" seats empty in Bus No: "<<bus[l].busn;
  }

//..shows all available busses
void Buses::avail()
{
    fstream file;
    string line;
    file.open("bus_number_index.rec",ios::in); //this file contains busnumber|index
  for(int n=0;n<p;n++)
  {
    while(getline(file,line))
    {
        std::string::size_type pos = line.find('|');
        if (pos != std::string::npos)
        {
            strcpy(number,line.substr(0, pos).c_str());
        }

        cout<<number<<endl;
        loadBusDataFromFile();

        vline('*');
        cout<<"\nBus no: \t"<<Bus.busn<<"\nDriver: \t"<<Bus.driver
        <<"\t\tArrival time: \t"<<Bus.arrival<<"\tDeparture Time: \t"
        <<Bus.depart<<"\nFrom: \t\t"<<Bus.from<<"\t\tTo: \t\t\t"
        <<Bus.to<<"\n";
        vline('*');
        vline('_');
    }
  }
}

static void initialiseP()
{
    ifstream file;
    file.open("bus_number_index.rec");

    if(!file.good())
    {
        file.close();
        cout<<"bus_number_index.rec file doesn't exists so creating........"<<endl;
        fstream file2;
        file2.open("bus_number_index.rec",ios::out);
        file2.close();
        p = 0;
        return;
    }

    string line;
    char c ='\0';
    file.seekg(-3,ios_base::end);


    getline(file,line);
    // cout<<"last line is "<<line<<endl;
    string no = line.substr(line.find("|") + 1);

    // cout<<"latest index is "<<no<<endl;
    p = std::stoi(no);
    p++;
    file.close();
}

static int get_index_of_Bus_with_bus_number(char number[])
{
    ifstream file("bus_number_index.rec");
    string line;

    while(getline(file,line))
    {
        string busnumber;
        std::string::size_type pos = line.find('|');
        if (pos != std::string::npos)
        {
            busnumber = line.substr(0, pos);
        }

        string no = line.substr(line.find("|") + 1);
        if(strcmp(busnumber.c_str(),number) == 0)
        {
            file.close();
            return std::stoi(no);
        }
    }
    file.close();
    return -1;
}

int main()
{
    system("clear");
    initialiseP();
    // cout<<"p is "<<p<<endl;

    int w;
    while(1)
    {
      cout<<"\n\n\n\n\n";
      cout<<"\t\t\t1.Add a new bus\n\t\t\t"
      <<"2.Reservation\n\t\t\t"
      <<"3.Show\n\t\t\t"
      <<"4.Buses Available. \n\t\t\t"
      <<"5.Exit";

      cout<<"\n\t\t\tEnter your choice:-> ";
      cin>>w;
      switch(w)
      {
        case 1:  Bus.install();
          break;
        case 2:  Bus.allotment();
          break;
        case 3:  Bus.show();
          break;
        case 4:  Bus.avail();
          break;
        case 5:  exit(0);
      }
    }
    return 0;
}
