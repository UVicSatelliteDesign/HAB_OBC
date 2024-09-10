#include "data_storage.h"

void erase_bank() {
  HAL_FLASH_Unlock();

  // declare and define an instance of the FLASH_EraseInitTypeDef structure
  FLASH_EraseInitTypeDef flash_erase_struct = {0};
  flash_erase_struct.TypeErase = FLASH_TYPEERASE_MASSERASE;
  flash_erase_struct.Banks = FLASH_BANK_2; /*!<  @ref FLASHEx_Banks */

  flash_erase_struct.Sector =
      FLASH_SECTOR_0; /*!< Choose sector arbitrarily as we are not erasing by
                         sector @ref FLASH_Sectors */

  flash_erase_struct.NbSectors =
      1; /*!< Assign 1 abitrarily as we are not erasing by sector */

  flash_erase_struct.VoltageRange =
      FLASH_PSIZE_DOUBLE_WORD; /*!< clear 64 bits at a time*/

  uint32_t error_status = 0;

  // erase bank 2
  HAL_FLASHEx_Erase(&flash_erase_struct, &error_status);

  HAL_FLASH_Lock();

  // check error status
  if (error_status != 0xFFFFFFFF) {
    printf("Flash erase error\n");
  }
}

void write_data(uint32_t *data) {
  assert(data != NULL);

  // Do not write more data if the end of the flash memory has been reached
  if (cur_flash_addr >= FLASH_END) {
    return;
  }

  HAL_FLASH_Unlock();

  HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, cur_flash_addr,
                    (uint32_t)data);

  HAL_FLASH_Lock();

  cur_flash_addr += 32; // 8 words * 4 bytes per word = 32
}

void read_data(UART_HandleTypeDef huart3) {
  int i = FLASH_USER_START_ADDR;
  for (; i < cur_flash_addr; i += 32) {
    HAL_UART_Transmit(&huart3, &(*(uint32_t *)(i)), 32, 100);
  }
}

void test_log_data() {
  erase_bank();

  Data data = {0};
  data.time = 1;
  data.volts = 2;
  data.altitude = 5;
  data.latitude = 4;
  data.longitude = 3;

  int i = cur_flash_addr;

  write_data((uint32_t *)(&data));

  int count = 0;
  for (; i < cur_flash_addr; i += 4) {
    uint32_t d = *(uint32_t *)(i);
    switch (count) {
    case 0:
      assert_param(d == (uint32_t)data.time);
      break;
    case 1:
      assert_param(d == (uint32_t)data.volts);
      break;
    case 2:
      assert_param(d == data.longitude);
      break;
    case 3:
      assert_param(d == data.latitude);
      break;
    case 4:
      assert_param(d == data.altitude);
      break;
    case 5:
      assert_param(d == (uint32_t)data.temperature);
      break;
    default:
      assert_param(d == 0);
    }
    count++;
  }
}

void test_read_data(UART_HandleTypeDef huart3) {
  erase_bank();

  Data data = {0};
  data.time = 1;
  data.volts = 2;
  data.altitude = 5;
  data.latitude = 4;
  data.longitude = 3;

  write_data((uint32_t *)(&data));
  write_data((uint32_t *)(&data));

  read_data(huart3);
}

void transfer_serial_for_testing(char packet_buffer[256]) {
  strcpy(packet_buffer,"");
  char buffer[32] = "";          
  int i = FLASH_USER_START_ADDR;

  for (; i < cur_flash_addr; i += 32) {
    snprintf(buffer, 32, "%u", *(uint32_t *)(i));  
    strcat(packet_buffer, buffer);                 
    strcat(packet_buffer, "_");                    
  }
  packet_buffer[255]="\0";
}
