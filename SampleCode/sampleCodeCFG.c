procedure First {
	x = 2;
	z = 3;
	call Second; 
}

procedure Second {
	x = 0;
	i = 5;
	while i {
		x = x + 2 * y;
		call Third;
		i = i - 1; 
		if x then 
		{
			x = x + 1; 
		}
		else
		{
			z = 1;
		}
	}
	z = z + x + i;
	y = z + 2;
	x = x * y + z; 
}

procedure Third {
	z = 5;
	v = z; 
	if z then
	{
		a = 1;
		while x
		{
			x = x + 1;
			z = 0;
		}
	}
	else
	{
		b = 0;
		while y
		{
			y = y + 1;
		}
	}
	p = 0;
}