     procedure Alpha
     {
   a = 0;
    b = 1;
    call Bravo;
    i = 7;
     }

     procedure Bravo
     {
while c{
    if b then
        {
           n = 3;
        }
        else
        {
            c = 6;
            call Charlie;
            i = 4;
            call Charlie;
        }
}
     }

     procedure Charlie
     {
    b = 15;
    i = a + 1;
    while i
        {
            a = a + 1;
            i = i - 1;
        }
     }