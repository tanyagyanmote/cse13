#!/bin/bash
#Graph1
for((i=2;i<=10000;i++)) #making a for loop to run {2..10000}
do
	./collatz -n "$i" > num.dat #opening collatz, creating and storing the sequence in num.dat
	echo $(wc -l num.dat | awk '{print $1}') >> y_val.dat #reading num.dat and storing it
	echo $i >> x_val.dat #storing all x values in x_val.dat
done

paste x_val.dat y_val.dat > graph1.dat

rm x_val.dat
rm y_val.dat

gnuplot <<END
    set terminal pdf
    set output "len.pdf"
    set title "Collatz Sequence Lengths"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    set yrange [0:300]
    set ytics 50
    set xrange [0:10000]
    set xtics 1000
    plot "graph1.dat" with dots notitle

END

#Graph2
for ((i=2; i<=10000;i++))
do
	./collatz -n “$i” > num.dat #opening collatz, creating and storing the sequence in num.dat
	sort -nr num.dat > sort_num.dat #sorting num.dat and setting it to sort_num.dat
	(head -1 < sort_num.dat) >> y2_val.dat #taking the head from sort and setting it to y2
	echo $i >> x2_val.dat #creating x val dat
done

paste x2_val.dat y2_val.dat > graph2.dat

rm x2_val.dat
rm y2_val.dat
rm sort_num.dat

gnuplot <<END
    set terminal pdf
    set output "val.pdf"
    set title "Maximum Collatz Sequence Value"
    set xlabel "n"
    set ylabel "value"
    set zeroaxis
    set yrange [0:100000]
    set ytics 20000
    set xrange [0:10000]
    set xtics 1000
    plot "graph2.dat" with dots notitle

END

#Graph3
for ((i=2; i<=10000;i++))
do
	./collatz -n “$i” > num.dat  #opening collatz, creating and storing the sequence in num.dat
	echo $(wc -l num.dat | awk '{print $1}') >> x3_val.dat #counts num.dat puts into a file
done
sort -n x3_val.dat | uniq -c > graph3.dat #frequency of x values

rm x3_val.dat

gnuplot <<END #gnu plot values
    set terminal pdf
    set output "freq.pdf"
    set title "Collatz Sequence Length Histogram"
    set xlabel "length"
    set ylabel "frequency"
    set zeroaxis
    set yrange [0:200]
    set xrange [0:225]
    set boxwidth 0.1
plot "graph3.dat" using 2:1 with boxes notitle
END

#Graph4
for((i=2;i<=10000;i++)) #making a for loop to run {2..10000}
do
./collatz -n "$i" > num.dat #opening collatz, creating and storing the sequence in num.dat
echo $(wc -l num.dat | awk '{print $1}') >> y4_val.dat #reading num.dat and storing it
echo $i >> x4_val.dat #storing all x values in x_val.dat
done

paste x4_val.dat y4_val.dat > graph4.dat

rm x4_val.dat
rm y4_val.dat

gnuplot <<END
    set terminal pdf
    set output "len.pdf"
    set title "Collatz Length Sequence"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    set yrange [0:100]
    set ytics 10
    set xrange [0:1000]
    set xtics 100
    plot "graph4.dat" with dots notitle

END

