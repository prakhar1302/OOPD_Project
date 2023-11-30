#include <bits/stdc++.h>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>

using namespace std;
namespace fs = std::filesystem;

bool interactive()
{
    char ch;
    cout<<"Do you wanna proceed with the action? Y/n ";
    cin>>ch;
    cout<<endl;
    if(ch == 'Y' || ch =='y')
    {
        return true;
    }
    return false;
}

class Shell {
public:
    void cd(vector<string> args);
    void mv(vector<string> args);
    void rm(vector<string> args);
    void ls(vector<string> args);
    void cp(vector<string> args);
    string pwd();
    void executeCommand(const string& command);

private:
};

void Shell::executeCommand(const string& command)
{
    istringstream iss(command);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(tokens));

    if (tokens.size() > 0) {
        if (tokens[0] == "ls") {
            ls(tokens);
        } else if (tokens[0] == "cd") {
            cd(tokens);
        } else if (tokens[0] == "pwd") {
            string str = pwd();
            if(str.length()>0)
                cout<<str<<endl;
            else
                cout<<"pwd failed."<<endl;
        } else if (tokens[0] == "mv") {
            mv(tokens);
        } else if (tokens[0] == "rm") {
            rm(tokens);
        // } else if (tokens[0] == "cp") {
        //     cp(tokens);
        } else {
            cout << "Unknown command: " << tokens[0] << endl;
        }
        }
}

void Shell::ls(vector<string> args) {
        DIR *dir;
        struct dirent *ent;
        regex wildcard_regex(".*");

        if (args.size() > 2) {
            cout << "Usage: ls <directory>" << endl;
            return;
        }

        string dir_path = args.size() == 2 ? args[1] : ".";
        if ((dir = opendir(dir_path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (regex_match(ent->d_name, wildcard_regex)) {
                    cout << ent->d_name << endl;
                }
            }
            closedir(dir);
        } else {
            cout << "Error: cannot open directory " << dir_path << endl;
        }
    }

void Shell::cd(vector<string> args) {
        if (args.size() != 2) {
            cout << "Usage: cd <directory>" << endl;
        } else {
            if (chdir(args[1].c_str()) != 0) {
                cout << "Error: cannot change directory to " << args[1] << endl;
            }
        }
    }

string Shell::pwd() {
    const int buffer_size = 256; // You can adjust the buffer size as needed
    char buffer[buffer_size];

    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return string(buffer);
    } else {
        // Handle error, for example:
        perror("getcwd");
        return ""; // Return an empty string on error
    }
}
void Shell::mv(vector<string> args) {
        if (args.size() != 3) {
            cout << "Usage: mv <source> <destination>" << endl;
        } else {
            if (rename(args[1].c_str(), args[2].c_str()) != 0) {
                cout << "Error: cannot move " << args[1] << " to " << args[2] << endl;
            }
        }
    }

void Shell::rm(vector<string> args) {
    if(args.size() == 2 && args[1] != "-i" && args[1]!= "-r")
        {
            if(remove(args[1].c_str())!=0)
                cout<<"File could not be deleted"<<endl;
        }
    else
    if(args[1] == "-i")
    {
        if(interactive())
        {
            if(args[2] != "-r" && remove(args[2].c_str())!=0)
                cout<<"File could not be deleted"<<endl;
            else if(args[2] == "-r")
                try {
                if (fs::is_directory(args[3])) {
                    // Remove directory and its contents recursively
                    fs::remove_all(args[3]);
                    std::cout << "Directory '" << args[3] << "' and its contents removed successfully." << std::endl;
                } else {
                    // Remove file
                    fs::remove(args[3]);
                    std::cout << "File '" << args[3] << "' removed successfully." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    else if(args[1] == "-r")
    {
        try {
                if (fs::is_directory(args[3])) {
                    // Remove directory and its contents recursively
                    fs::remove_all(args[3]);
                    std::cout << "Directory '" << args[3] << "' and its contents removed successfully." << std::endl;
                } else {
                    // Remove file
                    fs::remove(args[3]);
                    std::cout << "File '" << args[3] << "' removed successfully." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
    }
}
}

int main() {
    Shell myShell;
    string command;
    int i = 0;
    while (i<=14) {
        cout << "> ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }
        myShell.executeCommand(command);
        i++;
    }

    return 0;
}