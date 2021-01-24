#ifndef CACHE_H_FILE
#define CACHE_H_FILE

#include <optional>
#include <string>
namespace cpst {
/**
 * This is an abstract class representing a generic cache.
 * @tparam Ret - this specifies the type that this cache returns
 * @tparam Args... - this represnts the types of the
 */
template <typename Ret, typename... Args>
class Cache {
 public:
  /**
   * This function should attempt to fetch from the cache the value
   * corresponding to the passed in arguments. If no such entry exists,
   * then a nullopt should be returned.
   * @param args - The arguments to be searched from the cache.
   * @returns the value contained in the cache, corresponding to the
   * key formed by args (if no such key exists, the nullopt is returned).
   */
  virtual std::optional<Ret> get(const Args&... args) = 0;
  virtual void put(const Args&... args, const Ret& ret) = 0;
  virtual Cache* clone() = 0;
  /**
   * Virtual destructor
   */
  virtual ~Cache(){};
  /**
   * Getter for the key function of the cache
   * @returns the key function of the cache
   */
  auto getKey() { return key; }
  /**
   * Getter for the pickle function of the cache
   * @returns the pickle function of the cache
   */
  auto getPickle() { return pickle; }
  /**
   * Getter for the unpickle function of the cache
   * @returns the unpickle function of the cache
   */
  auto getUnpickle() { return unpickle; }

 protected:
  /**
   * a function variable.
   * Every cache should have a user defined key method. This method
   * should create a string corresponding to the given arguments
   */
  std::string (*key)(Args...);
  /**
   * a function variable.
   * Every cache should have a user defined pickle method. This method
   * constructs a string representation of a given instance of the 'Ret' type.
   */
  std::string (*pickle)(Ret);
  /**
   * a function variable.
   * Every cache should have a user defined unpickle method. This method
   * constructs an instance of the 'Ret' type from a string representation.
   */
  Ret (*unpickle)(std::string);
  std::string (*hash)(std::string);
};
}  // namespace cpst
#endif