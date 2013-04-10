procedure First {
	x = 2;
	z = 3;
	call Second; 
}

procedure Second {
	x = 0;
	i = 5;
	call Third;
	call Fourth;
	x = x * y + z; 
}

procedure Third {
	if c then {
		z = 5;
		v = z; 
		call Fourth;
	} else {
		v = 0;
	}
}

procedure Fourth {
	while a {
		b = 0;
		c = 0;
	}
}