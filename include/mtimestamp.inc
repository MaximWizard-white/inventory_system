#if !defined UNIX_MINUTE
    const UNIX_MINUTE = 60;
#endif
#if !defined UNIX_HOUR
    const UNIX_HOUR = UNIX_MINUTE*60;
#endif
#if !defined UNIX_DAY
    const UNIX_DAY = UNIX_HOUR*24;
#endif
#if !defined UNIX_MONTH
    const UNIX_MONTH = UNIX_DAY*30;
#endif
#if !defined UNIX_YEAR
    const UNIX_YEAR = UNIX_MONTH*12;
#endif

stock UnixTimeConvert(array[], timestamp, array_size = sizeof(array))
{
    if(array_size < 14)
        return 0;
    array[0] = EOS;
    new 
        compare = gettime(), 
        time = (timestamp > compare) ? 
        (timestamp - compare) : (compare - timestamp), remainder;
    if(time < UNIX_MINUTE)
    {
        remainder = time;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "секунда", array_size);
            case 2, 3, 4:
                strcat(array, "секунды", array_size);
            default:
                strcat(array, "секунд", array_size);
        }
    }
    else if(time < UNIX_HOUR)
    {
        remainder = time = time / UNIX_MINUTE;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "минута", array_size);
            case 2, 3, 4:
                strcat(array, "минуты", array_size);
            default:
                strcat(array, "минут", array_size);
        }
    }
    else if(time < UNIX_DAY)
    {
        remainder = time = time / UNIX_HOUR;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "час", array_size);
            case 2, 3, 4:
               strcat(array, "часа", array_size);
            default:
                strcat(array, "часов", array_size);
        }
    }
    else if(time < UNIX_MONTH)
    {
        remainder = time = time / UNIX_DAY;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "день", array_size);
            case 2, 3, 4:
                strcat(array, "дня", array_size);
            default:
                strcat(array, "дней", array_size);
        }
    }
    else if(time < UNIX_YEAR)
    {
        remainder = time = time / UNIX_MONTH;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "месяц", array_size);
            case 2, 3, 4:
                strcat(array, "месяца", array_size);
            default:
                strcat(array, "месяцев", array_size);
        }
    }
    else
    {
        remainder = time = time / UNIX_YEAR;
        if((remainder %= 100) > 19)
            remainder %= 10;
        switch(remainder)
        {
            case 1:
                strcat(array, "год", array_size);
            case 2, 3, 4:
                strcat(array, "года", array_size);
            default:
                strcat(array, "лет", array_size);
        }
    }
    format(array, array_size, "%d %s", time, array);
    return 1;
} 