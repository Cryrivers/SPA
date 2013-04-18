procedure Alpha{
	while i {
		a = 0;		
		call YueCong; 
		b = a + 1; 	
		c = a + b; 	 
		d = c + d; 
		call Bravo;   
		i = 7;
		call LeiDong;
	}
}

procedure Bravo{
	while c{
		if b then
		{
			n = 3; 
			call LeiDong;
		}
		else
		{
			c = 6+b;
			call Charlie;
			i = 4;
			call YueCong;
		}
	}
}	

procedure YueCong{			   
	i = a + 1;
	if b then   {
		while i	 {
			a = a + b;
			call Charlie;
			while a {   
				b = i - 1;
				call LeiDong;
			}
			b = c + 1;
		}
	}else{
		call LeiDong;
	}
}

procedure LeiDong{
	c = b + i;
	call Charlie; 
	d = a + c;
}

procedure Charlie{				 
	i = a + 1;
	while i
	{
		a = a + c;
		i = i - c;
		b = i + d;
	}
}