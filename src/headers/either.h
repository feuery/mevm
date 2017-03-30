#ifndef EITHER_H
#define EITHER_H

template<typename A, typename B>
struct Either {
  A* a = nullptr;
  B* b = nullptr;

  bool delete_on_exit;

  ~Either() {
    if(delete_on_exit) {
      if (a) delete a;
      if (b) delete b;
    }
  }

  void operator=(Either<A, B> &b) {
  // a.delete_on_exit = b.delete_on_exit = false;
    a = b.a;
    b = b.b;
  }
};

#endif //EITHER_H
