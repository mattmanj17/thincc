
typedef unsigned char u8;
typedef unsigned short u16;
enum {
    rc_abort   = 0xFFFE,
    rc_exit    = 0xFFFF,
    eof     = 0xFFFF,
};
u16         get(u16 ptr);
u16         deref(u16 ptr_ptr);
void        set(u16 ptr, u16 val);
void        sub(u16 ptr, u16 val);
void        dec(u16 ptr);
void        add(u16 ptr, u16 val);
void        inc(u16 ptr);
u16         in(void);
void        out(u16 ch);
u16         run(u16 i);
