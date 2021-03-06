#ifndef MONGO_H_FILE
#define MONGO_H_FILE

#include "crypto/sha256.h"
#include "memoization/persistent/memoizer.hpp"

namespace cpst {
/**
 * \defgroup PersistMongo Persist MongoDB
 * @{
 */

/**
 * Creates an object that has persistent memoization applied
 * to T's solve function. Here, the storage medium for persistent memoization
 * is mongoDB.
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 * @param dbURL - this corresponds to the URL of the mongoDB server.
 * @param hash - this is applied to the key produced by the key function, the
 * string it returns will be used as the final key for cache entries. Default =
 * sha256.
 */
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getMongoMemoizedObj(
    string (*key)(Args...), string (*pickle)(Ret), Ret (*unpickle)(string),
    string dbURL, string (*hash)(string) = sha256);

/**
 * Creates an object that has persistent memoization applied
 * to T's solve function. Here, the storage medium for persistent memoization
 * is mongoDB.
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 * @param dbURL - this corresponds to the URL of the mongoDB server.
 * @param funcName - this corresponds to which collection in the 'cppersist'
 * database results should be persisted. If this is not passed in, the name
 * typeid(T).name() is used instead.
 * @param hash - this is applied to the key produced by the key function, the
 * string it returns will be used as the final key for cache entries. Default =
 * sha256.
 */
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getMongoMemoizedObj(
    string (*key)(Args...), string (*pickle)(Ret), Ret (*unpickle)(string),
    string dbURL, string funcName, string (*hash)(string) = sha256);

/**
 * Creates an object that has persistent memoization applied
 * to T's solve function. Here, the storage medium for persistent memoization
 * is mongoDB.
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param primaryCache - this will initialise the specified memory cache and use
 * it as the primary cache.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 * @param dbURL - this corresponds to the URL of the mongoDB server.
 * @param hash - this is applied to the key produced by the key function, the
 * string it returns will be used as the final key for cache entries. Default =
 * sha256.
 */
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getMongoMemoizedObj(
    MemCacheType primaryCache, string (*key)(Args...), string (*pickle)(Ret),
    Ret (*unpickle)(string), string dbURL, string (*hash)(string) = sha256);

/**
 * Creates an object that has persistent memoization applied
 * to T's solve function. Here, the storage medium for persistent memoization
 * is mongoDB.
 * @tparam T - the class which the user wants to instantiate an object of,
 * with a memoized version of the solve function.
 * @param primaryCache - this will initialise the specified memory cache and use
 * it as the primary cache.
 * @param key - this function should return a string representation for a
 * given argument to the solve function.
 * @param pickle - this function should return a string representation
 * for any return of the solve function.
 * @param unpickle - this function is the inverse of pickle.
 * @param dbURL - this corresponds to the URL of the mongoDB server.
 * @param funcName - this corresponds to which collection in the 'cppersist'
 * database results should be persisted. If this is not passed in,
 * typeid(T).name() is used instead.
 * @param hash - this is applied to the key produced by the key function, the
 * string it returns will be used as the final key for cache entries. Default =
 * sha256.
 */
template <typename T, typename Ret, typename... Args>
PersistentMemoized<T, Ret, Args...> getMongoMemoizedObj(
    MemCacheType primaryCache, string (*key)(Args...), string (*pickle)(Ret),
    Ret (*unpickle)(string), string dbURL, string funcName,
    string (*hash)(string) = sha256);

/**@}*/
}  // namespace cpst
#include "mongo.cpp"
#endif