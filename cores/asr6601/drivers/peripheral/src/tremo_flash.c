#include <string.h>
#include "tremo_flash.h"
#include "sx126x-board.h"

/**
 * @brief Erase all the flash main area
 * @param None
 * @retval ERRNO_OK Erase successfully 
 * @retval ERRNO_FLASH_SEC_ERROR Erase failed due to the flash security policy 
 */
int32_t flash_erase_all(void)
{
    //clear sr
    if(SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK)
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
    
    FLASH_CR_UNLOCK();
    EFC->CR = (EFC->CR & EFC_CR_ECC_DISABLE_MASK) | EFC_CR_MASS_ERASE_EN_MASK | EFC_CR_PREFETCH_EN_MASK;
    FLASH_CR_LOCK();

    TREMO_REG_WR(FLASH_BASE, 0xFFFFFFFF);

    if (SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK) {
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
        return ERRNO_FLASH_SEC_ERROR;
    }

    while (!(EFC->SR & EFC_SR_OPERATION_DONE))
        ;
    EFC->SR = EFC_SR_OPERATION_DONE;

    return ERRNO_OK;
}

/**
 * @brief Erase one page 
 * @param addr The flash address
 * @retval ERRNO_OK Erase successfully 
 * @retval ERRNO_FLASH_SEC_ERROR Erase failed due to the flash security policy 
 */
int32_t flash_erase_page(uint32_t addr)
{
    //clear sr
    if(SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK)
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
    
    FLASH_CR_UNLOCK();
    EFC->CR = (EFC->CR & EFC_CR_ECC_DISABLE_MASK) | EFC_CR_PAGE_ERASE_EN_MASK | EFC_CR_PREFETCH_EN_MASK;
    FLASH_CR_LOCK();

    TREMO_REG_WR(addr, 0xFFFFFFFF);

    if (SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK) {
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
        return ERRNO_FLASH_SEC_ERROR;
    }

    while (!(EFC->SR & EFC_SR_OPERATION_DONE))
        ;
    EFC->SR = EFC_SR_OPERATION_DONE;

    return ERRNO_OK;
}

/**
 * @brief Program the data into flash 
 * @note  The address must be aligned by 8 bytes. If the size is not an integral multiple of 8 bytes, it will be padded with 0xFF 
 * @param addr The flash address
 * @param data The data to be programmed
 * @param size The size of the data
 * @retval ERRNO_OK Program successfully 
 * @retval ERRNO_FLASH_SEC_ERROR Program failed due to the flash security policy 
 */
int32_t flash_program_bytes(uint32_t addr, uint8_t* data, uint32_t size)
{
    uint8_t tmp[8];
    uint8_t* p            = tmp;
    uint32_t aligned_size = 0;

    //clear sr
    if(SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK)
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
    
    FLASH_CR_UNLOCK();
    EFC->CR = (EFC->CR & EFC_CR_ECC_DISABLE_MASK) | EFC_CR_PROG_EN_MASK | EFC_CR_PREFETCH_EN_MASK;
    FLASH_CR_LOCK();

    aligned_size = (size)&0xFFFFFFF8;
    memset((char*)tmp, 0xFF, sizeof(tmp));
    for (int i = aligned_size; i < size; i++) {
        tmp[i - aligned_size] = data[i];
    }

    for (int i = 0; i < size; i += 8) {
        if (i < aligned_size) {
            EFC->PROGRAM_DATA0 = *(uint32_t*)(data + i);
            EFC->PROGRAM_DATA1 = *(uint32_t*)(data + i + 4);
        } else {
            EFC->PROGRAM_DATA0 = *(uint32_t*)(p);
            EFC->PROGRAM_DATA1 = *(uint32_t*)(p + 4);
        }
        // program addr
        TREMO_REG_WR(addr + i, 0xFFFFFFFF);

        if (SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK) {
            SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
            return ERRNO_FLASH_SEC_ERROR;
        }

        while (!(EFC->SR & EFC_SR_OPERATION_DONE))
            ;
        EFC->SR = EFC_SR_OPERATION_DONE;
    }

    return ERRNO_OK;
}

/**
 * @brief Program a word line data into flash 
 * @note  The address must be aligned by 512 bytes.
 * @note  This function must be executed in RAM
 * @param addr The flash address
 * @param data The data to be programmed
 * @retval ERRNO_OK Program successfully 
 * @retval ERRNO_FLASH_SEC_ERROR Program failed due to the flash security policy 
 */
RAM_FUNC_ATTR int32_t flash_program_line(uint32_t addr, uint8_t* data)
{
    bool tempIrq = BoardDisableIrq( );
    //clear sr
    if(SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK)
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
    
    // wl program
    FLASH_CR_UNLOCK();
    EFC->CR = (EFC->CR & EFC_CR_ECC_DISABLE_MASK) | EFC_CR_WRITE_RELEASE_EN_MASK | EFC_CR_PROG_MODE_WLINE
        | EFC_CR_PROG_EN_MASK | EFC_CR_PREFETCH_EN_MASK;
    FLASH_CR_LOCK();

    while (!(EFC->SR & EFC_SR_PROGRAM_DATA_WAIT))
        ;
    EFC->PROGRAM_DATA0 = *(uint32_t*)(data);
    EFC->PROGRAM_DATA1 = *(uint32_t*)(data + 4);
    TREMO_REG_WR(addr, 0xFFFFFFFF);

    if (SEC->SR & SEC_SR_FLASH_ACCESS_ERROR_MASK) {
        SEC->SR = SEC_SR_FLASH_ACCESS_ERROR_MASK;
        BoardEnableIrq( tempIrq );
		
        return ERRNO_FLASH_SEC_ERROR;
    }

    for (int j = 8; j < 512; j += 8) {
        while (!(EFC->SR & EFC_SR_PROGRAM_DATA_WAIT))
            ;
        EFC->PROGRAM_DATA0 = *(uint32_t*)(data + j);
        EFC->PROGRAM_DATA1 = *(uint32_t*)(data + j + 4);
    }
    while (!(EFC->SR & EFC_SR_OPERATION_DONE))
        ;
    EFC->SR = EFC_SR_OPERATION_DONE;

    BoardEnableIrq( tempIrq );
    return ERRNO_OK;
}

/**
 * @brief Program the data into flash OTP area
 * @note  The address must be aligned by 8 bytes. If the size is not an integral multiple of 8 bytes, it will be padded with 0xFF 
 * @param addr The OTP area address
 * @param data The data to be programmed
 * @param size The size of the data
 * @retval ERRNO_OK Program successfully 
 * @retval ERRNO_FLASH_SEC_ERROR Program failed due to the flash security policy 
 * @retval ERRNO_FLASH_INVALID_ADDR The address is not a valid OTP address 
 * @retval ERRNO_FLASH_INVALID_SIZE The size is wrong
 * @retval ERRNO_FLASH_OTP_REFLASH This area have been programmed
 */
int32_t flash_otp_program_data(uint32_t addr, uint8_t* data, uint32_t size)
{
    int i                 = 0;
    uint32_t aligned_size = 0;
    if (addr < FLASH_OTP_ADDR_START || (addr + size) > FLASH_OTP_ADDR_END)
        return ERRNO_FLASH_INVALID_ADDR;

    if (size == 0 || size > FLASH_OTP_SIZE)
        return ERRNO_FLASH_INVALID_SIZE;

    aligned_size = (size + 7) & 0xFFFFFFF8;
    for (i = 0; i < aligned_size; i++) {
        if (*(uint8_t*)(addr + i) != 0xFF)
            return ERRNO_FLASH_OTP_REFLASH;
    }

    return flash_program_bytes(addr, data, size);
}
