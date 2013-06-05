#ifndef INC_REPLICADIMARRAY_H
#define INC_REPLICADIMARRAY_H
#include <vector>
class ReplicaDimArray {
  public:
    ReplicaDimArray() {}
    enum RemDimType { UNKNOWN=0, TEMPERATURE, PARTIAL, HAMILTONIAN, PH };
    void AddRemdDimension(int d) { remDims_.push_back((RemDimType)d); }
    void clear()                 { remDims_.clear();                  }
    int Ndims()            const { return (int)remDims_.size();       }
    const char* Description(int) const;
    bool operator!=(const ReplicaDimArray&) const;
  private:
    std::vector<RemDimType> remDims_;
};
// ----- INLINE FUNCTIONS ------------------------------------------------------
const char* ReplicaDimArray::Description(int idx) const {
  if (idx >= 0 && idx < (int)remDims_.size()) {
    switch (remDims_[idx]) {
      case UNKNOWN:     return "Unknown";
      case TEMPERATURE: return "Temperature";
      case PARTIAL:     return "Partial";
      case HAMILTONIAN: return "Hamiltonian";
      case PH:          return "pH";
    }
  }
  return 0; // Sanity check, should never reach.
}
/** \return true if two replica dimension arrays do not match. */
bool ReplicaDimArray::operator!=(const ReplicaDimArray& rhs) const {
  if (remDims_.size() != rhs.remDims_.size()) return true;
  std::vector<RemDimType>::const_iterator d1 = rhs.remDims_.begin();
  for (std::vector<RemDimType>::const_iterator d0 = remDims_.begin();
                                               d0 != remDims_.end(); ++d0)
    if ( *d0 != *(d1++) ) return true;
  return false;
}
#endif
