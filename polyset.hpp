#ifndef COM_MASAERS_POLYSET_HPP
#define COM_MASAERS_POLYSET_HPP

#include <utility>
#include <set>

namespace com_masaers {

	template<typename...>
	class polyset;

	template<>
	class polyset<> {
	public:
		template<typename T> void insert(T&& value) {
			static_assert(std::is_same<T, void>::value, "Cannot insert unsupported type into a polyset.");
		}
		template<typename T> void find(T&& value) const {
  		static_assert(std::is_same<T, void>::value, "Cannot find unsupported type in a polyset.");
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
		store_type store_m;
	public:
		auto insert(const Value& value) -> decltype(this->store_m.insert(value)) {
			return store_m.insert(value);
		}
		template<typename T> auto insert(const T& value) -> decltype(base_type::insert(value)) {
			return base_type::insert(value);
		}
		auto find(const Value& value) -> decltype(this->store_m.find(value)) {
			return store_m.find(value);
		}
		template<typename T> auto find(const T& value) -> decltype(base_type::find(value)) {
		  return base_type::find(value);
		}
		bool empty() const { return store_m.empty() && base_type::empty(); }
		std::size_t size() const { return store_m.size() + base_type::size(); }
		void clear() {
			store_m.clear();
			base_type::clear();
		}
	}; // polyset inductive case

} // namespace com_masaers


#endif
