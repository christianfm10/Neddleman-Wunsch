#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <tuple>
#include <regex>
#include <sstream>
#include <fstream>
#include <cctype>

using namespace std;


vector<string> preprocesamiento(ifstream &file){
    string linea;
    vector<string> Secuencias;
    string chain;

    while(getline(file, linea)){
        if(linea.empty() || linea.size()<2){
            // cout<<chain<<endl;
            if(chain!="")
                Secuencias.push_back(chain);
            chain = "";
        }
        else{
            if (linea.size() > 15)
                chain = chain + linea;
        }
    }
    Secuencias.push_back(chain);
    cout<<Secuencias.size()<<endl;
    regex numeros_espacios("[^a-zA-Z]");
    for(int i=0; i<Secuencias.size(); i++) {
        string resultado = regex_replace(Secuencias[i], numeros_espacios, "");
        Secuencias[i] = resultado;
        cout<<resultado<<endl;
        cout<<"-----------------------------------------------------------"<<endl;
    }

    return Secuencias;
}

#define GAP_PENALTY -2
#define MISMATCH_SCORE -1
#define MATCH_SCORE 1

int max3(int a, int b, int c);
int match_or_mismatch(char c1, char c2);
void printmatrix(int n, int m, int* M);
string neddleman_col(int i, int j, string alignment, string aligment2, int *M);
int score(char c1, char c2);
string s1, s2;
int n, m;




vector<pair<string,string>> backtrack(int i,int j,string align1,string align2, int *Matrix) {
    vector<pair<string, string>> results;
    if (i==0 && j==0) {
        pair<string, string> par = {align1, align2};
        results.push_back(par);
        return results;
    }

    if (i > 0 && j > 0 && Matrix[i*(m+1)+j] == Matrix[(i-1)*(m+1)+j-1] + match_or_mismatch(s1[i-1], s2[j-1])) {
        vector<pair<string,string>> new_results = backtrack(i-1, j-1, s1[i-1]+align1, s2[j-1]+align2, Matrix);
        results.insert(results.end(), new_results.begin(), new_results.end());
    }
        
    if (i > 0 && Matrix[i*(m+1)+j] == Matrix[(i-1)*(m+1)+j] + GAP_PENALTY) {
        vector<pair<string,string>> new_results = backtrack(i-1, j, s1[i-1] + align1, "-"+align2, Matrix);
        results.insert(results.end(), new_results.begin(), new_results.end());
    }
    if (j > 0 && Matrix[i*(m+1)+j] == Matrix[(i)*(m+1)+j-1] + GAP_PENALTY) {
        vector<pair<string, string>> new_results = backtrack(i, j-1, "-"+align1, s2[j-1]+align2, Matrix);
        results.insert(results.end(), new_results.begin(), new_results.end());
    }
    return results;
}

int contarGaps(const std::string& alineamiento) {
    return std::count(alineamiento.begin(), alineamiento.end(), '-');
}

pair<string, string> encontrar_alineamineto_con_menos_gaps (const vector<pair<string, string>>& alineamientos) {
    auto mejorAlineamiento = alineamientos[0];
    int minGaps = contarGaps(alineamientos[0].first) + contarGaps(alineamientos[0].second);

    // Iteramos sobre el resto de los alineamientos
    for (const auto& alineamiento : alineamientos) {
        int gaps = contarGaps(alineamiento.first) + contarGaps(alineamiento.second);
        if (gaps < minGaps) {
            mejorAlineamiento = alineamiento;
            minGaps = gaps;
        }
    }

    return mejorAlineamiento;

}

int main(int argc, char* argv[])
{
    string filename = "Sequencias.txt";
    ifstream file(filename);
    ofstream OutputFile("output.txt");

    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return 0;
    }
    vector<string> secuencias = preprocesamiento(file);
    
    OutputFile<<"Secuencia 1"<<endl;
    OutputFile<<secuencias[0]<<endl;
    OutputFile<<"Secuencia 2"<<endl;
    OutputFile<<secuencias[1]<<endl;
    OutputFile<<"_____________________________________________________"<<endl;
    s1 = secuencias[0], s2 = secuencias[1];
    n = s1.length(), m = s2.length();
    
    int Matrix[n+1][m+1];
    
    Matrix[0][0] = 0;
    for (int i=1;i<n+1;i++)
    {
        Matrix[i][0] = Matrix[i-1][0] + GAP_PENALTY;
    }
    for (int i=1;i<m+1;i++)
    {
        Matrix[0][i] = Matrix[0][i-1] + GAP_PENALTY;
    }
    
    for (int i=1;i<n+1;i++)
    {
        for (int j=1;j<m+1;j++)
        {
            Matrix[i][j] = max3(Matrix[i-1][j-1] + match_or_mismatch(s1[i-1], s2[j-1]),
                          Matrix[i][j-1] + GAP_PENALTY,
                          Matrix[i-1][j] + GAP_PENALTY);
        }
    }
    // printmatrix(n+1, m+1, &Matrix[0][0]);
    cout<<"Best score: "<< Matrix[n][m]<<endl;
    OutputFile<<"Best score:"<< Matrix[n][m]<<endl;
    vector<pair<string,string>> results = backtrack(n , m , "", "", &Matrix[0][0]);
    // cout<< results.size()<< endl;
    // OutputFile<<"Los alineamientos mas optimos"<<endl;
    // for (int i=0; i < results.size(); i=i+1){
    //     OutputFile<<"----------------------------------------------------------------"<<endl;
    //     cout<<"----------------------------------------------------------------"<<endl;
    //     cout<<results[i].first<<endl;
    //     OutputFile<<results[i].first<<endl;
    //     cout<<results[i].second<<endl;
    //     OutputFile<<results[i].second<<endl;
    //     cout<<"----------------------------------------------------------------"<<endl;
    //     OutputFile<<"----------------------------------------------------------------"<<endl;
    // }
    pair<string, string> mejor_alineamiento;
    mejor_alineamiento = encontrar_alineamineto_con_menos_gaps(results);
    
    cout<<"MEJOR ALINEAMIENTO CON MENOS GAPS"<<endl;
    OutputFile<<"MEJOR ALINEAMIENTO CON MENOS GAPS"<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
    OutputFile<<"----------------------------------------------------------------"<<endl;
    cout<<mejor_alineamiento.first<<endl;
    OutputFile<<mejor_alineamiento.first<<endl;
    cout<<mejor_alineamiento.second<<endl;
    OutputFile<<mejor_alineamiento.second<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
    OutputFile<<"----------------------------------------------------------------"<<endl;
    OutputFile.close();
    

    return 0;
}


int max3(int a, int b, int c)
{
    if (a >= b && a >= c) return a;
    else if (b >= a && b >= c) return b;
    else return c;
}


int match_or_mismatch(char c1, char c2)
{
    return (c1 == c2) ? MATCH_SCORE : MISMATCH_SCORE;
}

void printmatrix(int n, int m, int* Matrix)
{
    int count =0;
    for (int i=0;i<(n*m);i++)
    {
        if (Matrix[i]>= 0)
        {
            std::cout << " " << Matrix[i] << "  ";
        }
        else
        {
            std::cout << Matrix[i] << "  ";
        }
        
        count++;
        if (count == m)
        {
            std::cout << std::endl;
            count = 0;
        }
    }
}