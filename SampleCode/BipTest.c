procedure Alpha
{
	a = 0;
	b = 1;
	call Bravo;
	i = 7;
}

procedure Bravo
{
	if b then
	{
		a = 3;
	}
	else
	{
		a = 6;
		call Charlie;
		i = 4;
		call Charlie;
	}
	
}

procedure Charlie
{
	a = 15;
	i = a + 1;
	while i
	{
		a = a + 1;
		i = i - 1;
	}
}
