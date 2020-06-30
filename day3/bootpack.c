void io_hlt(void);
void write_mem8(int addr, int data);
void HariMain(void)
{
    int i;
    for (i = 0xa0000; i<=0xaffff;i++)
    {
        wrtie_mem8(i,i%15);
    }
    for(;;)
    {
        io_hlt();
    }
        
}