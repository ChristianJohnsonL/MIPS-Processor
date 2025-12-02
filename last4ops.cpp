
if (rFormat)
{
    for (int i = 5; i >= 0; i--)
    {
        if (binary[i] == 1)
        {
            result += pow(2, abs(i - 5));
        }
    }
    registers[0] = result;
    index += 6;
    result = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = index + 5; j > index; j--)
        {
            if (binary[j] == 1)
            {
                result += pow(2, abs(j - (index + 5)));
            }
        }
        registers[i] = result;
        index += 5;
        result = 0;
    }

    for (int i = index; i < index + 6; i++)
    {
        if (binary[j] == 1)
        {
            result += pow(2, abs(j - index));
        }
    }
    registers[4] = result;
}
else
{
}
}