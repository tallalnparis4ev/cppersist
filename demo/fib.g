LoadPackage("PackageManager");
LoadPackage("Memoisation");

FibMemo := MemoisedFunction(
function(n)
  if n = 0 then return 0; fi;
  if n = 1 then return 1; fi;
  return FibMemo(n-1) + FibMemo(n-2);
end,
rec(funcname := "fib",
    cache := "file://persist",
    hash := k -> StringFormatted("{}", k[1]),
    pickle := String,
    unpickle := Int));;

FibMemo(5);

FibMongoMemo := MemoisedFunction(
function(n)
  if n = 0 then return 0; fi;
  if n = 1 then return 1; fi;
  return FibMongoMemo(n-1) + FibMongoMemo(n-2);
end,
rec(funcname := "fib",
    cache := "mongodb://localhost:5000/persist",
    hash := k -> StringFormatted("{}", k[1]),
    pickle := String,
    unpickle := Int));;

FibMongoMemo(5);