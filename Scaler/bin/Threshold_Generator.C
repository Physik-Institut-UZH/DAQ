#include <fstream>
using namespace std;



Threshold_Generator(){
    fstream f;
    f.open("SpecificTreshold.txt", ios::out); 
    for(int j=0;j<4;j++){
      f << j << " ";
      for (int i=1;i<=255;i++){
		f << i << " " ;
      }
      f << "\r\n";
   }
    f.close();
}
