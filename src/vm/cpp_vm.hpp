#ifndef CPP_VM_HPP
#define CPP_VM_HPP 1

#include <iosfwd>

class Context {
  typedef uint64_t stack_entry;

public:
  /* Frame */
  class Frame {
  private:
    Frame *prev;
    Frame *next;
    int size;
    int top;
    stack_entry *u;

    Frame(int s, Frame *p);

    Frame(const Frame&);
    Frame& operator=(const Frame&);

  public:
    ~Frame();

    friend class Context;
    friend class TestContext;
    friend std::ostream& operator<<(std::ostream&, const Context::Frame&);
  };
  
  /* Registers */
  class Registers {
    friend class Context;
  };

  /* Handler */
  class Handler {
    friend class Context;
  };

  /* Meta */
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

  /* Frame management */
  Frame * push_new_frame(int size);
  void pop_frame();

  /* Values on frame */
  void push_on(stack_entry);
  stack_entry pop_off(bool zero=false);

  friend class TestContext;
};

#endif /* CPP_VM_HPP */
