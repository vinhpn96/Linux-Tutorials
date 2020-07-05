#include <stdio.h>

extern const char _binary_text_txt_start[];

int main()
{
	printf("[%s]\n", __func__);
	printf("text.txt file:%s", _binary_text_txt_start);
}
