#include "all_command.h"

using namespace std ;

command program ;

string command_line ;

int main()
{
    file_initialize() ;

    while (true){
        try{
            getline(cin,command_line) ;
            program.get_command(command_line) ;
            program.run_command() ;
        } catch (invalid_argument &x) {
            cout << "Invalid" << endl ;
            //cout << x.what() << endl ;
        }
    }

    return 0;
}