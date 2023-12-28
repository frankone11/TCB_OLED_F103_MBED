#include "oled.h"
#include "arial.h"
#include <cstring>

Oled::Oled():
i2c(NULL)
{
    memset(buffer, 0, BUFFER_TAM);
}

Oled::Oled(I2C *i2c):
i2c(i2c)
{
    memset(buffer, 0, BUFFER_TAM);
}

void Oled::ColocaI2C(I2C *i2c)
{
    this->i2c = i2c;
}

I2C * Oled::DameI2C(void)
{
    return i2c;
}

bool Oled::DameBuffer(uint8_t *buf, uint16_t tam)
{
    if(!buf)
        return false;

    if(tam > BUFFER_TAM)
        tam = BUFFER_TAM;
    
    memcpy(buf, buffer, tam);
    return true;

}

bool Oled::ColocaBuffer(const uint8_t *buf, uint16_t tam)
{
    if(!buf)
        return false;
    if(tam < BUFFER_TAM)
        return false;

    memcpy(buffer, buf, BUFFER_TAM);
    return true;
}

uint16_t Oled::DameTamBuffer(void)
{
    return BUFFER_TAM;
}

void Oled::Comando(uint8_t comando)
{
    uint8_t buf[2] = { 0, 0 };
    buf[1] = comando;
    i2c->write(ADDR, (const char *)buf, 2);
}

void Oled::Comandos(const uint8_t *buff, uint8_t tam)
{
    if(tam > MAX_DAT)
		return;
	uint8_t buf[MAX_DAT + 1] = {0};
	for(uint8_t i = 0; i < tam; i++)
		buf[i+1] = buffer[i];
    i2c->write(ADDR, (const char *)buf, tam+1);
}

void Oled::Dato(uint8_t dato)
{
    uint8_t buf[2] = { 0x40U, 0};
	buf[1] = dato;
    i2c->write(ADDR, (const char *)buf, 2);
}

void Oled::Inicializa(void)
{
    const uint8_t init1[] =
	{
			0xAEU,
			0xD5U,
			0x80U,
			0xA8U
	};

	Comandos( init1, sizeof(init1));
	Comando( 0x3FU);

	const uint8_t init2[] =
	{
			0xD3U,
			0x00U,
			0x40U,
			0x8DU
	};

	Comandos( init2, sizeof(init2));

	Comando( 0x14U);

	const uint8_t init3[] =
	{
			0x20U,
			0x00U,
			0xA1U, //A0 | 1
			0xC8U
	};

	Comandos( init3, sizeof(init3));

	Comando( 0xDAU);
	Comando( 0x12U);
	Comando( 0x81U);
	Comando( 0xCFU);
	Comando( 0xD9U);
	Comando( 0XF1U);

	const uint8_t init5[] =
	{
			0xDBU,
			0x40U,
			0xA4U,
			0xA6U,
			0x2EU,
			0xAFU,
	};

	Comandos( init5, sizeof(init5));

	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Comandos( initxy, sizeof(initxy));

	LimpiaDisplay();
}

void Oled::Dibuja(void)
{
    const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Comandos(initxy, sizeof(initxy));

	for(uint8_t j = 0; j < 128; j++)
	{
		for(uint8_t i = 0; i < 8; i++)
			Dato(buffer[(j*8)+i]);
	}
}

void Oled::LimpiaDisplay()
{
    memset(buffer, 0, 1024);
    Dibuja();
}

void Oled::ColocaCaracter(uint8_t *dir, const char caracter)
{
    uint8_t noval[8] = { 0x18U, 0x3CU, 0x76U, 0x9BU, 0x66U, 0x3CU, 0x18U, 0x00U };
	const tImage *img = NULL;

	if(caracter < 0x20 || caracter == 127)
		memcpy(dir, noval, 8);
	else
	{
		img = Arial.chars[caracter - 0x20U].image;
		memcpy(dir, img->data, img->dataSize);
	}
}

void Oled::EnviaCadenaXY(const char *txt, uint8_t fila, uint8_t columna)
{
    uint16_t offset;
	offset = (fila * 128) + (columna * 8);
	if(!txt)
		return;
	while(*txt)
	{
		ColocaCaracter((uint8_t *)(buffer + offset), *txt++);
		offset += 8;
	}
}
