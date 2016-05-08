#include "hpet.h"
#include "stdio.h"
#include "string.h"
#include "apic.h"

void init_hpet()
{
  printf("\nInitializing HPET (High Precision Event Timer)!!!");
  uint_fast32_t *ptr = acpiGetRSDPtr();
  // check if address is correct  ( if acpi is available on this pc )
  if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
  {
    //Get the APIC and HPET tables!!!
    struct RSDT* rsdt = (struct RSDT*)ptr;
    uint_fast32_t entries = rsdt->h.length;
    entries = (entries - sizeof(struct ACPI_SDTHeader))/4;
    ptr = (uint_fast32_t*)&rsdt->PointerToOtherSDT;

    for(uint32_t i=0; i<entries; i++)
    {
      if (acpiCheckHeader(*ptr, "HPET") == 0)
      {
        //TODO: INITIALIZE HPET
        hpet_sdt = (HPET_descriptor_table_t*)*ptr;
        printf("\nHPET TABLE FOUND :D CONGRATULATIONS :D %s ", hpet_sdt->signature);

        hpet_base = (uint32_t*)hpet_sdt->Base_address[1];
        hpet = (HPET_Table_t*)hpet_base;
      }
      ptr++;
    }
  }
}
