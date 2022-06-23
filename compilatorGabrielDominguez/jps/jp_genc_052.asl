func init(aC : array[6] of float)
  aC[0]=-5.4; aC[1]=-3.7; aC[2]=-8.2;
  aC[3]=-5.1; aC[4]=-7.9; aC[5]=-10;  
endfunc

func main()
  var a : array[6] of float
  var b : array[3] of char
  var i : int

  init(a);
  b[0] = 'h'; b[1] = 'b'; b[2] = 'o';
  i = 0;
  while i < 6 do
    write "a["; write i; write "]=";
    write a[i]; write "\n";
    i = i + 1;
  endwhile
  i = 1000;
  while i < 1003 do
    write "b["; write i-1000; write "]=";
    write b[i-1000]; write "\n";
    i = i + 1;
  endwhile
  write "a) a.max="; write a.max; write "\n";
  write "b) b.max="; write b.max; write "\n";
endfunc
