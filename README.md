<h2 align="center">
  <img src="https://emojis.slackmojis.com/emojis/images/1643514044/32/python.png?1643514044" width="30"/> 
    Transpiler python to c++</a> 
  <img src="https://emojis.slackmojis.com/emojis/images/1643514403/3795/cpp.png?1643514403" width="30"/> 
</h1>

---

<h3 style="arial;">
    This program consists of translating the programming language python to c++ </a>
</h3>

### Python Code

---

```python
def impresion(mensaje,informacion):#8udugduwu                                      
    print(mensaje,informacion)
def llenarLista(longitud):
    Contenedor=[]
    for i in range(0, longitud):
        elemento = int(input("Introduce un numero: "))
        Contenedor.append(elemento) 
    return Contenedor
def sumatoria(Lista):
    suma=0
    for x in range(0,len(Lista)):
        suma=suma+Lista[x]
    return suma
def calcularProducto(Lista):
    vActual=Lista[0]
    for x in range(1,len(Lista)):
        vActual=Lista[x]*vActual
    return vActual

print("Bienvenido a compiladores")
Datos=input("Datos a introducir: ")
impresion("El usuario introducira",Datos)
Datos=llenarLista(int(Datos))
Suma=sumatoria(Datos)
impresion("La sumatoria de los numeros es: ",Suma)
Producto=calcularProducto(Datos)
impresion("El producto de los numeros es: ",Producto)

```

## Translated code in c++

```cpp
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
```
