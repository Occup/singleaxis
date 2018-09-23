set terminal svg size 1920,600 enhanced font 'Verdana,10'
unset key
set output 'pic.svg' 
set object 1 rectangle from screen 0,0 to screen 1,1 fc rgb 'white10' behind
set xlabel 'X'
set ylabel 'Y'
set size ratio -1
set grid
plot 'temp' u 1:2 w l,'temp' u 1:3 w l
