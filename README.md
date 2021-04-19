# **Práctica 7: Modos de cifrado en bloque**

Este informe tiene la intención de mostrar y servir de ayuda para el usuario a la hora de usar el programa.
La explicación de la implementación del programa se encuentra en el siguiente vídeo de Youtube: [enlace de la explicación](https://youtu.be/ZXcerVVwyGw)

## **Uso del programa**

### **Compilación**

Para llevar a cabo la compilación del programa, se hará uso del comando **make**. Además, se encuentra disponible el comando **make clean**, que borrará del directorio el ejecutable creado.

En caso de carecer del programa Make, se procederá a compilar haciendo uso de: 

```
g++ -g cbc.cpp -o cbc 
```

### **Ejecución**

Una vez realizada la compilación del proyecto, se procederá a realizar la ejecución del mismo. El uso de este ejecutable es sencillo, ya que solamente hará falta realizar lo siguiente: 
```
./cbc
```

A continuación, se muestra un ejemplo de datos a introducir por la terminal: 


    Clave: 000102030405060708090a0b0c0d0e0f
    IV: 00000000000000000000000000000000
    Bloque 1 de Texto Original: 00112233445566778899aabbccddeeff
    Bloque 2 de Texto Original: 00000000000000000000000000000000