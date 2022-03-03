class bit_vector_ref {
  uint64_t* loc;
  uint64_t offset;

 public:
  // construct a bitvector ref to bit o of int array index l
  bit_vector_ref(uint64_t* l, uint64_t o)
      : loc(l), offset(o) {}

  ~bit_vector_ref() {
    loc = nullptr;
  }

  // allow assignment of bool to bitvec ref
  void operator=(bool b) {
    b ? *loc |= (1 << offset) : *loc &= ~(1 << offset);
  }

  // allow cast of bitvec ref to bool
  operator bool() const {
    return (*loc & (1 << offset)) >> offset;
  }
};

class bit_vector {
  uint64_t* vec;
  uint64_t size;

 public:
  // construct all-0 bit vector of size len
  bit_vector(std::size_t len)
      : size(len), vec(new uint64_t[(len / 64) + 1]()) {}

  // construct bit vector from a string of integers (should be 1's and 0's)
  bit_vector(const std::string& state) : bit_vector(state.size()) {
    for (int i = 0; i < state.size(); i++) {
      this[i] = state[i] - '0';  // convert char to corresponding int
    }
  }
  
  // copy constructor
  bit_vector(const bit_vector& other)
      : size(other.size),
        vec((uint64_t*) memcpy(new uint64_t[(other.size / 64) + 1], other.vec, sizeof(other.vec))) {}

  // move constructor
  bit_vector(bit_vector&& other)
      : size(other.size), vec(other.vec) {
    other.vec = nullptr;
  }

  // copy assignment operator
  bit_vector& operator=(const bit_vector& other) {
    if (&other != this) {
      delete[] vec;
      size = other.size;
      vec = (uint64_t*) memcpy(new uint64_t[(other.size / 64) + 1], other.vec, sizeof(other.vec));
    }
    return *this;
  }

  // move assignment operator
  bit_vector& operator=(bit_vector&& other) {
    if (&other != this) {
      delete[] vec;
      size = other.size;
      vec = other.vec;
      other.vec = nullptr;
    }
    return *this;
  }

  // destructor
  ~bit_vector() {
    delete[] vec;
  }

  // indexing operator returning a bit ref
  bit_vector_ref operator[](std::size_t i) {
    return bit_vector_ref(&vec[i / 64], i % 64);
  }
};
