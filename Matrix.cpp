#include <iostream>
#include <vector>


using namespace std;

class Matrix {
public:
//constructor
    Matrix(int _filas, int _columnas){
        filas= _filas;
        columnas = _columnas;
        for(int i{1};i<=filas*columnas;i++){
            datos.push_back(0);
        }


}


    float getElement (int fila, int columna ){
        if (fila <= 0 || columna <= 0){
            throw string  { "las filas y las columnas deben ser positivas " };
        }
        unsigned short int index = (columna-1) + (fila-1)*columnas;
        return datos.at(index);
    }


    void setElement (int fila, int columna, float valor){ //definimos como float el valor ya que en matrices como la inversa nos pueden dar los elementos en francciones
            if (fila <= 0 || columna <= 0 ){
                throw string  { "las filas y las columnas deben ser positivas " };
            }
            unsigned short int index = (columna-1) + (fila-1)*columnas;
            datos.at(index)=valor;
    }

    //Esta función suma una matriz con otra
    bool add(Matrix &other){

        if (filas != other.filas || columnas != other.columnas) return false;
           for(unsigned short int i{1};i<=filas;i++){
               for(unsigned short int j{1};j<=columnas;j++){
                   other.setElement(i, j, other.getElement(i, j) + getElement(i, j)); //rellenamos la matriz auxiliar con los nuevos elementos sumados
               }
           }
           return true;
       }
   //En esta funcion multiplicamos una matriz por otra
   bool Multiply( Matrix other, Matrix &aux ){
    if(columnas != other.filas) return false; //si las filas de la matriz no coincide con las columnas de la otra, es imposible de hacer, por lo que devolvemos un false
    for (unsigned short int i{1}; i <= filas; i ++){
            for (unsigned short int j{1}; j <= other.columnas; j ++){
            float valor{0};
            for(unsigned short k{1};k<=columnas;k++){
                    valor=valor+getElement(i,k)*other.getElement(k,j);  //multiplicamos los elementos y los sumamos
                }
                aux.setElement(i,j,valor);  //rellenamos la matriz auxiliar con los nuevos valores y la pasamos por referencia
            }
    }
    return true;
   }
   void transpose(Matrix &aux){

       for(unsigned short int i{1};i<=filas;i++){
           for(unsigned short int j{1};j<=columnas;j++){
               aux.setElement(i,j,getElement(j,i)); //trasponemos la matriz usando una matriz auxiliar
           }
       }
   }
   //esta funcion es refersiva, ya que se va llamando a si misma para asi poder hacer un determinante de matrices de dimension mayores que 3
   bool determinante(float &Detvalor){
           float valor{0},e, signo;
           Matrix auxDet(filas-1,columnas-1);
           if (filas !=columnas) return false; //si la matriz no es cuadrada es imposible hacer un determinante por lo que devolvemos un false

           Detvalor=0;
           for (int i=1; i<=columnas; i++){
               if ((1+i) % 2 == 0) signo=1; //si la posición del elemento es par, sabemos que en la formula del determinante tendrá un valor positivo, si es impar, será negativo
               else signo =-1;
               e = signo * getElement(1,i); //establecemos el valor
               detMatrix(1,i,auxDet); //con esta función establecemos una nueva matriz quitandole la primera fila y la columna i de la otra matriz
               if (auxDet.filas==1) valor = auxDet.getElement(1,1);
               else auxDet.determinante(valor);
               Detvalor=Detvalor+(e*valor); //vamos acumulando el valor 
           }
           return true;
       }
   //Esta funcion imprime cualquier matriz recorriendo todos sus elementos e imprimiendolos por pantalla
    void print(){
        for(unsigned short int i{1};i<=filas;i++){
            for(unsigned short int j{1};j<=columnas;j++){
                cout << getElement(i, j) << "\t";
            }
            cout << endl;
        }
    }
    //Esta funcion hace la inversa de una matriz, obteniendo la matriz y el valor de su determinante
    bool Inversa(Matrix &aux,int valor){
        Matrix auxAd(filas,columnas); //hacemos la adjunta
        Matrix auxTras(filas,columnas); //trasponemos la matriz adjunta
        if(valor==0 || filas!=columnas) return false; //si el determinante es 0 o la matriz no es cuadrada no existe solucion por lo que se devuelve un false
       aux.adjunta(auxAd); //hacemos la adjunta
       auxAd.transpose(auxTras); //hacemos la traspuesta de la adjunta
        for(unsigned short int i{1};i<=filas;i++){
            for(unsigned short int j{1};j<=columnas;j++){
                aux.setElement(i,j,auxTras.getElement(i,j)/valor); //recorremos todos los elementos dividiendo la traspuesta de la adjunta por el determinante de la matriz
            }
        }
        return true;
    }


private:
    vector<float> datos;
    int columnas, filas;
    
    //Esta función la usaremos para el determinante ya que la necesitaremos para poder quitar 1 fila y una columna cada vez que se llame a si misma
    void detMatrix(int outf, int outc, Matrix &aux){ //para la funcion necesitaremos saber la matriz, su fila y su columna para poder quitarselas
            int contf{1}, contc{1};
            for (int i=1; i<=filas; i++){
                if (i != outf) {                    //si es diferente a la fila la saltamos 
                    contc=1;
                    for (int j{1}; j<=columnas;j++){
                        if (j != outc){             //si es diferente a la columna la saltamos
                            aux.setElement(contf,contc,getElement(i,j));
                            contc++;                                        //contador de columnas
                        }
                    }
                    contf++;                                                //contador de filas
                }
            }
        }
    //Esta función hace la adjunta de una matriz
    bool adjunta(Matrix &aux){
         float signo, valor;
         Matrix auxad(filas-1,columnas-1);
         if (filas!= columnas || filas<2) { //si la matriz no es cuadrada o es de dimension menor que dos es imposible hacer la adjunta
             cout << "No existe la adjunta." << endl; return false;
         }
         for (int i{1}; i<=filas; i++){
             for (int j{1}; j<=columnas; j++){
                 detMatrix(i,j,auxad);
                 auxad.determinante(valor);
                if ((i+j)%2 == 0) signo= 1;
                  else signo= -1;
                 aux.setElement(i,j,signo * valor);
             }
         }
         return true;
     }

};

int main()
{
    try{
        int fila, columna, fila2, columna2;
        float valor,valor2;
        //Pedimos al usuario que introduzca las dimensiones de las matrices
        cout << "Introduzca el numero de filas de la primera matriz: ";
        cin >> fila;
        cout << "Introduzca el numero de columnas de la primera matriz: ";
        cin >> columna;
        Matrix one(fila,columna);
        cout <<endl;
        cout << "Introduzca el numero de filas de la segunda matriz: ";
        cin >> fila2;
        cout << "Introduzca el numero de columnas de la segunda matriz: ";
        cin >> columna2;
        cout <<endl;
        Matrix two(fila2, columna2);
        //Pedimos al usuario que introduzca los elementos de cada matriz
        for(unsigned short int i{1};i<=fila;i++){
            for(unsigned short int j{1};j<=columna;j++){
                cout << "Introduce elemento " << i << ", " << j << " de la primera matriz: ";
                float valor;
                cin >> valor;
                one.setElement(i, j,valor);
          }
        }
        cout << endl;
        for(unsigned short int i{1};i<=fila2;i++){
            for(unsigned short int j{1};j<=columna2;j++){
                cout << "Introduce elemento " << i << ", " << j << "de la segunda matriz: ";
                float valor;
                cin >> valor;
                two.setElement(i, j,valor);
            }
        }
        //Imprimimos por pantalla ambas matrices
        cout << endl;
        cout << "Primera matriz"<<endl;
        one.print();
        cout << endl;
        cout << "Segunda matriz"<<endl;
        two.print();
        //Definimos matrices auxiliares para poderlas modificar e imprimir por pantalla
        Matrix aux(fila, columna);
        cout << endl;
        aux= two;
        cout << "Suma de ambas matrices"<<endl;
        if(one.add(aux)) aux.print(); else cout << "No coinciden las filas y las columnas de ambas matrices"<<endl;
        Matrix aux2(fila, columna2); //para poder obtener unsa solución de la multiplicación de la primera por la segunda, la matriz resultante ha de tener las filas de la primera y las columnas de la segunda
        cout <<endl;
        cout << "La multiplicacion de la primera matriz por la segunda es: " << endl;
        if(one.Multiply(two,aux2)) aux2.print(); else cout << "No coinciden las filas y las columnas de ambas matrices";
        Matrix aux3(fila2,columna); //para poder obtener unsa solución de la multiplicación de la segunda por la primera, la matriz resultante ha de tener las filas de la segunda y las columnas de la primera
        cout <<endl;
        cout << "La multiplicacion de la segunda matriz por la primera es: " << endl;
        if(two.Multiply(one,aux3)) aux3.print(); else cout << "No coinciden las filas y las columnas de ambas matrices";
        Matrix aux4(fila,columna);
        aux4=one;
        cout << endl;
        cout << "La traspuesta de la primera matriz es: "<<endl;
        one.transpose(aux4);
       aux4.print();
       Matrix aux5(fila2,columna2);
       aux5=two;
       cout << "La traspuesta de la segunda matriz es: "<<endl;
       two.transpose(aux5);
       aux5.print();
       cout<<endl;
       cout << "El determinante de la primera matriz es: "<<endl;
       if(one.determinante(valor)) cout << valor <<endl;else cout << "No existe determinante al no ser cuadrada"<<endl;
       cout<<endl;
       cout << "El determinante de la segunda matriz es: "<<endl;
       if(two.determinante(valor2)) cout <<valor2 <<endl;else cout << "No existe determinante al no ser cuadrada"<<endl;
       cout<<endl;
       cout<<"La inversa de la primera matriz es: "<<endl;
       Matrix aux6(fila,columna);
       aux6=one;
       if(one.Inversa(aux6,valor))aux6.print();else cout << "No se puede invertir. "<<endl;
       cout <<endl;
       cout<<"La unversa de la segunda matriz es: "<<endl;
       Matrix aux7(fila2,columna2);
       aux7=two;
       if(two.Inversa(aux7,valor2))aux7.print();else cout << "No se puede invertir. "<<endl;

   }catch(string e){
        cout << e << endl;
        return -1;
    }
    return 0;
}
