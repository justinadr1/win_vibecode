#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include <istream>
#include <sstream>
#include <fstream>

using namespace std;


vector<string> htb;

vector<string> re_header = 
{
    R"(_______   _______  __ ___________  ______ ____  )",
    R"(\_  __ \_/ __ \  \/ // __ \_  __ \/  ___// __ \ )",
    R"( |  | \/\  ___/\   /\  ___/|  | \/\___ \\  ___/ )",
    R"( |__|    \___  >\_/  \___  >__|  /____  >\___  >)",
    R"(             \/          \/           \/     \/ )"
};

vector<string> re_entries;


void init()
{
    ifstream file("entries.txt");

    string line;
    while (getline(file, line))
    {
        re_entries.push_back(line);
    }

    file.close();
}

void re()
{
    for (int i = 0; i < 5; i++)
    {
        cout << re_header[i] << endl;
    }

    for (int i = 1; i <= re_entries.size(); i++)
    {
        cout << '[' << i << "] " << re_entries[i - 1] << "\n\n";
    }
}

void cool_print()
{
    cout << "[cool] ";
    string str;
    getline(cin, str);
    string alpha = " RXaNb!1H_c9dF#5efB3^gVhLi*PjD7kl=JTmZ)nUo8-pE&Kq6$rWsQtuCvG4+w@0AISxM(y2YzO";
    string temp;
    for (int i = 0; i < str.length(); i++)
    {
        for (int j = 0; j < alpha.length(); j++)
        {
            if (alpha[j] == str[i])
            {
                temp += alpha[j];
                cout << temp << endl;
                break;
            }
            cout << temp + alpha[j] << endl;
            this_thread::sleep_for(chrono::milliseconds(18));
        }
    }
}


int main()
{

    init();

    string cmd = "";
    while (true)
    {
        cout << "[input] ";    
        getline(cin, cmd);
        if (cmd == "re")
            re();
        else if (cmd == "cp")
            cool_print();
        else if (cmd == "x")
            break;
    }

}