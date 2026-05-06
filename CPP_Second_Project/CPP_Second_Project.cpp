#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

const int MAX = 100;

struct timeSlot
{
    int startHour;
    int endHour;
    int startMin;
    int endMin;
};
struct Booking
{
    int id;
    string name;
    string day;
    timeSlot slot;
};
struct Request
{
    int id;
    string name;
    string preferredDay[7];
    timeSlot window[7];
    int dayCount;
};

Booking bookings[MAX];
int bookingCount = 0;
Request requests[MAX];
int requestCount = 0;
int generateID()
{
    ifstream file("Input.txt");
    int count = 100;
    string line;
    while (getline(file, line))
    {
        count++;
    }
    file.close();
    return count + 1;
}
void submitRequest()
{
    Booking id;
     id.id = generateID();
    Request sub;
    cout << "Please enter your name: ";
    getline(cin, sub.name);
    cout << "enter your massaege: ";
    string massage;
    getline(cin, massage);
    ofstream file("Input.txt", ios::app);
    file << id.id << ' ' << sub.name << ": " << massage << endl;
    file.close();
    cout << "Your registeration saved successfully!\n" << "You can see your time with id : " << id.id << "\n\n";
}
bool isSlotFree(string day, int hour, int min)
{
    for (int i = 0; i < bookingCount; i++)
    {
        if (bookings[i].day == day && bookings[i].slot.startHour == hour && bookings[i].slot.startMin == min)
            return false;
    }
    return true;
}
void findSlot(Request sub)
{
    for (int i = 0; i < sub.dayCount; i++)
    {
        int startTotal = sub.window[i].startHour * 60 + sub.window[i].startMin;
        int endTotal = sub.window[i].endHour * 60 + sub.window[i].endMin;

        for (int j = startTotal; j + 60 <= endTotal; j += 60)
        {
            int hour = j / 60;
            int min = j % 60;

            if (isSlotFree(sub.preferredDay[i], hour, min))
            {
                bookings[bookingCount].id = sub.id;
                bookings[bookingCount].name = sub.name;
                bookings[bookingCount].day = sub.preferredDay[i];
                bookings[bookingCount].slot.startHour = hour;
                bookings[bookingCount].slot.startMin = min;
                bookings[bookingCount].slot.endHour = (j + 60) / 60;
                bookings[bookingCount].slot.endMin = (j + 60) % 60;
                bookingCount++;
                return;
            }
        }
    }
}
void loadRequests()
{
    ifstream file("Input.txt");
    string line;
    while (getline(file, line))
    {
        int pos = line.find(':');
        requests[requestCount].id = stoi(line.substr(0, 3));
        requests[requestCount].name = line.substr(3, pos-3);
        requests[requestCount].dayCount = 0;
        string rest = line.substr(pos + 2);

        while (rest.length() > 0)
        {
            int space = rest.find(' ');
            string word;
            if (space == string::npos)
            {
                word = rest;
                rest = "";
            }
            else
            {
                word = rest.substr(0, space);
                rest = rest.substr(space + 1);
            }

            if (word == "shanbe" || word == "yekshanbe" || word == "doshanbe" || word == "seshanbe" || word == "chaharshanbe" || word == "panjshanbe" || word == "jome" || word == "shanbeh" || word == "yekshanbeh" || word == "doshanbeh" || word == "seshanbeh" || word == "chaharshanbeh" || word == "panjshanbeh" || word == "jomeh")
            {
                if (word == "shanbeh")
                    word = "shanbe";
                else if (word == "yekshanbeh")
                    word = "yekshanbe";
                else if (word == "doshanbeh")
                    word = "yekshanbeh";
                else if (word == "seshanbeh")
                    word = "seshanbe";
                else if (word == "chaharshanbeh")
                    word = "chaharshanbe";
                else if (word == "panjshanbeh")
                    word = "panjshanbe";
                else if (word == "jomeh")
                    word = "jome";
                int dc = requests[requestCount].dayCount;
                requests[requestCount].preferredDay[dc] = word;
                requests[requestCount].window[dc].startHour = 8;
                requests[requestCount].window[dc].startMin = 0;
                requests[requestCount].window[dc].endHour = 22;
                requests[requestCount].window[dc].endMin = 0;
                requests[requestCount].dayCount++;
            }
            else if (word == "sobh")
            {
                int last = requests[requestCount].dayCount - 1;
                requests[requestCount].window[last].startHour = 8;
                requests[requestCount].window[last].startMin = 0;
                requests[requestCount].window[last].endHour = 12;
                requests[requestCount].window[last].endMin = 0;
            }
            else if (word == "asr")
            {
                int last = requests[requestCount].dayCount - 1;
                requests[requestCount].window[last].startHour = 12;
                requests[requestCount].window[last].startMin = 0;
                requests[requestCount].window[last].endHour = 18;
                requests[requestCount].window[last].endMin = 0;
            }
            else if (word == "shab")
            {
                int last = requests[requestCount].dayCount - 1;
                requests[requestCount].window[last].startHour = 18;
                requests[requestCount].window[last].startMin = 0;
                requests[requestCount].window[last].endHour = 22;
                requests[requestCount].window[last].endMin = 0;
            }
            else if (word.find('-') != string::npos)
            {
                int last = requests[requestCount].dayCount - 1;
                int dash = word.find('-');
                string startPart = word.substr(0, dash);
                string endPart = word.substr(dash + 1);

                int colon1 = startPart.find(':');
                int colon2 = endPart.find(':');

                if (colon1 != string::npos && colon2 != string::npos)
                {
                    requests[requestCount].window[last].startHour = stoi(startPart.substr(0, colon1));
                    requests[requestCount].window[last].startMin = stoi(startPart.substr(colon1 + 1));
                    requests[requestCount].window[last].endHour = stoi(endPart.substr(0, colon2));
                    requests[requestCount].window[last].endMin = stoi(endPart.substr(colon2 + 1));
                }
                else
                {
                    requests[requestCount].window[last].startHour = stoi(startPart);
                    requests[requestCount].window[last].startMin = 0;
                    requests[requestCount].window[last].endHour = stoi(endPart);
                    requests[requestCount].window[last].endMin = 0;
                }
            }
        }
        requestCount++;
        findSlot(requests[requestCount - 1]);
    }
    file.close();
}
void showSchedule()
{
    int pass;
    cout << "Enter admin pass to login: ";
    cin >> pass;
    if (pass != 13860424)
        cout << "\nWrong password!\n\n";
    else
    {
        system("cls");
        bookingCount = 0;
        requestCount = 0;
        loadRequests();
        string days[7] = { "shanbe" , "yekshanbe", "doshanbe", "seshanbe", "chaharshanbe", "panjshanbe", "jome" };
        for (int i = 0; i < bookingCount - 1; i++)
        {
            for (int j = 0; j < bookingCount - i - 1; j++)
            {
                int time1 = bookings[j].slot.startHour * 60 + bookings[j].slot.startMin;
                int time2 = bookings[j + 1].slot.startHour * 60 + bookings[j + 1].slot.startMin;
                if (time1 > time2)
                {
                    Booking temp = bookings[j];
                    bookings[j] = bookings[j + 1];
                    bookings[j + 1] = temp;
                }
            }
        }
        int maxLen = 0;
        for (int i = 0; i < bookingCount; i++)
        {
            if ((int)bookings[i].name.length() > maxLen)
                maxLen = bookings[i].name.length();
        }

        for (int i = 0; i < 7; i++)
        {
            bool hasBooking = false;
            for (int j = 0; j < bookingCount; j++)
            {
                if (bookings[j].day == days[i])
                {
                    hasBooking = true;
                    break;
                }
            }
            if (hasBooking)
            {
                cout << days[i] << ":\n";
                for (int j = 0; j < bookingCount; j++)
                {
                    if (bookings[j].day == days[i])
                    {
                        cout << "    " << setfill(' ') << left << setw(maxLen + 2) << bookings[j].name << setfill('0') << right << "(" << setw(2) << bookings[j].slot.startHour << ":" << setw(2) << bookings[j].slot.startMin << "-" << setw(2) << bookings[j].slot.endHour << ":" << setw(2) << bookings[j].slot.endMin << ")\n";
                    }
                }
            }
        }
        cout << '\n';
    }
}
void myAppoinment()
{
    int id;
    cout << "Enter your id: ";
    cin >> id;
    bookingCount = 0;
    requestCount = 0;
    loadRequests();
    bool found = false;
    for (int i = 0; i < bookingCount; i++)
    {
        if (bookings[i].id == id)
        {
            cout << bookings[i].name<<':' << '\n' << "     " << bookings[i].day  << setfill('0') << right << " (" << setw(2) << bookings[i].slot.startHour << ":" << setw(2) << bookings[i].slot.startMin << "-" << setw(2) << bookings[i].slot.endHour << ":" << setw(2) << bookings[i].slot.endMin << ")\n";
            found = true;
            break;
        }
    }
    if (!found)
        cout << "\nNo appoinment found!\n\n";
}
void showMenu()
{
    int choice;
    while (true)
    {
        system("cls");
        cout << "================================\n";
        cout << "Welcome to Reservation System!\n";
        cout << "1. Register Request\n";
        cout << "2. Show Schedule (Admin Panel)\n";
        cout << "3. My Appoinment\n";
        cout << "0. Exit\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        system("cls");
        switch (choice)
        {
        case 1:
            submitRequest();
            break;
        case 2:
            showSchedule();
            break;
        case 3:
            myAppoinment();
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice!\n";
        }
        system("pause");
    }
}
int main()
{
    showMenu();
}