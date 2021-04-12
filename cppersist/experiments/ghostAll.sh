start=5000000
for counter in {1..11}
do
   ./runWORep.sh 10 $start 0 
   start=$(($start+1000000))
done
