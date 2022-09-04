#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/dev/usart.h"

void reverse(int i,int j, uint8_t arr[]){
	while(i < j)
    {
        char temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        i++;
        j--;
	}
}

void intToBaseNStr(int32_t num, uint8_t arr[], int base){
	if(num == 0) {
        arr[0] = '0';
        arr[1] = '\0';
        return;
    }
    uint16_t i = 0; 
    uint8_t isNegative = 0;
    if(num < 0) { isNegative = 1; num = -num;}

	while(num != 0)
    {
        int x = (num % base);

        if(x <= 9) 
            arr[i] = (char)('0' + x);
        else 
            arr[i] = (char)('A' + x - 10);        

		num = num/base;
		i++;
	}

    if(base > 10) {
        arr[i] = 'x';
        arr[i + 1] = '0';
        i += 2;
    }

    if(isNegative) 
    {
        arr[i++] = '-';
    }

    arr[i] = 0;
	reverse(0,i-1,arr);
}



void baseNStrToInt(uint8_t* str, int32_t* num, uint32_t base)
{
    *num = 0;
    // kprintf("%s",str);
    uint16_t i = 0;
    uint8_t isNegative = 0;
    if(str[i] == '-') {
        i++; isNegative = 1;
    }
    for (; str[i] != 0; ++i) {
        if (str[i]>= '0' && str[i] <= '9')
            *num = *num * base + str[i] - '0';
        else if(str[i] >= 'A' && str[i] <= 'F')
            *num = *num * base + str[i] - 'A' + 10;
        else break;
    }
    if(isNegative) *num = 0 - *num;
}

void __concatstr(uint8_t* dest, uint8_t* src1, uint8_t* src2)
{
    uint16_t i = 0, j = 0;
    while(src1[i]) {dest[i] = src1[i]; i++;}
    while(src2[j]) dest[i++] = src2[j++];
    dest[i] = 0;
}

void __strcpy(uint8_t* dest, uint8_t *src)
{
    uint16_t i;
    for(i = 0; src[i]; i++) dest[i] = src[i];
    dest[i] = 0;
}

uint32_t __strlen(uint8_t *str)
{
    uint32_t i = 0;
    while(str[i]) i++;
    return i;
}

float convertStringToFloat(uint8_t* str)
{
    float whole = 0;
    uint16_t i = 0;
    uint8_t isNegative = 0;
    if(str[i] == '-') {
        i++;
        isNegative = 1;
    }
    for(; str[i] >= '0' && str[i] <= '9'; i++)
    {
        whole = whole * 10 + str[i] - '0';
    }
    float frac = 0, power = 1;;
    if(str[i] == '.') {
        i++; 
        uint16_t j = 0;
        for(j = 0; str[i+j] >= '0' && str[i+j] <= '9'; j++)
        {
            power /= 10.0f;
            frac += power * (str[i+j] - '0');
        }
    }
    return isNegative ? -whole-frac : whole + frac;
}

void convertFloatToString(uint8_t* dest, float val)
{
    uint8_t isNegative = 0;
    if(val < 0) isNegative = 1;
    uint32_t whole = val;
    if(isNegative){
        dest[0] = '-';
        intToBaseNStr(whole, dest+1, 10);
    }
    else 
        intToBaseNStr(whole, dest, 10);
    uint16_t i = __strlen(dest);
    val -= whole;
    if(val)
    {
        dest[i++] = '.';
        while(val)
        {
            val *= 10;
            uint32_t digit = val;
            val -= digit;
            dest[i++] = digit + '0';
        }
    }
    dest[i] = 0;
}

void USART_SEND(uint8_t* str)
{
    _USART_WRITE(USART2, str);
}


void kprintf(uint8_t *format, uint8_t* outvar)
{
    uint8_t* type = format + 1;
    uint8_t strToSend[15];
    for(uint32_t i = 0; i < 15; i++) strToSend[i] = 0;

    switch(*type) {
        case 'c':
            strToSend[0] = *outvar;
            strToSend[1] = 0;
            USART_SEND((uint8_t *) strToSend);
            break;
        case 's':
            USART_SEND(outvar);
            break;
        case 'd':
            intToBaseNStr(*(int32_t*) outvar, strToSend, 10);
            USART_SEND((uint8_t *) strToSend);
            break;
        case 'x':
            intToBaseNStr(*(int32_t*) outvar, strToSend, 16);
            USART_SEND((uint8_t *) strToSend);
            break;
        case 'o':
            intToBaseNStr(*(int32_t*) outvar, strToSend, 8);
            USART_SEND((uint8_t *) strToSend);
            break;
        case 'f':
        {
            convertFloatToString(strToSend, *((float*)outvar));
            USART_SEND(strToSend);
        }
    }
}

void kscanf(uint8_t *format, uint8_t* invar)
{
    uint8_t* type = format + 1;

    switch(*type) {
        case 'c':
            _USART_READ(USART2, (uint8_t *) invar, 1);
            break;
        case 's':
            _USART_READ(USART2, (uint8_t *) invar, (uint16_t) INT32_MAX);
            break;
        case 'd':
        {
            uint8_t inpInt[20];
            _USART_READ(USART2, inpInt, 20);
            baseNStrToInt(inpInt, (int32_t*) invar, 10);
            break;
        }
        case 'x':
        {
            uint8_t inpInt[20];
            _USART_READ(USART2, inpInt, 20);
            baseNStrToInt(inpInt, (int32_t*) invar, 16);
            break;
        }
        case 'o':
        {
            uint8_t inpInt[20];
            _USART_READ(USART2, inpInt, 20);
            baseNStrToInt(inpInt, (int32_t*) invar, 8);
            break;
        }
        case 'f':
        {
            uint8_t inpInt[20];
            _USART_READ(USART2, inpInt, 20);
            *((float *) invar) = convertStringToFloat(inpInt);
            break;
        }
    }
}

// int main()
// {
//     // char c = 'c';
//     // uint32_t a = 123123;
// 	// kprintf((uint8_t*)"%s", (uint8_t*) "Hello");
// 	// kprintf((uint8_t*)"%c", (uint8_t*) &c);
// 	// kprintf((uint8_t*)"%d", (uint8_t*) &a);
// 	// kprintf((uint8_t*)"%x", (uint8_t*) &a);

//     // kscanf((uint8_t*) "%x", (uint8_t*) &a);

// }



