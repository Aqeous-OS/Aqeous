#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"
#include "sys.h"
#include "Shell.h"
#include "vesa.h"

void idle()
{
  //scheduler();
  while(1)
  {
    //asm volatile("cli");
    printf(" 1-x-");
    //asm volatile("sti");
  }
}

void idle2()
{
  //scheduler();
  /**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*50) break;
    ++i;
    if(i%2)
      _printf("2");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--2%x--",i);
    ++i;
    //asm volatile("sti");
  }
  //kill();
  /**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 2-x-");
    asm volatile("sti");
  }//*/
}

void idle3()
{
  //scheduler();
  /**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*30) break;
    i++;
    if(i%2)
      _printf("3");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--3%x--",i);
    ++i;
    //asm volatile("sti");
  }/**/
  while(1)
  {
    //asm volatile("cli");
    _printf(" 3-x-");
    //asm volatile("sti");
  }//*/
}

void idle4()
{
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*10) break;
    i++;
    if(i%2)
      _printf("4");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--4%x--",i);
    i++;
    //asm volatile("sti");
  }/**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 4-x-");
    asm volatile("sti");
  }//*/
}

void idle5()
{
  //asm volatile("cli");
  //while(1);
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      _printf("5");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
  //  kill();
    //asm volatile("cli");
    if(i%2)
      _printf("--5%x--",i);
    i++;
    //asm volatile("sti");
  }
  _printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
  /**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 5-x-");
    asm volatile("sti");
  }//*/
}

void idle6()
{
  //asm volatile("cli");
  //int ij=0;
  //while(1)
  //ij++;
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*70) break;
    i++;
    if(i%2)
      _printf("6");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--6%x--",i);
    i++;
    //asm volatile("sti");
  }
  _printf("\neverything worked fine :D MULTITASKING WORKS :D \n");

/**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 6-x-");
    asm volatile("sti");
  }//*/
}

void tasking_initiator()
{
  current_task = (uint32_t)Idle_task;
  printf("\n\n\n\t-------------MISSION ACCOMPLISHED-------------\n\n-------------Welcome to the MultiThreading World!!!-------------\n");
  printf("\n----Spawning 5 Immortal Idle Tasks, each would print a unique number----\n\tStarting in 3...2...1... GO...\n\n");
  delay1(14);
  init_shell();
  //setVesa(0x110); //TEXT MODE VESA :v
  //init_hpet();
  apic_start_timer();       //The respective Timer initialization function of the timer of choice

  asm volatile("sti");
  while(1);
}

extern void kernel_main();

void init_multitasking()
{
  asm volatile("cli");
  //memset((void*)(200*1024*1024),0,(40*1024*1024));
  kernel_proc = create_process("microkernel", 0, 1, 0);
  kernel_proc->pgdir = system_dir;

  new_process = (uint32_t)kernel_proc;

  current_task = (uint32_t)create_task("initiating_task",tasking_initiator, 20, 0x202, kernel_proc);  //Scheduler initalization task
  old_task = current_task;

  Idle_task = create_task("System_idle_task",idle, 20, 0x202, kernel_proc);  //default task
  Activate_task_direct(Idle_task);

  Activate_task_direct(create_task("idle2",idle2, 10, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle3",idle3, 10, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle4",idle4, 10, 0x202, kernel_proc));
  //Activate_task_direct(create_task("idle5",idle5, 10, 0x202, kernel_proc));
  //Activate_task_direct(create_task("idle6",idle6, 10, 0x202, kernel_proc));
  //Activate_task_direct(create_task("Main_Kernel",kernel_main, 10, 0x202, kernel_proc));

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Activate_task_direct(create_task("Shell_Ostream", Console_Writer, 10, 0x202, Shell_proc)); //This is the task which would make printing to console possible!
  Activate_task_direct(create_task("Shell_Istream", Shell_Input, 10, 0x202, Shell_proc));
  Shell_task = create_task("Shell_task", Shell, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_task);
  //Activate_task_direct(create_task("Test_Vesa", Shell_sleep, 10, 0x202, Shell_proc));

  reached_bottom = 0;
  Scheduler_init(); // Let the FUN Begin :D Lets Switch from the old monotasking world to Multitasking World :D defined in tasking.asm
  while(1); //Never comeback :D
}
