#include <bits/stdc++.h>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <dirent.h>
#include <sys/types.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;
using namespace std::chrono_literals;

bool interactive()
{
    char ch;
    cout<<"Do you wanna proceed with the action? Y/n ";
    cin>>ch;
    cin.ignore();
    if(ch == 'Y' || ch =='y')
    {
        return true;
    }
    return false;
}

void listFilesRecursively(const fs::path& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path().filename().string() << endl;

        if (fs::is_directory(entry.path())) {
            cout << "--";
            std::thread(listFilesRecursively, entry.path()).detach();
        }
    }
}

void moveFilesRecursively(const fs::path& source, const fs::path& destination) {
    std::vector<std::thread> threads;

    for (const auto& entry : fs::directory_iterator(source)) {
        fs::path newDestination = destination / entry.path().filename();

        threads.emplace_back([entry, newDestination]() {
            try {
                fs::rename(entry, newDestination);

                if (fs::is_directory(entry)) {
                    moveFilesRecursively(entry, newDestination);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error moving file: " << e.what() << std::endl;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void removeRecursively(const fs::path& path) {
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            std::vector<std::thread> threads;

            for (const auto& entry : fs::directory_iterator(path)) {
                if (fs::is_directory(entry)) {
                    threads.emplace_back([entry]() {
                        removeRecursively(entry.path());
                    });
                } else {
                    fs::remove(entry);
                }
            }

            for (auto& thread : threads) {
                thread.join();
            }

            fs::remove(path);  // Remove the directory itself after its contents have been deleted
            std::cout << "Directory removed: " << path << std::endl;
        } else if (fs::exists(path) && fs::is_regular_file(path)) {
            fs::remove(path);
            std::cout << "File removed: " << path << std::endl;
        } else {
            std::cerr << "Path does not exist: " << path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error removing path: " << e.what() << std::endl;
    }
}

void copyRecursively(const fs::path& source, const fs::path& destination) {
    try {
        if (fs::exists(source)) {
            if (fs::is_directory(source)) {
                fs::create_directories(destination);

                std::vector<std::thread> threads;

                for (const auto& entry : fs::directory_iterator(source)) {
                    threads.emplace_back([entry, destination]() {
                        copyRecursively(entry.path(), destination / entry.path().filename());
                    });
                }

                for (auto& thread : threads) {
                    thread.join();
                }
            } else if (fs::is_regular_file(source)) {
                fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
            }
        } else {
            std::cerr << "Source path does not exist: " << source << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error copying path: " << e.what() << std::endl;
    }
}

void move(string x,string y)
{
    string sourcePath = x,destinationPath = y;
        fs::path source(sourcePath);
        fs::path destination(destinationPath);
        if (std::filesystem::exists(source)) {
            if (std::filesystem::is_regular_file(source)) {
                std::filesystem::rename(source,destination);
            }
        else{
        try {
            if (fs::exists(source) && fs::is_directory(source)) {
                if (!fs::exists(destination)) {
                    fs::create_directory(destination);
                }

                moveFilesRecursively(source, destination);
                fs::remove(source); 
            } else {
                std::cerr << "Invalid source directory path or not a directory." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
        }
        }
}

void copy(string sourceFilePath,string destinationFilePath)
{
    std::ifstream sourceFile(sourceFilePath, std::ios::binary);
    std::ofstream destinationFile(destinationFilePath, std::ios::binary);

    if (!sourceFile.is_open()) {
        std::cerr << "Error opening source file: " << sourceFilePath << std::endl;
        return;
    }

    if (!destinationFile.is_open()) {
        std::cerr << "Error opening destination file: " << destinationFilePath << std::endl;
        return;
    }

    destinationFile << sourceFile.rdbuf();

    sourceFile.close();
    destinationFile.close();

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
        } else if (tokens[0] == "cp") {
            cp(tokens);
        } else {
            cout << "Unknown command: " << tokens[0] << endl;
        }
        }
}

void Shell::ls(vector<string> args) {
    if(args[1] == "-help")
    {
        cout<<"\"ls\" will display the contents of the current directory. By default, ‘ls’ lists files and directories in alphabetical order."<<endl;
        cout<<"-l : a detailed list of files."<<endl;
        cout<<"-a : a list containing all the files including hidden files."<<endl;
        cout<<"-r : recursively display the connect of all the directory present."<<endl;
    }
    else
    if(args[1] == "-l")
    {
        for (const auto & entry : fs::directory_iterator("."))
        {
            struct stat fileInfo;

            if (stat(".", &fileInfo) != 0) {
                perror("stat");
                return;
            }

            if (fs::is_directory(entry.path())) {
                printf("d");
            } else {
                printf("-");
            }

            for (int i = 8; i >= 0; i--) {
                int mask = 1 << i;
                switch (fileInfo.st_mode & mask) {
                    case S_IRUSR:
                        printf("r");
                        break;
                    case S_IWUSR:
                        printf("w");
                        break;
                    case S_IXUSR:
                        printf("x");
                        break;
                    default:
                        printf("-");
                }
            }

            cout<<entry.path().filename().string() <<"\t";
            if (fs::is_regular_file(entry.path())) {
                std::cout << "File\t";
            } else if (fs::is_directory(entry.path())) {
                std::cout << "Directory\t";
            } else {
                std::cout << "Other\t";
            }
            cout<<endl;
        }
    }
    else if(args[1] == "-a")
    {
        for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().filename().string().front() == '.') 
            std::cout << entry.path().filename().string() <<"(Hidden)"<< std::endl;
        else
            cout<<entry.path().filename().string() <<endl;
    }
    }
    else if(args[1] == "-R")
    {
        std::vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(".")) {
            entries.push_back(entry);
        }
        std::sort(entries.rbegin(), entries.rend(), [](const auto& a, const auto& b) {
            return a.path().filename() < b.path().filename();
        });
        for (const auto& entry : entries) {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
    else if(args[1] == "-r")
    {
        string directoryPath = ".";
        try {
        fs::path path(directoryPath);

        if (fs::exists(path) && fs::is_directory(path)) {
            listFilesRecursively(path);
        } else {
            std::cerr << "Invalid directory path or not a directory." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    }
    else
    {
        for (const auto& entry : fs::directory_iterator(".")) {
            cout<<entry.path().filename().string() <<"\t";
        }
    }
    }

void Shell::cd(vector<string> args) {
        if(args[1] == "-help")
        {
            cout<<"cd command in Linux known as the change directory command. It is used to move efficiently from the current working directory to different directories in our System."<<endl;
        }
        if (args.size() != 2) {
            cout << "Usage: cd <directory>" << endl;
        } else {
            if (chdir(args[1].c_str()) != 0) {
                cout << "Error: cannot change directory to " << args[1] << endl;
            }
        }
    }

string Shell::pwd() {
    const int buffer_size = 256; 
    char buffer[buffer_size];

    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return string(buffer);
    } else {
        perror("getcwd");
        return ""; 
    }
}
void Shell::mv(vector<string> args) {
        if(args[1] == "-help")
        {
            cout<<"mv command is used to rename file directories and move files from one location to another within a file system. "<<endl;
            cout<<"-i : for interactive move."<<endl;
            cout<<"-r : for recursively move a directory and its content."<<endl;
        }
        else
        if(args[1] == "-i")
        {
            if(interactive())
            {
                if(args[2] == "-r")
                {
                    move(args[3],args[4]);
                }
                else
                {
                    move(args[2],args[3]);
                }
            }
        }
        else
        if(args[1] == "-r")
        {
            move(args[2],args[3]);
        }
        else
        {
            move(args[1],args[2]);
        }
        
}

void Shell::rm(vector<string> args) {
    if(args[1] == "-help")
        {
            cout<<"rm command is used to remove objects such as files, directories, symbolic links and so on from the file system like UNIX.  "<<endl;
            cout<<"-i : for interactive remove."<<endl;
            cout<<"-r : for recursively remove a directory and its content."<<endl;
        }
    else
    if(args[1] == "-i")
    {
        if(interactive() == true)
        {
            if(args[2] == "-r")
            {
                string targetPath = args[3];
                fs::path pathToRemove(targetPath);
                removeRecursively(pathToRemove);
            }
            else
            {
            string targetPath = args[2];
            fs::path pathToRemove(targetPath);
            removeRecursively(pathToRemove);
            }
        }
    }
    else
    if(args[1] == "-r")
    {
        string targetPath = args[2];
        fs::path pathToRemove(targetPath);
        removeRecursively(pathToRemove);
    }
    else
    {string targetPath = args[1];
    fs::path pathToRemove(targetPath);
    removeRecursively(pathToRemove);}
}

void Shell::cp(vector<string> args){
    if(args[1] == "-help")
    {
        cout<<"cp stands for a copy. This command is used to copy files or groups of files or directories."<<endl;
        cout<<"-i : for interactive copy."<<endl;
        cout<<"-r : for recursively copy a directory and its content."<<endl;
    }
    if(args[1] == "-i")
    {
        if(interactive() == true)
        {   if(args[2] == "-r")
            {
                string sourcePath = args[3],destinationPath = args[4];
                fs::path source(sourcePath);
                fs::path destination(destinationPath);
                if(!(fs::is_directory(source) && fs::is_directory(destination)))
                    copy(source,destination);
                else
                    copyRecursively(source, destination);
                    
            }
            else
            {
                string sourcePath = args[2],destinationPath = args[3];
                fs::path source(sourcePath);
                fs::path destination(destinationPath);
                copy(args[2],args[3]);
            }
        }
    }
    else if(args[1] == "-r")
    {
        string sourcePath = args[2],destinationPath = args[3];
        fs::path source(sourcePath);
        fs::path destination(destinationPath);
        if(!(fs::is_directory(source) && fs::is_directory(destination)))
            copy(source,destination);
        else
            copyRecursively(source, destination);
    }
    else
    {
        copy(args[1],args[2]);
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