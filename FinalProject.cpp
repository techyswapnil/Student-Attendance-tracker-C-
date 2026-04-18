#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// ================= BASE CLASS =================
class Person {
protected:
    int id;
    string name;

public:
    Person(int i = 0, string n = "") {
        id = i;
        name = n;
    }

    virtual void display() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
    }
};

// ================= DERIVED CLASS =================
class Student : public Person {
private:
    vector<bool> attendance;
    static int totalStudents;   // static member

public:
    // Constructor
    Student(int i = 0, string n = "") : Person(i, n) {
        totalStudents++;
    }

    int getId() const {
        return id;
    }

    static int getTotalStudents() {
        return totalStudents;
    }

    void markAttendance(bool status) {
        attendance.push_back(status);
    }

    double calculatePercentage() const {
        int present = 0;

        for (int i = 0; i < attendance.size(); i++) {
            if (attendance[i] == true)
                present++;
        }

        if (attendance.size() == 0)
            return 0;

        return (double)present / attendance.size() * 100;
    }

    // Operator overloading (compare attendance)
    bool operator>(const Student& other) {
        return calculatePercentage() > other.calculatePercentage();
    }

    void display() const override {
        Person::display();
        cout << "Total Classes: " << attendance.size() << endl;
        cout << "Attendance %: " << calculatePercentage() << "%" << endl;
        cout << "-------------------------" << endl;
    }

    void saveToFile(ofstream& file) {
        file << id << " " << name << " ";
        for (int i = 0; i < attendance.size(); i++) {
            file << attendance[i] << " ";
        }
        file << endl;
    }
};

// initialize static member
int Student::totalStudents = 0;

// ================= MAIN =================
int main() {
    
    //The vector stores  pointers to Student objects
    
    vector<Student*> students;  
    int choice;

    do {
        cout << "\n===== Student Attendance System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. Display Students\n";
        cout << "4. Save to File\n";
        cout << "5. Compare First Two Students\n";
        cout << "6. Exit\n";
        cout << "Enter your  choice: ";
        
        cin >> choice;

        try {

            if (choice == 1) {
                int id;
                string name;

                cout << "Enter ID: ";
                cin >> id;

                if (id < 0 )
                    throw "Invalid ID!";

                cout << "Enter Name: ";
                cin >> name;

                students.push_back(new Student(id, name));
                cout << "Student added.\n";
            }

            else if (choice == 2) {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                for (int i = 0; i < students.size(); i++) {
                    if (students[i]->getId() == id) {
                        char status;
                        cout << "Present (P) / Absent (A): ";
                        cin >> status;

                        if (status == 'P' || status == 'p')
                            students[i]->markAttendance(true);
                        else
                            students[i]->markAttendance(false);
                    }
                }
            }

            else if (choice == 3) {
                for (int i = 0; i < students.size(); i++) {
                    students[i]->display();
                }

                cout << "Total Students: "
                     << Student::getTotalStudents() << endl;
            }

            else if (choice == 4) {
                ofstream file("attendance.txt");

                for (int i = 0; i < students.size(); i++) {
                    students[i]->saveToFile(file);
                } 

                file.close();
                cout << "Data saved to file.\n";
            }

            else if (choice == 5) {
                if (students.size() >= 2) {
                    if (*students[0] > *students[1])
                        cout << "First student has higher attendance.\n";
                    else
                        cout << "Second student has higher attendance.\n";
                } else {
                    cout << "Need at least 2 students.\n";
                }
            }

        } catch (const char* msg) {
            cout << "Error: " << msg << endl;
        }

    } while (choice != 6);

    // Free memory
    for (int i = 0; i < students.size(); i++) {
        delete students[i];
    }

    return 0;
}
