#include <iostream>
#include <string>
#include <vector> 

// Matriz de la S-Caja
uint8_t s_box[16][16] {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

// Matriz para la multiplicación en MixColumns
uint8_t mix_column_box[4][4] {

    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}  
};

// Vector de elementos pertenecientes al Rcon utilizado en la expansion de la clave
uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36
};


// Función que rota las posiciones de la columna
void rotWord(uint8_t *columna) {
    uint8_t aux;
    aux = columna[0];

    for(int i = 0; i < 3; i++) {
        columna[i] = columna[i+1];
    }
    columna[3] = aux;
} 



// Realiza la operacion SubBytes en una columna 
void subBytesRow(uint8_t *columna) {
    for (int i = 0; i < 4; i++) {
        int dato = columna[i];
        uint8_t rows = dato & 0x0f;
        uint8_t column = dato & 0xf0;
        column >>= 4;
        columna[i] = s_box[column][rows];
    }
}

void expansionClave(uint8_t (*clave)[4], uint8_t *clave_expandida) {

    int k = 0;
    uint8_t columna_copia[4];

    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            clave_expandida[k] = clave[j][i];
            k++;
        }
    }

    for(int i = 4; i < 44; i++) {
        for (int j = 0; j < 4; j++) {
            columna_copia[j] = clave_expandida[(i-1) * 4 + j];
        }
        if(i % 4 == 0) {
            rotWord(columna_copia);
            subBytesRow(columna_copia);
            columna_copia[0] = columna_copia[0] ^ Rcon[i / 4 - 1];
        }

        for (int k = 0; k < 4; k++) {
            clave_expandida[i * 4 + k] = clave_expandida[(i - 4) * 4 + k] ^ columna_copia[k];
        }
    }


    // Imprime por pantalla las diferentes Claves creadas.
//     //std::cout << "Key 0 = ";
//     for (int i = 0; i < 176; i++) {
//         //printf("%02x", clave_expandida[i]);
//         if((i+1) % 16 == 0 && i < 175) {
//             //std::cout << std::endl;
//             //std::cout << "Key " << i/16 + 1 << " = ";
//         }
//     }
//     //std::cout << std::endl;   
}

// Método que sustituye los elementos de la matriz por los valores de la S-Caja seleccionados
void subByte(uint8_t (*texto)[4]) {
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint8_t dato = texto[i][j];
            uint8_t rows = dato & 0x0f;
            uint8_t column = dato & 0xf0;
            column = column >> 4;
            texto[i][j] = s_box[column][rows];
        }
    }
}

// Método que cambia de posición cada elemento de las filas
void shiftRows(uint8_t (*texto)[4]) {

    uint8_t aux = texto[1][0];
    for(int i = 0; i < 4; i++) {
        texto[1][i] = texto[1][i+1];
    }
    texto[1][3] = aux;

    for (int i = 0; i < 2; i++) {
        aux = texto[2][i];
        texto[2][i] = texto[2][i+2];    
        texto[2][i+2] = aux;
    }

    aux = texto[3][3];
    for (int i = 3; i > 0; i--) {
        texto[3][i] = texto[3][i-1];
    }
    texto[3][0] = aux;
}

// Método MixColumns
void mixColumns(uint8_t (*r)[4]) {

    uint8_t a[4], b[4], h;
    uint8_t aux;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            a[j] = r[j][i];
            h = r[j][i] & 0x80;
            b[j] = r[j][i] << 1;

            if (h == 0x80)
                b[j] ^= 0x1b;   // Rijndael's Galois Field 
            
        }
        r[0][i] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
        r[1][i] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
        r[2][i] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
        r[3][i] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];

    }
}

// Método addRoundKey
void addRoundKey(uint8_t (*texto)[4], uint8_t *clave_expandida, int pos) {

    uint8_t copia_texto[16];
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            copia_texto[i*4 + j] = texto[j][i];
        }
    }

    std::cout << "\nR" << pos << "(Subclave = ";
    for(int i = 0; i < 16; i++) {
        printf("%02x", clave_expandida[pos * 16 + i]);
    }
    std::cout << ") = ";
    for(int i = 0; i < 16; i++) {
        copia_texto[i] ^= clave_expandida[pos * 16 + i];
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            texto[j][i] = copia_texto[i*4 + j];
        }
    }

    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            
            printf("%02x", texto[j][i]);
        }
    }

}

// Función de cifrado
void cifrado(uint8_t (*texto)[4], uint8_t *clave_expandida) {
    
    addRoundKey(texto, clave_expandida, 0);
    
    for (int i = 1; i < 10; i++) {
        subByte(texto);
        shiftRows(texto);
        mixColumns(texto);
        addRoundKey(texto, clave_expandida, i);
    }

    subByte(texto);
    shiftRows(texto);
    addRoundKey(texto, clave_expandida, 10);

}

// Convierte un string en una matriz de uint8_t
void stringToInt(std::string str, uint8_t (*matriz)[4]) {

    int final= 2;
    int inicio = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            std::string aux = str.substr(inicio, final);
            inicio += 2;
            //final += 2;
            uint32_t entero = std::stoi(aux, 0, 16);
            matriz[j][i] = entero;

            //printf("%02x", matriz[i][j]);
        }

    }

}

// Convierte un string en una matriz de uint8_t
void stringToVector(std::string str, uint8_t *v) {

    int final= 2;
    int inicio = 0;
    for (int i = 0; i < 16; i++) {
        
        std::string aux = str.substr(inicio, final);
        inicio += 2;
        //final += 2;
        uint32_t entero = std::stoi(aux, 0, 16);
        v[i] = entero;
        //printf("%02x", v[i]);
         
    }
//std::cout << std::endl;
}

void vectorToMatriz(uint8_t *v, uint8_t (*matriz)[4]) {

    int final= 2;
    int contador = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            matriz[j][i] = v[contador];
            contador++;
            printf("%02x", matriz[i][j]);
        }

    }

}

void xorPlainText(uint8_t *v_texto, uint8_t *v_initialization, uint8_t *v_resultado) {

    for (int i = 0; i < 16; i++) {
        v_resultado[i] = v_texto[i] ^ v_initialization[i];
        printf("%02x", v_resultado[i]);
        std::cout << " ";
    }

    std::cout << std::endl;

}

int main(void) {


    std::string clave = "000102030405060708090a0b0c0d0e0f";
    std::string initialization_str = "00000000000000000000000000000000";
    std::string texto_1 = "00112233445566778899aabbccddeeff";
    std::string texto_2 = "00000000000000000000000000000000";

    uint8_t matriz_clave[4][4] = {
     { 0x00, 0x04, 0x08, 0x0c },
     { 0x01, 0x05, 0x09, 0x0d },
     { 0x02, 0x06, 0x0a, 0x0e },
     { 0x03, 0x07, 0x0b, 0x0f }
    };
    uint8_t matriz_xor[4][4];

    uint8_t clave_expandida[176];
    uint8_t v_xor[16];
    uint8_t v_initialization[16];
    uint8_t v_text1[16];
    uint8_t v_clave[16];

    stringToVector(texto_1, v_text1);
    stringToVector(initialization_str, v_initialization);
    stringToVector(clave, v_clave);
    //stringToInt(clave, matriz_clave);
    
    
    xorPlainText(v_text1, v_initialization, v_xor);
    vectorToMatriz(v_xor, matriz_xor);

    expansionClave(matriz_clave, clave_expandida);
    cifrado(matriz_xor, clave_expandida);











}