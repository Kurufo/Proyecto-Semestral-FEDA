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
    

    for(int t = 0; t < pruebas; t++){

        unordered_map<string,int> autor_num;
        string aux;
        int cont=0;
        int lineacout=0;

        auto start=chrono::high_resolution_clock::now();

        ifstream inputFile;
        inputFile.open("autores.txt",ios::in);
        
        while(getline(inputFile, aux)){
            if(aux!="t"){
                if(autor_num.find(aux)==autor_num.end() ){
                    autor_num[aux]=cont;
                    cont++;
                }
            }
            else lineacout++;
            if(lineacout==1000000) break;
        }
        
        inputFile.close();

        vector<vector<pair<int,int>>> adyacencia(cont);

        vector<int> temporal;
        lineacout=0;

        inputFile.open("autores.txt",ios::in);

        while(getline(inputFile, aux)){


            if(aux!="t"){
                temporal.push_back(autor_num[aux]);
                }
            

            else{
                lineacout++;
                for(int i=0;i<temporal.size();i++){
                    for(int j=0; j<temporal.size();j++){
                        if(i!=j){
                            int flag=0;
                            for(int x=0;x<adyacencia[temporal[i]].size();x++){
                                if(adyacencia[temporal[i]][x].first==temporal[j]){
                                    adyacencia[temporal[i]][x].second++;
                                    flag=1;
                                    break;
                                }
                            }
                            if(flag==0)adyacencia[temporal[i]].push_back({temporal[j],1});
                        }
                    }
                }
                temporal.clear();
            }
            if(lineacout==1000000) break;
        }
        
        inputFile.close();
        

        auto end= chrono::high_resolution_clock::now();
		double tiempo=chrono::duration_cast<chrono::milliseconds>(end-start).count();

        construccion[t]=tiempo;

        int coludido=0;
        int total_aristas=0;
        
        auto start_2=chrono::high_resolution_clock::now();
        for(int i=0; i<cont; i++){
            for(int j=0; j<adyacencia[i].size();j++){
                if(i<adyacencia[i][j].first){
                    total_aristas++;
                    if(adyacencia[i][j].second%2==0){                    
                        coludido++;
                    }
                }
            }
        }
        if(coludido>total_aristas/2) cout<<"Existe colusion"<<endl;
        else cout<<"No existe colusion"<<endl;
        auto end_2= chrono::high_resolution_clock::now();
		double tiempo_2=chrono::duration_cast<chrono::milliseconds>(end_2-start_2).count();

        colusion[t]=tiempo_2;

        if(t==pruebas-1){
            for(int i=0;i<adyacencia.size();i++){
                memoria+=sizeof(adyacencia[i]);
                if(adyacencia[i].size()>0){
                    for(int j=0;j<adyacencia[i].size();j++){
                        memoria+=sizeof(adyacencia[i][j]);
                    }

                }
            }
            
            cout<<"Coludidos: "<<coludido<<endl;

            unordered_map<string, int>::iterator it = autor_num.begin(); //Para revisqar los equipos participantes iteramos sobre los elementos del mapa. Para ello, usamos un iterador

            while(it!=autor_num.end()){ //Para cada equipo formamos una tupla de 3 elementos usando losdatos almacenados en la matriz, junto a la id del equipo. Luego agregamos esa tupla a una cola de prioridad
                memoria+=sizeof(it);
                ++it;
		    }

        }

        cout<<"Prueba "<<t<<endl;
    }

    cout<<"Tiempo medio de crear el grafo: "<<media(construccion,pruebas)<<endl;
    cout<<"Tiempo medio para buscar colusiones: "<<media(colusion,pruebas)<<endl;
    cout<<"Espacio usado: "<<static_cast<int>(memoria)/1024<<endl;
}