from pypersist import persist

@persist(key=lambda x: "{x}".format(x=x),
         hash=lambda k: "{k}".format(k=k),
         pickle=str,
         unpickle=int,
         verbosity=4)
def fib(n):
    if n==0:
      return 0
    if n==1:
      return 1
    return fib(n-1) + fib(n-2)

print(fib(6))

@persist(key=lambda x: "{x}".format(x=x),
         hash=lambda k: "{k}".format(k=k),
         pickle=str,
         unpickle=int,
         verbosity=4,
         cache="mongodb://localhost:5000/persist")
def fib(n):
    if n==0:
      return 0
    if n==1:
      return 1
    return fib(n-1) + fib(n-2)
  
print(fib(6))
