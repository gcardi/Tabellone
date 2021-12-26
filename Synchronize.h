#ifndef SynchronizeH
#define SynchronizeH
//---------------------------------------------------------------------------

template<typename LockObj>
class synchronize_lk
{
public:
    explicit synchronize_lk( LockObj& obj ) : obj_( obj ) { obj_.Enter(); }
    ~synchronize_lk() /*nothrow*/ { obj_.Leave(); }
    operator bool() const { return true; }
private:
    LockObj& obj_;
};

template<typename LockObj>
class synchronize_read_lk
{
public:
    explicit synchronize_read_lk( LockObj& obj ) : obj_( obj ) { obj_.EnterRead(); }
    ~synchronize_read_lk() /*nothrow*/ { obj_.LeaveRead(); }
    operator bool() const { return true; }
private:
    LockObj& obj_;
};

template<typename LockObj>
inline synchronize_lk<LockObj> make_lk_obj( LockObj& obj )
{
    return synchronize_lk<LockObj>( obj );
}

template<typename LockObj>
inline synchronize_read_lk<LockObj> make_read_lk_obj( LockObj& obj )
{
    return synchronize_read_lk<LockObj>( obj );
}

#define synchronize( o ) \
  if ( bool const __lk_obj_inst_of__ = make_lk_obj( o ) )

#define synchronize_read( o ) \
  if ( bool const __lk_obj_inst_of__ = make_read_lk_obj( o ) )

#define synchronize_write( o ) \
  if ( bool const __lk_obj_inst_of__ = make_lk_obj( o ) )

//---------------------------------------------------------------------------
#endif


 