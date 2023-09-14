set datafile separator ","
set terminal png size 1280,1024
set output report_file.'.png'
set xlabel "Message"
set ylabel "Time in ms"

plot \
 report_file using 0:( ( (($5*1000) + ($6/1000000)) - (($1*1000)+($2/1000000)))) with linespoints title "send->receive", \
 report_file using 0:( ( (($3*1000) + ($4/1000000)) - (($1*1000)+($2/1000000)))) with linespoints title "send->dispatch", \
 report_file using 0:( ( (($5*1000) + ($6/1000000)) - (($3*1000)+($4/1000000)))) with linespoints title "dispatch->receive"
