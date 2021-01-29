from eve import Eve
import sys, os
# Disable
def blockPrint():
    sys.stdout = open(os.devnull, 'w')

app = Eve()
blockPrint()
print("hey")
if __name__ == "__main__":
    app.run()
