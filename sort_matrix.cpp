
#include "stl.h"
#include "/home/cobalt/pw52889/programs/FEM_Matrix/Transform.h"
#include <string.h>

#define FORMAT 12
#define PRECISION 4

struct numeric_string{

       numeric_string(const StringList& list){
       numbers = list.numbers();
       }

       numeric_string(const vector<double>& list){
       numbers = list;
       }

       numeric_string(){
       }
const double& operator[](const int& i) const { return numbers[i];}
double& operator[](const int& i) { return numbers[i];}

const int& size() const { return numbers.size(); }

vector<double> numbers;

};

//-------------------------------------
struct compare_dimension{
int dimension;

     compare_dimension(const int& i) { dimension = i; }
     bool operator()( const numeric_string& ns1,  const numeric_string& ns2){
     ////cout << dimension << '\n';
     return ns1.numbers[dimension] < ns2.numbers[dimension];
     }

};


//-------------------------------------------
class rectangular_matrix_functions{ // responsible for the rectangular matrix operations

public:
rectangular_matrix_functions(vector<numeric_string>& m):matrix(m){}
int transform(); // transform matrix rows become columns and vice versa
int construct_from_columns(const vector<int>& columns); // construct matrix from columns 
const vector<numeric_string>& array() const { return matrix;}
int sort_dimension(const int& dim) { 
                                     if ( dim < 0 || dim >= matrix.size()) return 1;
                                     sort(matrix.begin(), matrix.end(), compare_dimension(dim));
                                     return 0;
                                   }
private:
vector<numeric_string>& matrix;
};

//------------------------------------------------------------
int rectangular_matrix_functions::construct_from_columns(const vector<int>& columns){
vector<numeric_string> new_matrix;
      
        for(int i=0; i<matrix.size(); ++i){
        vector<double> new_string;

            for(int j=0; j<columns.size(); ++j){
            int col = columns[j]; 
                 if ( col>=0 &&  col < matrix[i].numbers.size() ){ 
                      new_string.push_back(matrix[i].numbers[col]);
                 }
            }

          if(new_string.size() > 0 ){
           numeric_string ns(new_string);
           new_matrix.push_back(ns); // add numbers only in the columns
          }

        }        
 
        if (new_matrix.size() > 0 ) matrix = new_matrix;      
return 0;
}

//------------------------------------------------------------
int rectangular_matrix_functions::transform(){
vector<numeric_string> new_matrix;
int columns = matrix[0].size();    
/////cout << " cols: " << columns << '\n';
          for(int j = 0; j < columns; ++j){
          vector<double> nv;

             for(int i = 0; i < matrix.size(); ++i)
             nv.push_back(matrix[i][j]);

          numeric_string ns(nv);
          new_matrix.push_back(ns); 
          }
matrix = new_matrix;
return 0;
}

//------------------------------------------------------------
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
         for(int i =0; i<v.size(); ++i)
          os << setw(FORMAT) << v[i];
return os;
}

//------------------------------------------------------------
ostream& operator<<(ostream& os, const numeric_string& ns){
          os << ns.numbers ;
return os;
}

//------------------------------------------------------------
ostream& operator<<(ostream& os, const vector<numeric_string>& matrix){
         
          for(int i=0; i<matrix.size(); ++i){
             os << matrix[i] << '\n';
          }
return os;
}

//------------------------------------------------------------

int main(int argc, char **argv){
string words;

    while( getline(cin,words) ){
    StringList list(words); // does not work properly if file contains empty strings
    //// list.print();
    char c = list[0][0] ; // read first letter 
    if ( isdigit(c) || c=='-') break;
    
    } 

    ////cout << words << '\n';

    //numeric_string ns(words);
    //cout << ns << '\n';

    int n = words.size(); // define number of the columns of the matrix
    vector<numeric_string> matrix;
    numeric_string ns(words);
    matrix.push_back(ns); // add a current row


    while( getline(cin,words) ){
    StringList list(words);
    ///cout << words <<'\n';

    char c = list[0][0] ; // read first letter 
    if ( !(isdigit(c) || c=='-') ) break;

    numeric_string ns(words);
    matrix.push_back(ns); // add a current row
    } 
    //cout << " initial array:\n";
    //cout << matrix;
    //cout << " sorted array:\n";

    int dim = 0;
    bool transform = false; 
    vector<int> columns; // accumulate number of the columns to write down

    int i = 1;
    for(;i<argc;++i) {//cout << argv[i] << '\n';
    if (strcmp("-n",argv[i]) == 0) dim = atoi(argv[i+1]) - 1; // substruct 1 because C++ indexing form 0
    if (strcmp("-column",argv[i]) == 0) {
                                         int n;
                                         for(int j=i+1; j < argc && ( n = atoi(argv[j]) ); ++j) 
                                         columns.push_back(n - 1); // substruct 1 because C++ indexing form 0
                                        }
    if (strcmp("-transform",argv[i]) == 0) transform = true;
  
    }// checked input of the program
    ///cout << columns << '\n';

    rectangular_matrix_functions matrix_functions(matrix);
    matrix_functions.sort_dimension(dim);

    if (columns.size() > 0) matrix_functions.construct_from_columns(columns);
    if (transform) matrix_functions.transform();
    cout << matrix;

return 0;
}
