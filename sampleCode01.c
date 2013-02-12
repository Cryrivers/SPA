procedure Bill {
	z = 10;
	x = z + 5;
	y = x * z + 4;
	d = x + y * 12;
	call John;
	d = d + z + x;
	call Jane;
	e = d + z * x;
	while i {
		e = x - y;
		if z then {
			y = y + z;
			i = y + 1; }
		else {
			x = y + z;
			call Jane;
			i = i - 1; }
		x = y + z; }
	call Mary; }

procedure Mary {
	while i {
		z = y*3 + 2*x;
		call John;
		i = i - 1; }
	y = y + z + i; }

procedure Jane {
	while d {
		z = x + z; }
	x = z + x; }

procedure John {
	while e {
		if i then {
			y = z + 1; }
		else {
			z = z * x + y + e * d; } } }