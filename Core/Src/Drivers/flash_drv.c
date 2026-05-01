/**
  ******************************************************************************
  * @file    flash_drv.c
  * @brief   Flash storage driver.
  *
  * Writes are word-aligned (4 bytes). savedData_t must be a multiple of 4
  * bytes — add padding if needed.
  ******************************************************************************
  */

#include "flash_drv.h"
#include "main.h"

/* ── flash_drv_load ───────────────────────────────────────────────────────── */

uint8_t flash_drv_load(savedData_t *out)
{
    const savedData_t *stored = (const savedData_t *)FLASH_USER_START_ADDR;

    if (stored->magic != FLASH_MAGIC) { return 0; }

    *out = *stored;
    return 1;
}

/* ── flash_drv_save ───────────────────────────────────────────────────────── */

void flash_drv_save(const savedData_t *data)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase = {
        .TypeErase   = FLASH_TYPEERASE_PAGES,
        .PageAddress = FLASH_USER_START_ADDR,
        .NbPages     = 1,
    };
    uint32_t page_error = 0;
    HAL_FLASHEx_Erase(&erase, &page_error);

    const uint32_t *src   = (const uint32_t *)data;
    uint32_t        words = sizeof(savedData_t) / sizeof(uint32_t);

    for (uint32_t i = 0; i < words; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                          FLASH_USER_START_ADDR + i * 4u,
                          src[i]);
    }

    HAL_FLASH_Lock();
}
