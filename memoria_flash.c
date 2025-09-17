#include <stdio.h>
#include "saveSystem.h"

void encapsular_dados(uint8_t level, uint16_t highScore, uint8_t *buffer) 
{    
	buffer[0] = level;    
	buffer[1] = (highScore >> 8) & 0xFF;    
	buffer[2] = highScore & 0xFF;
}

uint16_t recuperar_level(uint8_t *buffer) {
    // O level está armazenado no primeiro byte do buffer
    return buffer[0];
}

uint16_t recuperar_highScore(uint8_t *buffer) {
    // Combina os bytes do buffer para formar o highScore
    return (buffer[1] << 8) | buffer[2];
}

int main()
{
    // inicial comunicação serial e aguarda 1.5s
    stdio_init_all();
    sleep_ms(1500);

    uint8_t level = 5;
    uint16_t highScore = 12345;
    uint8_t buffer[FLASH_PAGE_SIZE];

    // Encapsular dados no buffer
    encapsular_dados(level, highScore, buffer);

    // Salvar o buffer na memória flash
    saveProgress((uint16_t *)buffer);

    // Criar buffer temporário para carregar os dados
    uint8_t buffer_carregado[FLASH_PAGE_SIZE];

    // Carregar o buffer da memória flash
    loadProgress((uint16_t *)buffer_carregado, FLASH_PAGE_SIZE);

    // Recuperar os dados do buffer carregado
    uint8_t level_carregado = recuperar_level(buffer_carregado);
    uint16_t highScore_carregado = recuperar_highScore(buffer_carregado);

    // Exibir os dados carregados
    printf("Level carregado: %u\n", level_carregado);
    printf("HighScore carregado: %u\n", highScore_carregado);

    // agurda 5s e em seguida limpa a memória
    sleep_ms(5000);
    clearSaveData();

    return 0;
}