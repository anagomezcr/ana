//#define ANA_VM_COMPUTED_GOTO 1

#define ANA_ABORT() ana_abort(frame)
#define ANA_TRACE_DEBUG

#ifdef ANA_VM_COMPUTED_GOTO
#   define vm_case(o)
#   define vm_target(x) do_##x:
#define vm_continue() do { \
  opline = (ana_uint32_t)((unsigned long)(code[frame->pc])); \
  opcode = (opline >> 24) & 0xff; \
  oparg = (opline >> 8) & 0xffff; \
  frame->pc++;  \
  goto *DISPATCH_TABLE[opcode]; \
} while(0)
#else 
#   define vm_case(o) switch(o)
#   define vm_target(x) case x:
#   define vm_continue() break
#endif

#define DO_TRACE(op, arg, flag, argused) do { \
    fprintf(stdout, "%-5d: ", (int)frame->pc); \
    if(argused) { \
      fprintf(stdout, "%-15s%-10d\n", op, arg); \
    } else {\
      fprintf(stdout, "%-15s\n", op); \
    } \
} while(0)

#ifdef ANA_TRACE_DEBUG
#   define TRACE(op, arg, flag, argused) if(1) {\
      if(vm->flags & COMO_VM_TRACING) { \
        DO_TRACE(#op, arg, flag, argused); \
        if(op != IRETURN) \
          break; \
        else \
          goto exit; \
      } \
      else if(vm->flags & COMO_VM_LIVE_TRACING) {\
        DO_TRACE(#op, arg, flag, argused); \
      } \
  } \

#else
#   define TRACE(op, arg, flag, argused) do {} while(0)
#endif


#define fetch() do { \
  opline = (ana_uint32_t)((unsigned long)(code[frame->pc])); \
  opcode = (opline >> 24) & 0xff; \
  oparg = (opline >> 8) & 0xffff; \
  frame->pc++;  \
} while(0)

#define get_const(x) (constants[x])
#define get_arg() (oparg)

#define push_ex(frame, arg) do { \
  if(frame->sp >= frame->sz) \
  { \
    frame->sz = frame->sz * 2; \
    frame->stack = realloc(frame->stack, sizeof(ana_object *) * frame->sz); \
  } \
  frame->stack[frame->sp++] = arg; \
} while(0)

#define push(x) push_ex(frame, x)

#define pushto(frame, arg) \
  push_ex(frame, arg)
  
#define pop() \
  (frame->stack[--frame->sp])

#define empty() \
  (frame->sp == 0)

#define set_except(exception_type, fmt, ...) do { \
  ex_type = exception_type; \
  ex = make_except(fmt, ##__VA_ARGS__); \
} while(0)
