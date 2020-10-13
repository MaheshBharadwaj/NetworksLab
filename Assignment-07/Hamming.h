int countbits(long num)
{
    int r, count = 0;
    while(num > 0)
    {
        num = num / 10;
        count++;
    }
    return count;
}
int binary(int num)
{
    int bin = 0, r;
    int i = 0;
    while(num > 0)
    {
        r = num % 2;
        bin += r * pow(10, i);
        num /= 2;
        i++;
    }
    return bin;
}
int ispresent(int num,int pos)
{
    int rem;
    for(int i = 0; i < pos; i++)
    {
        rem = num % 10;
        num = num / 10;
    }
    if(rem == 1)
        return 1;
    else
        return 0;
}
int isapower2(int n)
{
    if(ceil(log2(n)) == floor(log2(n)))
        return 1;
    else
        return 0;
}

int decimal(int num)
{
    int rem, i = 0, result;
    while(num > 0)
    {
        rem = num % 10;
        result += pow(2, i) * rem;
        num /= 10;
        i++;
    }
    return result;
}