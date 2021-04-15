#if !defined(MEMOIZABLE_H_FILE)
#define MEMOIZABLE_H_FILE
namespace cpst {
/**
 * A helper function for memoization, will return the value you give input.
 * Common use case: identity<string> for a hashing function.
 */
template <typename T>
T identity(T obj) {
  return obj;
}

/**
 * A helper function for memoization, will return true regardless of input.
 * Common use case: returnTrue<Args...> to reset your decision function.
 */
template <typename... Args>
bool returnTrue(Args... args){
  return true;
}

/**
 * This is an abstract class, and a class which a user wants to memoize
 * must inherit from this class.
 */
template <typename Ret, typename... Args>
class Memoizable {
 public:
  /**
   * This is a user-defined solve function, which the user wants to
   * memoize. It can be recursive (i.e. call solve()), and these
   * recursive calls will also be memoized. For the memoization to be
   * accurate, this function must be pure.
   */
  virtual Ret solve(Args... args) {
    throw std::invalid_argument("Memoizable::solve should never be called");
  };
};
}  // namespace cpst
#endif  // MEMOIZABLE_H_FILE