//



int main()
{
	char* vidmem = (char*) 0xb8000;
	for(int i = 0; i < (2 * 25 * 80); i += 2)
		*(vidmem + i) = 0;

	*vidmem = 'X';
}
