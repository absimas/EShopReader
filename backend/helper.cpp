#include "helper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

//void log(string msg) {
//    // Output to file
//    ofstream file;
//    file.open("log");
//    file << msg << "\n";
//    file.close();
//
//    // STD output
//    cout << msg << std::endl;
//}
//

void log_err(string msg) {
    namespace pt = boost::posix_time;
    ostringstream ss;
    const pt::ptime now = pt::second_clock::local_time();
    pt::time_facet*const f = new pt::time_facet("%Y-%m-%d %H:%M:%S");
    ss.imbue(std::locale(ss.getloc(),f));
    ss << now << ": " << msg;
    cerr << ss.str() << endl;
}
