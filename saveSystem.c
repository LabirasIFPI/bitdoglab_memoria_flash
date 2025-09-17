#include "saveSystem.h"
#include <string.h>

void saveProgress(uint16_t *buffer) 
{  
    uint8_t buffer_completo[FLASH_PAGE_SIZE];
    memcpy(buffer_completo, buffer, FLASH_PAGE_SIZE);
    uint32_t interruptions = save_and_disable_interrupts();    
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);    
    flash_range_program(FLASH_TARGET_OFFSET, buffer_completo, FLASH_PAGE_SIZE);    
    restore_interrupts(interruptions);
}

void loadProgress(uint16_t *buffer, size_t tamanho) 
{    
	uint8_t *address = (uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);    
	memcpy(buffer, address, tamanho);
}

void clearSaveData() 
{
	uint32_t interruptions = save_and_disable_interrupts();    
	flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);    
	flash_range_program(FLASH_TARGET_OFFSET, 0x00, FLASH_PAGE_SIZE);    
	restore_interrupts(interruptions);
}