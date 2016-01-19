#include <fstream>
using namespace std;



Threshold_Generator(){
    fstream f;
    f.open("SpecificTreshold.txt", ios::out);
    f << "0 " ;
    for (int i=1;i<256;i++)
		f << i << " " ; 
    f.close();


}
