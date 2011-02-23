#ifndef CPP_VM_HPP
#define CPP_VM_HPP 1

class Context {
public:
  class Frame {
  private:
    Frame *prev;
    Frame *next;
    int size;
    union {
      void *obp;
      unsigned long im;
    } *u;

    Frame(int s, Frame *p);

  public:
    
    friend class Context;
    friend class TestContext;
  };
  
  class Registers {
    friend class Context;
  };

  class Handler {
    friend class Context;
  };

  class Meta {
    friend class Context;
  };

private:
  Frame *top;    //Frame stack grows from top to bottom
  Frame *bottom;

  Context(const Context&); //private copy-constructor
  Context& operator=(const Context&); //private operator=

public:
  Context();
  explicit Context(int initial_frame_size); // no conversion constructor
  ~Context();

  Frame * push_new_frame(int size);
  void pop_frame();

  friend class TestContext;
};

#endif /* CPP_VM_HPP */
