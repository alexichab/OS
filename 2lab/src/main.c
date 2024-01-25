#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define attribute(value) __attribute__((value))

//Размеры

#define MMU_PAGE_TABLE_ENTRIES_COUNT 128 //изменяем ее для наглядности

// PDE struct
    struct page_directory_entry_t {
  unsigned char p : 1;
  unsigned char r_w : 1;
  unsigned char u_s : 1;
  unsigned char w_t : 1;
  unsigned char c_d : 1;
  unsigned char a : 1;
  unsigned char z : 1;
  unsigned char p_s : 1;
  unsigned char i : 1;
  unsigned char ava : 3;
  unsigned int p_table_addr : 20;
} attribute(packed);

// PTE struct
struct page_table_entry_t {
  unsigned char p : 1;
  unsigned char r_w : 1;
  unsigned char u_s : 1;
  unsigned char w_t : 1;
  unsigned char c_d : 1;
  unsigned char a : 1;
  unsigned char d : 1;
  unsigned char z : 1;
  unsigned char g : 1;
  unsigned char ava : 3;
  unsigned int p_phys_addr : 20;
} attribute(packed);

//Структуры

static struct page_directory_entry_t p_dir attribute(aligned(4096));

static struct page_table_entry_t p_tab[MMU_PAGE_TABLE_ENTRIES_COUNT] attribute(aligned(4096));


//Инициализация каталога страниц // 4mb
void mmu_init() 
{
  //Инициализация каталога нулями
  memset(&p_dir, 0, sizeof(struct page_directory_entry_t));

  //Каталог(флаги)
  p_dir.z = 1;
  p_dir.a = 0;
  p_dir.ava = 0;
  p_dir.c_d = 0;
  p_dir.i = 0;
  p_dir.p_s = 0;
  p_dir.p = 1;
  p_dir.r_w = 1;
  p_dir.u_s = 1;
  p_dir.w_t = 1;
  //Адрес первой таблицы
  p_dir.p_table_addr = (uintptr_t)p_tab >> 12;
  
  //Таблица страниц(флаги)
  for (int i = 0; i < MMU_PAGE_TABLE_ENTRIES_COUNT; ++i) {
    //Таблица страниц(флаги)
    p_tab[i].z = 0;
    p_tab[i].a = 0;
    p_tab[i].ava = 0;
    p_tab[i].c_d = 0;
    p_tab[i].d = 0;
    p_tab[i].g = 1;
    p_tab[i].p = 1;
    p_tab[i].r_w = 1;
    p_tab[i].u_s = 1;
    p_tab[i].w_t = 1;
    //Физический адрес
    p_tab[i].p_phys_addr = (i * 4096) >> 12; //
  }
}
//Получение каталога страниц
extern struct page_directory_entry_t *mmu_get_dir() { return &p_dir; }
//Получение таблицы страниц
extern struct page_table_entry_t *mmu_get_tab() { return p_tab; }
//Вывод на экран
void mmu_print() {
  printf("present: %d ", p_dir.p);
  printf("read_write: %d ", p_dir.r_w);
  printf("user_supervisor: %d ", p_dir.u_s);
  printf("write_through: %d ", p_dir.w_t);
  printf("cache_disabled: %d ", p_dir.c_d);
  printf("accessed: %d ", p_dir.a);
  printf("zero: %d ", p_dir.z);
  printf("page_size: %d ", p_dir.p_s);
  printf("ignored: %d ", p_dir.i);
  printf("available: %d\n\n", p_dir.ava);
  printf("\tpage_dir_phys_addr: %p\n", (void *)mmu_get_dir());
  printf("\tpage_table_addr: 0x%x000\n\n\n", p_dir.p_table_addr);

printf("zero\taccessed\tavailable\tcache_disabled\tdirty\tglobal\tpresent\tread_write\tuser_supervisor\twrite_through\tpage_virt_addr\tpage_phys_addr\n");

//Таблица страниц
printf("\t\tTable address\t\tpoint to\n");
    for (int i = 0; i < MMU_PAGE_TABLE_ENTRIES_COUNT; ++i) {
        // printf("%1d\t", p_tab[i].z);
        // printf("%5d\t", p_tab[i].a);
        // printf("%9d\t", p_tab[i].ava);
        // printf("%9d\t", p_tab[i].c_d);
        // printf("%13d\t", p_tab[i].d);
        // printf("%5d\t", p_tab[i].g);
        // printf("%5d\t", p_tab[i].p);
        // printf("%5d\t", p_tab[i].r_w);
        // printf("%9d\t", p_tab[i].u_s);
        // printf("%13d\t", p_tab[i].w_t);
        printf("\t\t%p\t", (void *)&p_tab[i]);
    printf("\t\t\t0x%-4x\t", p_tab[i].p_phys_addr);
    printf("\n");
    }
}

int main(int argc, const char *argv[]) {

  mmu_init();
  mmu_print();

  printf("\n");

  return 0;
}