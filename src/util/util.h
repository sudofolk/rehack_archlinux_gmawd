inline uintptr_t getAbsoluteAddress(uintptr_t ptr, int offset, int size) {
	return ptr + *reinterpret_cast<int32_t*>(ptr + offset) + size;
};

template<typename out, class type>
inline out method(size_t index, type* self){
	return reinterpret_cast<out>((*reinterpret_cast<void***>(self))[index]);
}

namespace PatternScan {
    std::vector<uintptr_t> findMatches(const std::string &pattern, uintptr_t addr, size_t size);
    std::vector<uintptr_t> findMatchesInModule(const char* moduleName, const std::string &pattern);
    uintptr_t findFirstInModule(const char* moduleName, const std::string &pattern);
}