#include "all_command.h"

using namespace std ;

command program ;

string command_line ;

int main()
{
    file_initialize() ;

//    freopen( "../../../data_out.txt" , "w" , stdout ) ;

    while (getline(cin,command_line)){
        try{
            program.get_command(command_line) ;
            program.run_command() ;
        } catch (invalid_argument &x) {
            cout << "Invalid" << endl ;
            //cout << x.what() << endl ;
        }
    }

    return 0;
}