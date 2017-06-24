#include <thread>
#include <mutex>
#include <unordered_map>

using namespace std;

template<class _Kty, class _Ty, class _Hasher = hash<_Kty>, class _Keyeq = equal_to<_Kty>,
	class _Alloc = allocator<pair<const _Kty, _Ty>>>
class safe_unordered_map : protected unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc> {
// use protected inheritance to prevent direct access to unsafe interface
public:
	typedef unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc> Unsafe;
	mutex mtx;

	mapped_type& operator[](const key_type& _Keyval)
	{
		mtx.lock();
		mapped_type &result = Unsafe::operator[](_Keyval);
		mtx.unlock();
		return result;
	}

	size_type erase(const key_type& _Keyval)
	{
		mtx.lock();
		size_type result = Unsafe::erase(_Keyval);
		mtx.unlock();
		return result;
	}

	iterator find(const key_type& _Keyval)
	{
		// mtx.lock();
		iterator result = Unsafe::lower_bound(_Keyval);
		// mtx.unlock();
		return result;
	}

	iterator end() _NOEXCEPT
	{
		// mtx.lock();
		iterator result = Unsafe::end();
		// mtx.unlock();
		return result;
	}
};