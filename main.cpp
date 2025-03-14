#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <sstream>

using namespace std;

string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    string currentDate = ctime(&currentTime);
    return currentDate;
}

class Step
{
public:
    virtual void execute() = 0;
};

class TitleStep : public Step
{
protected:
    string title;
    string subtitle;

public:
    TitleStep(const string &title, const string &subtitle)
    {
        this->title = title;
        this->subtitle = subtitle;
    }
    void execute() override
    {
        ofstream file("flows.txt", ios::app);
        if (file.is_open())
        {
            file << "TITLE step:" << endl;
            file << "Title: " << title << " " << endl;
            file << "Subtitle: " << subtitle << endl;
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }
};

class TextStep : public TitleStep
{
public:
    TextStep(const string &title, const string &subtitle) : TitleStep{title, subtitle} {}
    void execute() override
    {
        ofstream file("flows.txt", ios::app);
        if (file.is_open())
        {
            file << "TEXT step:" << endl;
            file << "Title: " << title << " " << endl;
            file << "Subtitle: " << subtitle << endl;
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }
};

class Text_InputStep
{
private:
    string description;
    string input;

public:
    void setdescription(const string &description)
    {
        this->description = description;
    }
    void setinput(const string &input)
    {
        this->input = input;
    }
    string getDescription() const
    {
        return description;
    }

    string getInput() const
    {
        return input;
    }
};

class Number_InputStep
{
private:
    string description;
    float input;

public:
    void setdescription(const string &description)
    {
        this->description = description;
    }
    void setinput(const float &input)
    {
        this->input = input;
    }
    string getDescription() const
    {
        return description;
    }
    float getInput() const
    {
        return input;
    }
};

class CalculusStep : public Step
{
private:
    float step1;
    float step2;
    string operation;

public:
    void setstep1(float step1)
    {
        this->step1 = step1;
    }
    void setstep2(float step2)
    {
        this->step2 = step2;
    }
    void setoperation(const string &operation)
    {
        this->operation = operation;
    }
    float getstep1()
    {
        return this->step1;
    }
    float getstep2()
    {
        return this->step2;
    }
    string getoperation()
    {
        return this->operation;
    }
    void execute() override
    {
        float result;
        if (operation == "+")
        {
            result = step1 + step2;
        }
        else if (operation == "-")
        {
            result = step1 - step2;
        }
        else if (operation == "*")
        {
            result = step1 * step2;
        }
        else if (operation == "/")
        {
            result = step1 / step2;
        }
        else if (operation == "max")
        {
            result = max(step1, step2);
        }
        else if (operation == "min")
        {
            result = min(step1, step2);
        }
        if (!(operation == "max" || operation == "min"))
            cout << step1 << " " << operation << " " << step2 << " = " << result << " " << endl;
        else
            cout << operation << "(" << step1 << ", " << step2 << ") = " << result << " " << endl;
    }
};
class DisplayStep : public Step
{
private:
    string step;

public:
    DisplayStep(const string &text_input)
    {
        this->step = text_input;
    }
    void execute() override
    {
        ifstream file(step);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
            }
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }
};

class TextFileInputStep : public Step
{
protected:
    string description;
    string fileName;

public:
    void setdescription(const string &description)
    {
        this->description = description;
    }
    void setfileName(const string &fileName)
    {
        this->fileName = fileName;
    }
    void execute() override
    {
        fstream file(fileName, ios::app);
    }
};

class CsvFileInputStep : public TextFileInputStep
{
public:
    void execute() override
    {
        fstream file(fileName, ios::app);
    }
};

class OutputStep
{
private:
    int step;
    string fileName;
    string title;
    string description;

public:
    OutputStep(int step, const string &fileName, const string &title, const string &description)
    {
        this->step = step;
        this->fileName = fileName;
        this->title = title;
        this->description = description;
    }
    void execute(char c[200][200], int findit)
    {
        ofstream file(fileName, ios::app);
        if (file.is_open())
        {
            file << title << endl;
            file << description << endl;
            while(1)
            {
                file << c[findit] << endl;
                findit++;
                if(strstr(c[findit],"step"))
                    break;
            }
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }
};

class EndStep : public Step
{
public:
    void execute() override
    {
        ofstream file("flows.txt", ios::app);
        if (file.is_open())
        {
            file << "End of the flow" << endl;
            file.close();
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }
};

void create_flow()
{
    ofstream file("flows.txt", ios::app);
    if (!(file.is_open()))
        cout << "Unable to open file." << endl;
    string flowName;
    cout << "Enter the name of the flow: ";
    cin >> flowName;
    file << "Flow name: " << flowName << endl;
    file << "Date: " << getCurrentDate() << endl;
    file.close();

    char confirm;
    cout << "Do you want to add TITLE step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            cout << "TITLE step: title (string), subtitle (string)" << endl;
            try
            {
                cout << "Type the title:";
                string title;
                cin.ignore();
                getline(cin,title);
                cout << "Type the subtitle:";
                string subtitle;
                cin.ignore();
                getline(cin,subtitle);
                if (title.find('\n') != string::npos || subtitle.find('\n') != string::npos)
                    throw runtime_error("Invalid input.");
                else
                {
                    TitleStep *titleStep = new TitleStep(title, subtitle);
                    titleStep->execute();
                    cout << "Do you want to add same step once again?Y/N:";
                    cin >> confirm;
                }
            }
            catch (const exception &e)
            {
                cerr << e.what() << '\n';
                cout << "Do you want to repeat the step?Y/N:";
                cin >> confirm;
                if (confirm == 'N' || confirm == 'n')
                    break;
            }

        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add TEXT step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            cout << "TITLE step: title (string), subtitle (string)" << endl;
            try
            {
                cout << "Type the title:";
                string title;
                cin.ignore();
                getline(cin,title);
                cout << "Type the copy:";
                string subtitle;
                cin.ignore();
                getline(cin,subtitle);
                if (title.find('\n') != string::npos || subtitle.find('\n') != string::npos)
                    throw runtime_error("Invalid input.");
                else
                {
                    TextStep *textstep = new TextStep(title, subtitle);
                    textstep->execute();
                    cout << "Do you want to add same step once again?Y/N:";
                    cin >> confirm;
                }
            }
            catch (const exception &e)
            {
                cerr << e.what() << '\n';
                cout << "Do you want to repeat the step?Y/N:";
                cin >> confirm;
                if (confirm == 'N' || confirm == 'n')
                    break;
            }

        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add TEXT input step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "TEXT input step:" << endl;
                file << "description: " << endl;
                file << "text input: " << endl;
                file.close();
                cout << "Do you want to add another TEXT input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add NUMBER input step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "NUMBER input step:" << endl;
                file << "description: " << endl;
                file << "number input: " << endl;
                file.close();
                cout << "Do you want to add another NUMBER input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');

    cout << "Do you want to add CALCULUS step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "CALCULUS step:" << endl;
                file << "step1: " << endl;
                file << "step2: " << endl;
                file << "operation: " << endl;
                file.close();
                cout << "Do you want to add another CALCULUS input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add DISPLAY step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "DISPLAY step:" << endl;
                file.close();
                cout << "Do you want to add another DISPLAY input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add TEXT FILE input step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "TEXT FILE input step:" << endl;
                file << "description: " << endl;
                file << "file name: " << endl;
                file.close();
                cout << "Do you want to add another TEXT input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add CSV FILE input step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "CSV FILE input step:" << endl;
                file << "description: " << endl;
                file << "file name: " << endl;
                file.close();
                cout << "Do you want to add another CSV FILE input step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');
    cout << "Do you want to add OUTPUT step?Y/N:" << endl;
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y')
        do
        {
            ofstream file("flows.txt", ios::app);
            if (file.is_open())
            {
                file << "OUTPUT step:" << endl;
                file << "step: " << endl;
                file << "name of file: " << endl;
                file << "title: " << endl;
                file << "description: " << endl;
                file.close();
                cout << "Do you want to add another OUTPUT step?Y/N:" << endl;
                cin >> confirm;
            }
            else
            {
                cout << "Unable to open file." << endl;
            }
        }
        while (confirm == 'Y' || confirm == 'y');

    EndStep endstep;
    endstep.execute();
}
void run_flow(int b[100][100], int av[100][100], int &avg)
{
    cout << "Type the name of the flow you want to run: ";
    string flowName;
    cin >> flowName;
    float a[10];
    int i = 1, index = 0, numbers = 0, linia = 0;
    char c[200][200];
    ifstream file("flows.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            int ok = 0;
            if (line.find(flowName) != string::npos)
            {
                b[index][0]++;
                ok = 1;
                cout << line << endl;
                getline(file, line);
                cout << line << endl;
                while (getline(file, line))
                {
                    if (line == "TITLE step:")
                    {
                        cout << "Do you want to skip TITLE step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                        }
                    }
                    else if (line == "TEXT step:")
                    {
                        cout << "Do you want to skip TEXT step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);

                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << endl;
                        }
                    }
                    else if (line == "TEXT input step:")
                    {
                        cout << "Do you want to skip TEXT INPUT step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            Text_InputStep textinputstep;
                            string description, input;
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line;
                            cin.ignore();
                            getline(cin,description);
                            strcat(c[linia], description.c_str());
                            textinputstep.setdescription(description);
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line;
                            cin.ignore();
                            getline(cin,input);
                            strcat(c[linia], input.c_str());
                            textinputstep.setinput(input);
                        }
                    }
                    else if (line == "NUMBER input step:")
                    {
                        cout << "Do you want to skip NUMBER INPUT step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            string linep1 = line, linep2, linep3;
                            Number_InputStep numberinputstep;
                            string description;
                            float input;
                            cout << line << endl;
                            getline(file, line);
                            linep2 = line;
                            cout << line << " ";
                            cin.ignore();
                            getline(cin,description);
                            numberinputstep.setdescription(description);
                            getline(file, line);
                            linep3 = line;
                            int ok=1;
                            do
                            {
                                try
                                {
                                    cout << line << " ";
                                    cin >> input;
                                    if (cin.fail())
                                    {
                                        b[index][2]++;
                                        throw ios_base::failure("Invalid input. Do you want to try again?");
                                    }
                                    else break;
                                }
                                catch(const ios_base::failure& e)
                                {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << e.what() << "Y/N:";
                                    char conf;
                                    cin>>conf;
                                    if(!(conf == 'y' || conf == 'Y'))
                                    {
                                        break;
                                        ok = 0;
                                    }
                                }
                            }
                            while(1);
                            if(ok==1)
                            {
                                numbers++;
                                strcpy(c[linia++], linep1.c_str());
                                strcpy(c[linia++], linep2.c_str());
                                strcat(c[linia], description.c_str());
                                strcpy(c[linia++], linep3.c_str());
                                strcat(c[linia], to_string(input).c_str());
                                numberinputstep.setinput(input);
                                a[i] = numberinputstep.getInput();
                                i++;
                            }
                        }
                    }
                    else if (line == "CALCULUS step:")
                    {
                        cout << "Do you want to skip CALCULUS step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            string linep1 = line, linep2, linep3, linep4, operation;
                            CalculusStep calculusstep;
                            int step1, step2;
                            cout << line << endl;
                            getline(file, line);
                            linep2 = line;
                            int ok = 1;
                            do
                            {
                                try
                                {
                                    cout << line << " ";
                                    cin >> step1;
                                    if (cin.fail() || step1 > numbers)
                                    {
                                        b[index][2]++;
                                        throw ios_base::failure("Invalid input or you typed a bigger number. Do you want to try again?");
                                    }
                                    else break;
                                }
                                catch(const ios_base::failure& e)
                                {
                                    cin.clear();
                                    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                                    cout << e.what() << "Y/N:";
                                    char conf;
                                    cin>>conf;
                                    if(!(conf == 'y' || conf == 'Y'))
                                    {
                                        break;
                                        ok = 0;
                                    }
                                }
                            }
                            while(1);
                            if(ok==1)
                            {
                                calculusstep.setstep1(a[step1]);
                                getline(file, line);
                                linep3 = line;
                                int ok2=1;
                                do
                                {
                                    try
                                    {
                                        cout << line << " ";
                                        cin >> step2;
                                        if (cin.fail() || step2 > numbers)
                                        {
                                            b[index][2]++;
                                            throw ios_base::failure("Invalid input or you typed a bigger number. Do you want to try again?");
                                        }
                                        else break;
                                    }
                                    catch(const ios_base::failure& e)
                                    {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << e.what() << "Y/N:";
                                        char conf;
                                        cin>>conf;
                                        if(!(conf == 'y' || conf == 'Y'))
                                        {
                                            break;
                                            ok2 = 0;
                                        }
                                    }
                                }
                                while(1);
                                if(ok2==1)
                                {
                                    calculusstep.setstep2(a[step2]);
                                    getline(file, line);
                                    linep4 = line;
                                    int ok3 = 1;
                                    do
                                    {
                                        try
                                        {
                                            cout << line << " ";
                                            string operation1;
                                            cin.ignore();
                                            getline(cin,operation1);
                                            if (operation1 != "-" && operation1 != "+" && operation1 != "*" && operation1 != "/" && operation1 != "min" && operation1 != "max")
                                            {
                                                b[index][2]++;
                                                throw ios_base::failure("Invalid input. Do you want to try again?");
                                            }
                                            else
                                            {
                                                operation = operation1;
                                                break;
                                            }
                                        }
                                        catch(const ios_base::failure& e)
                                        {
                                            cout << e.what() << "Y/N:";
                                            char conf;
                                            cin>>conf;
                                            if(!(conf == 'y' || conf == 'Y'))
                                            {
                                                break;
                                                ok3 = 0;
                                            }
                                        }
                                    }
                                    while(1);
                                    if(ok3 == 1)
                                    {
                                        strcpy(c[linia++], linep1.c_str());
                                        strcpy(c[linia++], linep2.c_str());
                                        strcat(c[linia], to_string(step1).c_str());
                                        strcpy(c[linia++], linep3.c_str());
                                        strcat(c[linia], to_string(step2).c_str());
                                        strcpy(c[linia++], linep4.c_str());
                                        strcat(c[linia], operation.c_str());
                                        calculusstep.setoperation(operation);
                                        calculusstep.execute();
                                    }
                                }
                            }
                        }
                    }
                    else if (line == "DISPLAY step:")
                    {
                        cout << "Do you want to skip DISPLAY step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            string step;
                            cout << line << " ";
                            cin.ignore();
                            getline(cin,step);
                            strstr(c[linia], step.c_str());
                            DisplayStep displaystep(step);
                            displaystep.execute();
                        }
                    }
                    else if (line == "TEXT FILE input step:")
                    {
                        cout << "Do you want to skip TEXT FILE input step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            TextFileInputStep textfileinputstep;
                            string description, fileName;
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << " ";
                            cin.ignore();
                            getline(cin,description);
                            strstr(c[linia], description.c_str());
                            textfileinputstep.setdescription(description);
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << " ";
                            getline(cin, fileName);
                            strstr(c[linia], fileName.c_str());
                            textfileinputstep.setfileName(fileName);
                            textfileinputstep.execute();
                        }
                    }
                    else if (line == "CSV FILE input step:")
                    {
                        cout << "Do you want to skip CSV FILE step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            strcpy(c[linia++], line.c_str());
                            CsvFileInputStep csvfileinputstep;
                            string description, fileName;
                            cout << line << endl;
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << " ";
                            cin.ignore();
                            getline(cin,description);
                            strstr(c[linia], description.c_str());
                            csvfileinputstep.setdescription(description);
                            getline(file, line);
                            strcpy(c[linia++], line.c_str());
                            cout << line << " ";
                            getline(cin,fileName);
                            strstr(c[linia], fileName.c_str());
                            csvfileinputstep.setfileName(fileName);
                            csvfileinputstep.execute();
                        }
                    }
                    else if(line == "OUTPUT step:")
                    {
                        cout << "Do you want to skip OUTPUT step?Y/N:" << endl;
                        char confirm;
                        cin >> confirm;
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            b[index][1]++;
                            getline(file, line);
                            getline(file, line);
                            getline(file, line);
                            getline(file, line);
                        }
                        else
                        {
                            string linep1 = line, linep2;
                            string description, fileName, title;
                            int step;
                            cout << line << endl;
                            getline(file, line);
                            linep2 = line;
                            int findit = -1;
                            int ok2=1;
                            do
                            {
                                int step_ver = 0;
                                int ok=1;
                                do
                                {
                                    try
                                    {
                                        cout << line << " ";
                                        cin >> step;
                                        if (cin.fail())
                                        {
                                            b[index][2]++;
                                            throw ios_base::failure("Invalid input. Do you want to try again?");
                                        }
                                        else break;
                                    }
                                    catch(const ios_base::failure& e)
                                    {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << e.what() << "Y/N:";
                                        char conf;
                                        cin>>conf;
                                        if(!(conf == 'y' || conf == 'Y'))
                                        {
                                            break;
                                            ok = 0;
                                        }
                                    }
                                }
                                while(1);
                                if(ok==1)
                                {
                                    for(int i = 0 ; i < linia ; i++)
                                    {
                                        if(strstr(c[i],"step"))
                                            step_ver++;
                                        if(step == step_ver)
                                        {
                                            findit = i;
                                            break;
                                        }
                                    }
                                    if(findit == -1)
                                    {
                                        cout << "Wrong step!" << endl;
                                        b[index][2]++;
                                    }
                                }
                                else
                                {
                                    ok2 = 0;
                                    break;
                                }
                            }
                            while(findit == -1);
                            if(ok2 == 1)
                            {
                                strcpy(c[linia++], linep1.c_str());
                                strcpy(c[linia++], linep2.c_str());
                                strcat(c[linia], to_string(step).c_str());
                                getline(file, line);
                                strcpy(c[linia++], line.c_str());
                                cout << line << " ";
                                cin.ignore();
                                getline(cin,fileName);
                                strstr(c[linia], fileName.c_str());
                                getline(file, line);
                                cout << line << " ";
                                cin.ignore();
                                getline(cin,title);
                                strstr(c[linia], title.c_str());
                                getline(file, line);
                                cout << line << " ";
                                cin.ignore();
                                getline(cin,description);
                                strstr(c[linia], description.c_str());
                                OutputStep outputStep(step,fileName,title,description);
                                outputStep.execute(c, findit);
                            }
                        }
                    }
                    else if (line == "End of the flow")
                    {
                        cout << line << endl;
                        break;
                    }
                }
            }

            if (ok == 1)
            {
                av[index][avg]=b[index][2]-av[index][avg-1];
                avg++;
                file.close();
                break;
            }
            index++;
        }
    }
    else
    {
        cout << "Unable to open file." << endl;
    }
}

void delete_flow()
{
    string flowName;
    cout << "Enter the name of the flow to delete: ";
    cin >> flowName;

    ifstream inputFile("flows.txt");
    ofstream tempFile("temp.txt");

    if (!inputFile || !tempFile)
    {
        cout << "Error opening file." << std::endl;
        return;
    }

    string line;
    int ok = 1;

    while (getline(inputFile, line))
    {
        size_t cautare = line.find(flowName);
        if (cautare != string::npos)
        {
            ok = 0;
        }
        if (ok == 1)
        {
            tempFile << line << endl;
        }
        if (line == "End of the flow" && ok == 0)
        {
            ok = 1;
            tempFile << endl;
        }
    }
    inputFile.close();
    tempFile.close();
    remove("flows.txt");
    rename("temp.txt", "flows.txt");
}

void show_analytics(int b[100][100], int av[100][100], int avg)
{
    string flowName;
    char confirm;
    ifstream file("flows.txt");
    do
    {
        cout << "Enter the name of the flow to show analytics: ";
        cin >> flowName;
        int index = 0;
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                if (line.find(flowName) != string::npos)
                {
                    cout << "The flow " << flowName << " was run " << b[index][0] << " times." << endl;
                    cout << "The flow has been skipped " << b[index][1] << " times." << endl;
                    cout << "The flow had " << b[index][2] << " errors" << endl;
                    float ma = 0;
                    for(int i = 0 ; i < avg ; i++)
                            ma+=av[index][i];
                        ma/=avg;
                    cout << "The flow has an average of " << ma << " errors per flow completed" << endl;
                }
                index++;
            }
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
        cout << "Do you want to show analytics for another flow?Y/N:" << endl;
        cin >> confirm;
    }
    while(confirm == 'Y' || confirm == 'y');
}
void question()
{
    int x;
    int b[100][100], av[100][100], avg = 0;
    do
    {
        cout << "What do you want to do?" << endl;
        cout << "1. Create a flow" << endl;
        cout << "2. Run a flow" << endl;
        cout << "3. Delete a flow" << endl;
        cout << "4. Show analytics" << endl;
        cout << "5. Exit" << endl;
        cin >> x;
        if (x == 1)
        {
            cout << "Create a flow" << endl;
            create_flow();
        }
        else if (x == 2)
        {
            cout << "Run a flow" << endl;
            run_flow(b,av,avg);
        }
        else if (x == 3)
        {
            cout << "Delete a flow" << endl;
            delete_flow();
        }
        else if (x == 4)
        {
            cout << "Show analytics" << endl;
            show_analytics(b,av,avg);
        }
        else
            cout << "Inchidem aplicatia" << endl;
    }
    while (x != 5);
}
int main()
{
    cout << "Welcome!" << endl;
    question();
    return 0;
}
