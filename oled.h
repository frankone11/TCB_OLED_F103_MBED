#undef __ARM_FP
#include "mbed.h"

#define ADDR 0x78U
#define MAX_DAT 32U
#define BUFFER_TAM 1024U

class Oled
{
    private:
    I2C *i2c;
    char buffer[BUFFER_TAM];
    protected:

    public:

    Oled();
    Oled(I2C *i2c);
    ~Oled();


    void ColocaI2C(I2C *i2c);
    I2C * DameI2C(void);

    bool DameBuffer(uint8_t *buf, uint16_t tam);
    bool ColocaBuffer(const uint8_t *buf, uint16_t tam);
    uint16_t DameTamBuffer(void);

    void Comando(uint8_t comando);
    void Comandos(const uint8_t *buff, uint8_t tam);
    void Dato(uint8_t dato);
    void Inicializa(void);
    void Dibuja(void);
    void LimpiaDisplay();
    void ColocaCaracter(uint8_t *dir, const char caracter);
    void EnviaCadenaXY(const char *txt, uint8_t fila, uint8_t columna);

};