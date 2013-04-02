#ifndef INC_TRIANGLEMATRIX_H
#define INC_TRIANGLEMATRIX_H
#include <cstddef> // size_t
#include "DataSet.h"
// Class: TriangleMatrix
/// Store the upper half of a symmetric matrix
/** Useful when calculating e.g. all N^2 distances between all atoms, 
  * where the diagonal elements would be zero and element i,j == element j,i.
  * Accepts doubles, but internal storage is float to reduce memory footprint
  */
class TriangleMatrix : public DataSet {
  public :
    size_t Nrows()             const { return nrows_;                 }
    size_t Nelements()         const { return nelements_;             }
    double operator[](int idx) const { return (double)elements_[idx]; }

    TriangleMatrix();
    TriangleMatrix(size_t);
    TriangleMatrix(const TriangleMatrix&);
    ~TriangleMatrix();
    TriangleMatrix & operator=(const TriangleMatrix &);

    int Setup(size_t);
    int AddElement(double);
    int AddElementF(float);
    void SetElement(size_t,size_t,double);
    void SetElementF(size_t,size_t,float);
    double GetElement(size_t,size_t) const;
    float GetElementF(size_t,size_t) const;
    // ----- DataSet functions -------------------
    int Allocate(int)                          { return 1;               }
    int Size()                           const { return (int)nelements_; }
    int FrameIsEmpty(int)                const { return 1;               }
    void Add(int, void*)                       { return;                 }
    void WriteBuffer(CpptrajFile&, int)  const { return;                 }
    void Write2D(CpptrajFile&, int, int) const;
    int Sync()                                 { return 1;               }
    double Dval(int)                     const { return 0.0;             }
    void Info()                          const { return;                 }
    // -------------------------------------------
  protected:
    float* elements_;       ///< Hold all elements
  private:
    size_t nrows_;          ///< Number of elements in one row
    size_t nelements_;      ///< Total number of elements
    size_t currentElement_; ///< Current element, used in AddElement only.
    size_t calcIndex(size_t,size_t) const;
};
#endif
