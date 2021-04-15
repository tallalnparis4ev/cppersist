from eve import Eve
import sys, os
# Disable
def blockPrint():
    sys.stdout = open(os.devnull, 'w')

app = Eve()
blockPrint()
print("hey")

port_arg = 5000
try:
    port_arg = int(sys.argv[1])
except:
    pass
if __name__ == "__main__":
    app.run(port=port_arg)
