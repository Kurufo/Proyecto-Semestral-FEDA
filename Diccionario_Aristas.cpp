#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <math.h>
#include <cstdlib>
#include <unordered_map>
#include <limits.h>
#include <chrono>
#include <sstream>
#include <fstream>
using namespace std;

class HashTable {
public:
  int size; //Capacidad máxima
  int carga=0;//Carga
  vector<pair<pair<string,string>,int>> table; 
  int hash(const string& key1,const string& key2 ) const{
      int hashVal = 0;
      for(char ch : key1) hashVal += ch;
      for(char ch : key2) hashVal += ch*ch*1000;
      return hashVal % size;
  }

  void rehash(){ 
    vector<pair<pair<string,string>,int>> newhash(2*size); 
    for(int i=0; i<size;i++){
      if(table[i].first.first!=""){
        int reha=hash(table[i].first.first, table[i].first.second);
        while(newhash[reha].first.first!="" && newhash[reha].first!=table[i].first){
          reha=(reha+1)%(2*size);
        }
        newhash[reha]=table[i];
      } 
    }
    table=newhash;
    size=2*size;
  }


  HashTable(int tam): size(tam), table(tam) {}

  ~HashTable(){

  } 

  void insert(const string &key1,const string &key2 ) { //Inserta una clave nueva
    int i = hash(key1, key2);
    pair<string,string> dupla={key1,key2};
    while(table[i].first.first != "" && table[i].first != dupla)  i = (i + 1) % size;
    
    if(table[i].first.first=="") table[i] = {dupla,1};
    else table[i].second++;
    carga+=1;
    if(carga/size>=0.75) {
        cout<<"It's rehashing time\n";
      rehash();
    }
  }

  
  size_t memoria(){//Devuelve la memoria usada por la clase
    int mem=0;
    for(int i=0;i<table.size();i++){
      mem+=sizeof(table[i])+sizeof(table[i].first.first)+sizeof(table[i].first.second)+sizeof(table[i].second);
    }
    return mem;
  }
};


double media(double *u, int tam){
    double m = 0;
    for (int i = 0; i < tam; i++)
    {
        m += u[i];
    }
    return m / tam;
}

double desviacion(double *u, int tam){
    double sd = 0;
    double m = media(u,tam);
    for (int i = 0; i < tam; i++)
    {
        sd += pow(u[i] - m, 2);
    }
    return pow(sd / (tam - 1), 1 / 2);
}


int main(){
    int pruebas=30;

    size_t memoria=0;

	double colusion[pruebas];
    double construccion[pruebas];

	string aux;

    

    for(int t = 0; t < pruebas; t++){

        
        HashTable tabla(200000);
        auto start=chrono::high_resolution_clock::now();
        ifstream inputFile;
        vector<string> temporal;
        int articulos=0;
        inputFile.open("autores.txt",ios::in);

        while(getline(inputFile, aux)){

            if(aux!="t"){
                temporal.push_back(aux);
                }
            

            else{
                for(int i=0;i<temporal.size();i++){
                    for(int j=0; j<temporal.size();j++){
                        if(i!=j){
                            if(i<j){
                                string key1, key2;
                                if(temporal[i]<temporal[j]){
                                    key1=temporal[i];
                                    key2=temporal[j];
                                }
                                else{
                                    key1=temporal[j];
                                    key2=temporal[i];
                                }
                                tabla.insert(key1,key2);
                            }
                        }
                    }
                }
                temporal.clear();
                articulos++;
            }
            if (articulos==1000000) break;
            
        }
    
	
	    inputFile.close();

        auto end= chrono::high_resolution_clock::now();
		double tiempo=chrono::duration_cast<chrono::milliseconds>(end-start).count();

        construccion[t]=tiempo;

        int coludido=0;

        int cant_aristas=0;

        auto start_2=chrono::high_resolution_clock::now();

        for(int i=0; i<tabla.table.size();i++){
            if(tabla.table[i].first.first!=""){
                cant_aristas++;
                if(tabla.table[i].second%2==0){
                    coludido++;
                }
            }
        }
        
        if(coludido>cant_aristas/2) cout<<"Existe colusion"<<endl;
        else cout<<"No existe colusion"<<endl;

        auto end_2= chrono::high_resolution_clock::now();
		double tiempo_2=chrono::duration_cast<chrono::milliseconds>(end_2-start_2).count();
        colusion[t]=tiempo_2;

        if(t==pruebas-1){
            memoria=tabla.memoria(); 
            cout<<"Coludidos: "<<coludido<<endl;           
        }

        cout<<"Prueba "<<t<<endl;
    }

    cout<<"Tiempo medio de crear el grafo: "<<media(construccion,pruebas)<<endl;
    cout<<"Tiempo medio para buscar colusiones: "<<media(colusion,pruebas)<<endl;
    cout<<"Espacio usado: "<<static_cast<int>(memoria)/1024<<endl;
}
