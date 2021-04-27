#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_
#include <stdint.h>
#include <stdbool.h>

/** \addtogroup PERIPHERAL
 *  @{
 */

/** \addtogroup SPI_FLASH
 *  @{
 */

/// Flash Page Size
#define FLASH_PAGE_SIZE (256) 
/// Flash Sector Size
#define FLASH_SECTOR_SIZE (0x1000) 

/** \brief Set the status indicating whether the Flash is in XIP mode */
void spi_flash_xip_status_set(bool xip);

/** \brief Set the status indicating whether the Flash is programming or erasing */
void spi_flash_writing_status_set(bool writing);

/** \brief Initialize variables of the SPI Flash driver */
void spi_flash_drv_var_init(bool xip,bool writing);

/** \brief Initialize SPI Flash driver */
void spi_flash_init(void);

/** \brief Enter SPI Flash XIP mode */
void spi_flash_xip_start(void);

/** \brief Exit SPI Flash XIP mode */
void spi_flash_xip_stop(void);

/** \brief Read Status Register 0 */
void spi_flash_read_status_register_0(uint8_t *status_reg_0);

/** \brief Read Status Register 1 */
void spi_flash_read_status_register_1(uint8_t *status_reg_1);

/** \brief Write Status Register */
void spi_flash_write_status_register(uint16_t status);

/** \brief Quad Page Program */
void spi_flash_quad_page_program(uint32_t offset,uint8_t *data,uint16_t length);

/** \brief Page Program */
void spi_flash_page_program(uint32_t offset,uint8_t *data,uint16_t length);

/** \brief Page Erase */
void spi_flash_page_erase(uint32_t offset);

/** \brief Sector Erase */
void spi_flash_sector_erase(uint32_t offset);

/** \brief Chip Erase */
void spi_flash_chip_erase(void);

/** \brief Quad IO Read */
void spi_flash_quad_io_read(uint32_t offset,uint8_t *data,uint16_t length);

/** \brief Fast Read */
void spi_flash_fast_read(uint32_t offset,uint8_t *data,uint16_t length);

/** \brief Deep Power Down */
void spi_flash_deep_power_down(void);

/** \brief Release From Deep Power Down */
void spi_flash_release_from_deep_power_down(void);

/** \brief Read ID */
void spi_flash_read_id(uint8_t jedec_id[3]);

/** \brief Read Unique ID */
void spi_flash_read_unique_id(uint8_t unique_serial_id[16]);

/** \brief Erase Security Area */
void spi_flash_erase_security_area(uint8_t idx);

/** \brief Program Security Area */
void spi_flash_program_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length);

/** \brief Read Security Area */
void spi_flash_read_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length);

/** \brief Software Reset */
void spi_flash_software_reset(void);

/** \brief Check and Set Quad Enable Status */
void spi_flash_qe_status_read_and_set(void);

/** \brief Suspend Program or Erase Operation */
void spi_flash_prog_erase_suspend(void);

/** \brief Resume Program or Erase Operation */
void spi_flash_prog_erase_resume(void);

/** \brief Check if Flash is writing */
bool spi_flash_writing_busy(void);

/** \brief Check if Flash is in XIP mode */
bool spi_flash_xip_status_get(void);

/** @}*/


/** @}*/

#endif

