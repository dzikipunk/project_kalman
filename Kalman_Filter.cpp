#include <iostream>
#include <cstdio>
#include "FlightEmulator.h"
#include<fstream>
#include<string>


class Kalman_Values{
    private:

        double dposition=0;
        double dpredPosition=0;
        double dVelocity=0;
        double dpredVel=0;
        double dAcceleration=0;
        int duration_time=0;
        double dUncertain=0, destimUncertain=0,err_var=0;
        double Kalman_gain = 0;

    public:
        


        double dalpha=0.5;
        double dbetha=0.4;
        double dgamma=0.1;
        void predict(int time){ 
            dpredPosition = dposition + (time * dVelocity) + (dAcceleration * time * time) / 2;
            dpredVel = dVelocity + dAcceleration * time;
        };
        int Next_Position(int time, int measurement) {
            duration_time += time;
            Set_gain();
            
            predict(time);

            dposition = dpredPosition + dalpha * (measurement - dpredPosition);
            dVelocity = dpredVel + dbetha  * (measurement - dpredPosition) / time;
            dAcceleration = dAcceleration + dgamma * 2 * (measurement - dpredPosition) / time / time;
            
            
            return dposition;
  
        };
        void Set_zero(int position, double Vel, double Acceleration,double measurement_err) {
            duration_time = 0;
            dposition = position;
            dAcceleration = Acceleration;
            dpredVel = Vel;
            dUncertain =measurement_err* measurement_err;
            err_var = measurement_err * measurement_err;
            dalpha = 1;
            dbetha = 1;
            dgamma = 1;

        };
        void Set_gain() {
            Kalman_gain = dUncertain / (dUncertain + err_var);
            dUncertain = (1 - Kalman_gain) * dUncertain;
            dalpha = dalpha * Kalman_gain;
            dbetha = dbetha * Kalman_gain;
            dgamma = dgamma * Kalman_gain;


        };
        void Input_start_value() {
            
            std::cout << "wartosci poczatkowe V:\n";
            std::cin >> dVelocity;
            std::cout << "\npozycja poczatkowa:\n";
            std::cin >> dposition;
            std::cout << "\nprzyspieszenie początkowe:\n";
            std::cin >> dAcceleration;
            std::cout << "\nniepewnosc pomiaru:\n";
            std::cin >> dUncertain;
            err_var = dUncertain;
            duration_time = 0;
            dalpha = 0.5;
            dbetha = 0.4;
            dgamma = 0.1;

        };



};
int extract_string(std::string liczba) {
    int output=0;
    while (liczba[liczba.length()-1] < 48 || liczba[liczba.length() - 1] > 57)
        liczba.pop_back();
    for (int i = 1, multipla = 1; i <= liczba.length(); i++, multipla = multipla* 10)
        output += multipla * (liczba[liczba.length() - i]-48);
    return output;
}
//void load_test_values(std::string name)
//{   
//    std::ifstream Loadfile;
//    Loadfile.open(name);
//    int c;
//    std::string liczba;
//    while (Loadfile)
//    {
//        getline(Loadfile, liczba, ' ');
//        std::cin >> c;
//        std::cout << liczba;
//        std::cout << extract_string(liczba);
//    }
//}

int main()
{
    FlightEmulator Flight;
    Kalman_Values x, y, z;
    Filter_movement xx, yy, zz;
    int choose=1;
    while (true)
    {
        std::cout << "co poradzisz kochaneczku\n znowu sie spierdolilo ?\n 1 jesli chesz nowy (losowy XD) zestaw testowy\n 2 zaladuj zestaw testowy z pliku\n bazowo:test.txt \n 3 wpisz swoje cyferki ";
        std::cin >> choose;
       
        if (choose == 1)
        {
            Flight.set_error(500);
            Flight.emulate(3);
            Flight.close_files();
        }
        if (choose == 2)
        {   
            xx.reset_fliter_val();
            yy.reset_fliter_val();
            zz.reset_fliter_val();
            
            std::string name;
            std::cout << "\nszybciej, szybcioej podaj  sciezkę:";
            std::getline(std::cin,name);
            std::cin.ignore();

            if(name=="")
                name = "test1.txt";
            std::ifstream Loadfile;
            Loadfile.open(name, std::ios::binary);
            Loadfile.seekg(0, std::ios::end);
            int length = Loadfile.tellg();
            Loadfile.seekg(0);
            int czekoj=0;
            int posx, posy, posz, czas_trenere=0;
            int measure_err = 5;
            std::string liczba;
            while (Loadfile)
            {
                czekoj = czas_trenere;
              //  std::cout<<"getcount:"<< Loadfile.tellg()<<"\n";
                getline(Loadfile, liczba, ' ');
                czas_trenere = extract_string(liczba);
                std::cout<<"\n\n" <<"t:"<< czas_trenere << " ";
                liczba.clear();
                

                getline(Loadfile, liczba, ' ');
                std::cout << " x:";// << liczba << " ";
                posx= extract_string(liczba);
                std::cout << posx;
                liczba.clear();

                getline(Loadfile, liczba, ' ');
                std::cout << " y:"; //<< liczba << " ";
                posy = extract_string(liczba);
                std::cout << posy;
                liczba.clear();
                
                getline(Loadfile, liczba, '\n');
                std::cout << " z:";//<< liczba << " ";
                posz = extract_string(liczba);
                std::cout << posz<<"\n";
                liczba.clear();

                if (czas_trenere==0)
                {
                    xx.set_values(posx, 0, 0, measure_err);
                    yy.set_values(posy, 0, 0, measure_err);
                    zz.set_values(posy, 0, 0, measure_err);

                    x.Set_zero(posx, 0, 0, measure_err);
                    y.Set_zero(posy, 0, 0, measure_err);
                    z.Set_zero(posz, 0, 0, measure_err);
                    

                }
                else
                {
                  
                   std::cout << "xe: " << xx.Filter_Input(czas_trenere - czekoj, posx) << "xk: " << x.Next_Position(czas_trenere - czekoj, posx) << " \n";
                   std::cout << "ye: " << yy.Filter_Input(czas_trenere - czekoj, posy) << "yk: " << y.Next_Position(czas_trenere - czekoj, posy) << " \n";
                   std::cout << "ze: " << zz.Filter_Input(czas_trenere - czekoj, posz) << "xk: " << z.Next_Position(czas_trenere - czekoj, posz) << " \n";
                }
             //   std::cin >> czekoj;
             // std::cout<<"end:" <<Loadfile.tellg()<<"\n";
                if (Loadfile.tellg() == length)
                    break;
            }
        }
        if (choose == 3)
        {

        }
        //else break;
    }
    
   
   
   
    
 /*   x.Input_start_value();
  
    int positionx, time;
    while (true)
    {
        std::cout << "\nposition:";
        std::cin >> positionx;
        std::cout << "\ntime:";
        std::cin>> time;
        std::cout<<"good old joe:"<< x.Next_Position(time, positionx);
        std::cout << "\nSome NPW600 or Amphetamine:" << xx.Filter_Input(time, positionx);

    }*/
    
    return 0;
    
    
}

