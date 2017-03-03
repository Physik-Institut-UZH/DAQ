#include <fstream>
#include <string>
using namespace std;



int Threshold_Generator(){
    fstream f;
    f.open("SpecificTreshold.txt", ios::out); 
    for(int j=0;j<1;j++){
      f << j << " ";
      for (int i=1;i<=40;i++){
		f << i << " " ;
      }
      f << "\r\n";
   }
    f.close();
return 0;
}
