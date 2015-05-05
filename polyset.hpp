#ifndef COM_MASAERS_POLYSET_HPP
#define COM_MASAERS_POLYSET_HPP

#include "pointer_union.hpp"
#include <utility>
#include <set>

namespace com_masaers {

	template<typename...>
	class polyset;

	template<>
	class polyset<> {
	protected:
		template<typename T> const void* do_find(const T& value) const {
		  static_assert(std::is_same<T, void>::value, "Cannot find unsupported type in a polyset.");
		  return NULL;
		}
		template<typename T> std::pair<const void*, bool> do_insert(const T& value) {
			static_assert(std::is_same<T, void>::value, "Cannot insert unsupported type into a polyset.");
		  return std::pair<const void*, bool>(NULL, false);
		}
	public:
	  template<typename T> void erase(T&& value) {
	    static_assert(std::is_same<T, void>::value, "Cannot erase unsupported type from a polyset.");
	  }
	  bool empty() const { return true; }
	  std::size_t size() const { return 0; }
	  void clear() {}
	}; // polyset base case

	template<typename Value, typename... Values>
	class polyset<Value, Values...> : public polyset<Values...> {
	protected:
		typedef polyset<Values...> base_type;
		typedef std::set<Value> store_type;
		typedef ptrunion<typename std::add_const<Value>::type, typename std::add_const<Values>::type...> ptr_type;
		store_type store_m;
		const void* do_find(const Value& value) const {
			const void* result = NULL;
			auto it = store_m.find(value);
			if (it != store_m.end()) {
				result = static_cast<const void*>(&*it);
			}
			return result;
		}
		template<typename T> auto do_find(const T& value) const -> decltype(base_type::do_find(value)) {
		  return base_type::do_find(value);
		}
		std::pair<typename store_type::iterator, bool> do_insert(const Value& value) {
			return store_m.insert(value);
		}
		template<typename T> auto do_insert(const T& value) -> decltype(base_type::do_insert(value)) {
			return base_type::do_insert(value);
		}
	public:
		template<typename T> ptr_type find(const T& value) const {
		  return ptr_type(static_cast<const typename std::decay<T>::type*>(do_find(value)));
		}
		template<typename T> std::pair<ptr_type, bool> insert(const T& value) {
		  auto p = do_insert(value);
		  return std::pair<ptr_type, bool>(static_cast<const typename std::decay<T>::type*>(&*p.first), p.second);
		}
		decltype(std::declval<store_type>().erase(std::declval<const Value&>()))
		erase(const Value& value) {
			return store_m.erase(value);
		}
		template<typename T> auto erase(const T& value) -> decltype(base_type::erase(value)) {
			return base_type::erase(value);
		}
		bool empty() const { return store_m.empty() && base_type::empty(); }
		std::size_t size() const { return store_m.size() + base_type::size(); }
		void clear() { store_m.clear(); base_type::clear(); }
	}; // polyset inductive case

} // namespace com_masaers


#endif
