#ifndef CPP_VM_HPP
#define CPP_VM_HPP 1

#include "vm_error.hpp"

#include <iosfwd>

class context {
  typedef uint64_t stack_entry;

public:
  /* frame */
  class frame {
  private:
    context *parent;
    frame *prev;
    frame *next;
    int size;
    int top;
    stack_entry *u;

    frame(context *prnt, frame *p, int s);

    frame(const frame&);
    frame& operator=(const frame&);

  public:
    ~frame();

    friend class context;
    friend class test_context;
    friend std::ostream& operator<<(std::ostream&, const context::frame&);
  };
  
  /* Registers */
  class Registers {
    friend class context;
  };

  /* Handler */
  class Handler {
    friend class context;
  };

  /* Meta */
  class Meta {
    friend class context;
  };

  /* Errors */
  class frame_overflow : public virtual std::overflow_error,
                         public virtual vm_error {
  public:
    explicit frame_overflow(const std::string& s) :
      std::overflow_error(s)
    {
    }
  };

  class frame_underflow : public virtual std::underflow_error,
                          public virtual vm_error {
  public:
    explicit frame_underflow(const std::string& s) :
      std::underflow_error(s)
    {
    }
  };

private:
  frame *top;    //Frame stack grows from top to bottom
  frame *bottom;

  context(const context&); //private copy-constructor
  context& operator=(const context&); //private operator=

public:
  context();
  explicit context(int initial_frame_size); // no conversion constructor
  ~context();

  /* Frame management */
  frame * push_new_frame(int size);
  void pop_frame();

  /* Values on frame */
  void push_on(stack_entry);
  stack_entry pop_off(bool zero=false);

  friend class test_context;
};

#endif /* CPP_VM_HPP */
