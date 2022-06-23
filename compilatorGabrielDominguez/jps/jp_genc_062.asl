func f(a : array[3] of int, b : int) : int
  return b + a.max;
endfunc

func g(a : char, b: array[3] of char) : char
  if a > b.max then
    return b.max;
  else
    return b[2];
  endif
endfunc

func main()
  var a : array[3] of int
  var b : array[3] of char

  a[0] = 7;   a[1] = 3;   a[2] = 14;
  b[0] = 'o'; b[1] = 'c'; b[2] = 'a';

  write "a) f(a, 23)="; write f(a, 23); write "\n";
  write "b) g('z', b)="; write g('z', b); write "\n";
  write "c) g('e', b)="; write g('e', b); write "\n";
endfunc
