#include <bits/stdc++.h>

void impresion(std::string mensaje, std::string informacion){
    std::cout << mensaje << informacion << std::endl;
}

template<class TIPO_DATO>
std::vector<int> llenarLista(TIPO_DATO longitud){
    std::vector<int> Contenedor {};
    for(int i = 0; i < longitud; i ++){
        std::cout << "Introduce un numero: ";
int elemento;
        std::cin >> elemento;
        Contenedor.push_back(elemento);
    }
    return Contenedor;
}

template<class TIPO_DATO>
int sumatoria(TIPO_DATO Lista){
    int suma  = 0;
    for(int x = 0; x < Lista.size(); x ++){
        suma  = suma+Lista[x];
    }
    return suma;
}

template<class TIPO_DATO>
int calcularProducto(TIPO_DATO Lista){
    int vActual  = Lista[0];
    for(int x = 1; x < Lista.size(); x ++){
        vActual  = Lista[x]*vActual;
    }
    return vActual;
}

int main(){
std::cout << "Bienvenido a compiladores" << std::endl;
std::cout << "Datos a introducir: ";
int Datos;
std::cin >> Datos;
impresion("El usuario introducira",std::to_string(Datos));
std::vector<int> Datos2=llenarLista(int(Datos));
int Suma =sumatoria(Datos2);
impresion("La sumatoria de los numeros es: ",std::to_string(Suma));
int Producto =calcularProducto(Datos2);
impresion("El producto de los numeros es: ",std::to_string(Producto));
}

