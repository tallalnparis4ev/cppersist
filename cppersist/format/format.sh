find .. -type d \( -name build \) -prune -false -o -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | while read line; do
	clang-format -i --style=file $line    
done
